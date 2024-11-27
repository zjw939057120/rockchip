//
// Created by zjw93 on 2024/11/26.
//

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "fifo.h"

void fifo_init() {
    unlink(FIFO_NAME_1);
    if (mkfifo(FIFO_NAME_1, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_1);
    }
    unlink(FIFO_NAME_2);
    if (mkfifo(FIFO_NAME_2, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_2);
    }
    unlink(FIFO_NAME_3);
    if (mkfifo(FIFO_NAME_3, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_4);
    }
    unlink(FIFO_NAME_4);
    if (mkfifo(FIFO_NAME_4, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_4);
    }
}

int fifo_write_open(const char *file) {
    return open(file, O_WRONLY | O_NONBLOCK);
}

int fifo_read_open(const char *file) {
    return open(file, O_RDONLY);
}

void fifo_write(int fd, void *buf, size_t len) {
    printf("fifo_write start %d %zu\n", buf, len);
    size_t send = 0;
    while (send < len) {
        write(fd, buf + send, FIFO_MAX_SIZE);
        send += FIFO_MAX_SIZE;
    }
    printf("fifo_write done %d %zu\n", buf, len);
}

void fifo_read(int fd, void *buf, size_t len) {
    printf("fifo_read start %p %zu\n", buf, len);
    size_t rev = 0;
    while (rev < len) {
        read(fd, buf + rev, FIFO_MAX_SIZE);
        rev += FIFO_MAX_SIZE;
    }
    printf("fifo_read done %p %zu\n", buf, len);
}