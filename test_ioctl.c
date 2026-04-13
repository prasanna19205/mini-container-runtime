#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "monitor_ioctl.h"

int main() {
    int fd = open("/dev/container_monitor", O_RDWR);

    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    struct container_request req;

    req.pid = getpid();
    req.soft_limit = 2000000;   // 2 MB
    req.hard_limit = 3000000;   // 3 MB

    printf("Adding PID %d...\n", req.pid);
    ioctl(fd, ADD_CONTAINER, &req);

    sleep(15);  // allow monitoring

    printf("Removing PID %d...\n", req.pid);
    ioctl(fd, REMOVE_CONTAINER, &req.pid);

    close(fd);
    return 0;
}


