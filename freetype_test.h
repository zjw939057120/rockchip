//
// Created by zjw93 on 2024/12/10.
//

#ifndef MPP_FREETYPE_TEST_H
#define MPP_FREETYPE_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

// 定义灰度图像结构
typedef struct {
    unsigned char *data;
    int width;
    int height;
    int pitch;
} GrayscaleImage;

// 创建一个新的灰度图像
GrayscaleImage *create_image(int width, int height);

// 清空图像数据
void clear_image(GrayscaleImage *image);

// 释放图像内存
void free_image(GrayscaleImage *image);

// 渲染字符到灰度图像
void render_glyph_to_image(FT_Face face, const wchar_t *text, GrayscaleImage *image);

// 保存灰度图像为 PGM 格式
void save_image_as_pgm(const char *filename, GrayscaleImage *image);

#endif //MPP_FREETYPE_TEST_H
