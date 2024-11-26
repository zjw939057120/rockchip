//
// Created by zjw93 on 2024/11/26.
//

#ifndef _FIFO_H
#define _FIFO_H

#include <stdio.h>

#define FIFO_MAX_SIZE 65536
#define FIFO_NAME_1 "/tmp/my_fifo_1"
#define FIFO_NAME_2 "/tmp/my_fifo_2"
#define FIFO_NAME_3 "/tmp/my_fifo_3"
#define FIFO_NAME_4 "/tmp/my_fifo_4"

void fifo_init();

void fifo_write(void *buf, size_t len,FILE *fp);

void fifo_read(void *buf, size_t len,FILE *fp);

#endif //_FIFO_H
