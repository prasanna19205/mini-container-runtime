#ifndef MONITOR_IOCTL_H
#define MONITOR_IOCTL_H

#include <linux/ioctl.h>

struct container_request {
    int pid;
    unsigned long soft_limit;
    unsigned long hard_limit;
};

#define ADD_CONTAINER _IOW('a', 'a', struct container_request)
#define REMOVE_CONTAINER _IOW('a', 'b', int)

#endif
