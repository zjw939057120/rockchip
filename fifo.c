//
// Created by zjw93 on 2024/11/26.
//

#include <unistd.h>
#include <sys/stat.h>
#include "fifo.h"

void fifo_init() {
    unlink(FIFO_NAME_1);
    if (mkfifo(FIFO_NAME_1, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_1);
    }
    unlink(FIFO_NAME_2);
    if (mkfifo(FIFO_NAME_2, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_3);
    }
    unlink(FIFO_NAME_3);
    if (mkfifo(FIFO_NAME_3, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_3);
    }
    unlink(FIFO_NAME_4);
    if (mkfifo(FIFO_NAME_4, 0666) == -1) {
        perror("mkfifo "FIFO_NAME_4);
    }
}

void fifo_write(void *buf, size_t len, FILE *fp) {
    printf("fifo_write: %d,%d\n", buf, len);
    size_t send = 0;
    while (send < len) {
        fwrite(buf + send, 1, FIFO_MAX_SIZE, fp);
        send += FIFO_MAX_SIZE;
        if (send == len)return;
    }
}

void fifo_read(void *buf, size_t len, FILE *fp) {
    printf("fifo_read: %d,%d\n", buf, len);
    size_t rev = 0;
    while (rev < len) {
        fread(buf + rev, 1, FIFO_MAX_SIZE, fp);
        rev += FIFO_MAX_SIZE;
        if (rev == len)return;
    }
}