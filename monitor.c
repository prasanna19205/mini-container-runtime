#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include "monitor_ioctl.h"

#define DEVICE_NAME "container_monitor"

static int major;

/* ---------------- DATA STRUCTURE ---------------- */

struct container_info {
    int pid;
    unsigned long soft_limit;
    unsigned long hard_limit;
    struct list_head list;
};

static LIST_HEAD(container_list);
static struct task_struct *monitor_thread;

/* ---------------- MEMORY FUNCTION ---------------- */

unsigned long get_memory_usage(struct task_struct *task) {
    if (!task->mm)
        return 0;

    return task->mm->total_vm << PAGE_SHIFT;
}

/* ---------------- LIST FUNCTIONS ---------------- */

void add_container(int pid, unsigned long soft, unsigned long hard) {
    struct container_info *new;

    new = kmalloc(sizeof(*new), GFP_KERNEL);
    if (!new)
        return;

    new->pid = pid;
    new->soft_limit = soft;
    new->hard_limit = hard;

    list_add(&new->list, &container_list);

    printk(KERN_INFO "Added PID %d (soft=%lu hard=%lu)\n",
           pid, soft, hard);
}

void remove_container(int pid) {
    struct container_info *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &container_list, list) {
        if (entry->pid == pid) {
            list_del(&entry->list);
            kfree(entry);
            printk(KERN_INFO "Removed PID %d\n", pid);
            return;
        }
    }

    printk(KERN_INFO "PID %d not found\n", pid);
}

/* ---------------- DEVICE FUNCTIONS ---------------- */

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

/* ---------------- IOCTL ---------------- */

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct container_request req;
    int pid;

    switch (cmd) {

        case ADD_CONTAINER:
            if (copy_from_user(&req, (void *)arg, sizeof(req)))
                return -EFAULT;

            add_container(req.pid, req.soft_limit, req.hard_limit);
            break;

        case REMOVE_CONTAINER:
            if (copy_from_user(&pid, (int *)arg, sizeof(pid)))
                return -EFAULT;

            remove_container(pid);
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

/* ---------------- FILE OPS ---------------- */

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
};

/* ---------------- MONITOR THREAD ---------------- */

int monitor_fn(void *data) {
    struct container_info *entry;

    while (!kthread_should_stop()) {

        list_for_each_entry(entry, &container_list, list) {

            struct task_struct *task;
            task = pid_task(find_vpid(entry->pid), PIDTYPE_PID);

            if (task) {
                unsigned long mem = get_memory_usage(task);

                printk(KERN_INFO "PID %d memory: %lu bytes\n",
                       entry->pid, mem);

                /* SOFT LIMIT */
                if (mem > entry->soft_limit) {
                    printk(KERN_WARNING "PID %d exceeded SOFT limit\n",
                           entry->pid);
                }

                /* HARD LIMIT */
                if (mem > entry->hard_limit) {
                    printk(KERN_ALERT "PID %d exceeded HARD limit — killing\n",
                           entry->pid);
                    send_sig(SIGKILL, task, 0);
                }

            } else {
                printk(KERN_INFO "PID %d not found\n", entry->pid);
            }
        }

        ssleep(5);
    }

    return 0;
}

/* ---------------- INIT ---------------- */

static int __init monitor_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major;
    }

    monitor_thread = kthread_run(monitor_fn, NULL, "monitor_thread");

    printk(KERN_INFO "Device registered with major %d\n", major);
    return 0;
}

/* ---------------- EXIT ---------------- */

static void __exit monitor_exit(void) {
    struct container_info *entry, *tmp;

    if (monitor_thread)
        kthread_stop(monitor_thread);

    list_for_each_entry_safe(entry, tmp, &container_list, list) {
        list_del(&entry->list);
        kfree(entry);
    }

    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");

