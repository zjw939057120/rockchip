//
// Created by zjw93 on 2024/11/26.
//

#ifndef _FIFO_H
#define _FIFO_H

#include <stdio.h>

#define FIFO_BUF_SIZE 1382400
#define FIFO_MAX_SIZE 65535 //64KB
#define FIFO_NAME_1 "/tmp/mpp_fifo_1"
#define FIFO_NAME_2 "/tmp/mpp_fifo_2"
#define FIFO_NAME_3 "/tmp/mpp_fifo_3"
#define FIFO_NAME_4 "/tmp/mpp_fifo_4"

void fifo_init();

int fifo_write_open(const char *file);

int fifo_read_open(const char *file);

void fifo_write(int fd, void *buf, size_t len);

void fifo_read(int fd, void *buf, size_t len);

#endif //_FIFO_H
