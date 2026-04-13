#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "monitor_ioctl.h"

#define STACK_SIZE 1024 * 1024
#define MAX_CONTAINERS 10

typedef struct {
    int id;
    pid_t pid;
    char state[20];
} container_t;

container_t containers[MAX_CONTAINERS];
int container_count = 0;

static char stacks[MAX_CONTAINERS][STACK_SIZE];

int monitor_fd;   // GLOBAL DEVICE FD

// ---------------- LOGGING ----------------

void remove_container_from_monitor(int pid) {
    ioctl(monitor_fd, REMOVE_CONTAINER, &pid);
}

void read_logs(int fd, int pid) {
    char buffer[256];

    while (1) {
        int n = read(fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;

        buffer[n] = '\0';
        printf("[PID %d]: %s", pid, buffer);
    }

    // REMOVE FROM MONITOR AFTER EXIT
    remove_container_from_monitor(pid);
}

void *log_thread(void *arg) {
    int *data = (int *)arg;

    read_logs(data[0], data[1]);

    free(data);
    return NULL;
}

// ---------------- CONTAINER ----------------

int container_func(void *arg) {
    int *pipefd = (int *)arg;

    close(pipefd[0]);

    dup2(pipefd[1], STDOUT_FILENO);
    dup2(pipefd[1], STDERR_FILENO);

    sethostname("container", 9);

    printf("Container started!\n");

    // LONG RUNNING PROCESS (IMPORTANT)
    execlp("/bin/sh", "sh", "-c",
           "while true; do echo Running...; sleep 1; done",
           NULL);

    perror("exec failed");
    return 1;
}

// ---------------- START CONTAINER ----------------

void start_container() {

    if (container_count >= MAX_CONTAINERS) {
        printf("Max containers reached\n");
        return;
    }

    int pipefd[2];
    pipe(pipefd);

    int id = container_count;

    pid_t pid = clone(container_func,
                      stacks[id] + STACK_SIZE,
                      CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD,
                      pipefd);

    if (pid < 0) {
        perror("clone failed");
        return;
    }

    containers[id].id = id;
    containers[id].pid = pid;
    strcpy(containers[id].state, "running");

    container_count++;

    printf("Started container %d (PID: %d)\n", id, pid);

    // ---------------- SEND TO MONITOR ----------------

    struct container_request req;
    req.pid = pid;
    req.soft_limit = 500000;   // 1 MB
    req.hard_limit =  1000000;   // 2 MB

    ioctl(monitor_fd, ADD_CONTAINER, &req);

    // ------------------------------------------------

    close(pipefd[1]);

    pthread_t tid;

    int *data = malloc(2 * sizeof(int));
    data[0] = pipefd[0];
    data[1] = pid;

    pthread_create(&tid, NULL, log_thread, data);
}

// ---------------- LIST ----------------

void list_containers() {
    printf("\nContainers:\n");

    for (int i = 0; i < container_count; i++) {
        printf("ID: %d | PID: %d | STATE: %s\n",
               containers[i].id,
               containers[i].pid,
               containers[i].state);
    }
}

// ---------------- CLI ----------------

void cli_loop() {
    char cmd[100];

    while (1) {
        printf("\nengine> ");
        scanf("%s", cmd);

        if (strcmp(cmd, "run") == 0) {
            start_container();
        }
        else if (strcmp(cmd, "ps") == 0) {
            list_containers();
        }
        else if (strcmp(cmd, "exit") == 0) {
            break;
        }
        else {
            printf("Commands: run | ps | exit\n");
        }
    }
}

// ---------------- MAIN ----------------

int main() {

    // OPEN MONITOR DEVICE
    monitor_fd = open("/dev/container_monitor", O_RDWR);

    if (monitor_fd < 0) {
        perror("Failed to open monitor device");
        return 1;
    }

    printf("Mini Container Engine Started\n");

    cli_loop();

    close(monitor_fd);

    return 0;
}
