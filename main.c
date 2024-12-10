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
GrayscaleImage *create_image(int width, int height) {
    GrayscaleImage *image = (GrayscaleImage *) malloc(sizeof(GrayscaleImage));
    image->width = width;
    image->height = height;
    image->pitch = width;
    image->data = (unsigned char *) malloc(width * height);
    return image;
}

// 清空图像数据
void clear_image(GrayscaleImage *image) {
    if (image) {
        memset(image->data, 255, image->width * image->height); // 背景设为白色
    }
}

// 释放图像内存
void free_image(GrayscaleImage *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}

// 渲染字符到灰度图像
void render_glyph_to_image(FT_Face face, const wchar_t *text, GrayscaleImage *image) {
    int x_offset = 50; // X 轴偏移
    int y_offset = 50; // Y 轴偏移

    for (int i = 0; text[i] != '\0'; ++i) {
        unsigned int codepoint = text[i];  // 获取字符的 Unicode 码点
        FT_Load_Char(face, codepoint, FT_LOAD_RENDER);  // 渲染字符
        FT_GlyphSlot slot = face->glyph;

        // 获取字形的宽度和高度
        int width = slot->bitmap.width;
        int height = slot->bitmap.rows;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int pixel_value = slot->bitmap.buffer[y * width + x];
                if (pixel_value > 0) {
                    image->data[(y_offset + y) * image->width + (x_offset + x)] = 255 - pixel_value; // 反转成黑色文字
                }
            }
        }

        // 更新偏移量
        x_offset += slot->bitmap.width + 10; // 每个字之间留些空白
    }
}

// 保存灰度图像为 PGM 格式
void save_image_as_pgm(const char *filename, GrayscaleImage *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file for saving image");
        return;
    }
    fprintf(file, "P5\n%d %d\n255\n", image->width, image->height);
    fwrite(image->data, 1, image->width * image->height, file);
    fclose(file);
}

int main() {
    FT_Library library;
    FT_Face face;
    const char *font_path = "/opt/hei.TTF";  // 替换为合适的字体文件路径

    // 初始化 FreeType 库
    if (FT_Init_FreeType(&library)) {
        fprintf(stderr, "Could not initialize FreeType library\n");
        return 1;
    }

    // 加载字体
    if (FT_New_Face(library, font_path, 0, &face)) {
        fprintf(stderr, "Could not open font file %s\n", font_path);
        return 1;
    }

    // 设置字体大小
    if (FT_Set_Pixel_Sizes(face, 0, 24)) {
        fprintf(stderr, "Could not set font size\n");
        return 1;
    }

    // 创建灰度图像
    GrayscaleImage *image = create_image(320, 100);
    clear_image(image);

    // 渲染汉字 "我爱中国"
    const wchar_t *text = L"我爱中国";
    render_glyph_to_image(face, text, image);

    // 保存为 PGM 格式
    save_image_as_pgm("/opt/output.pgm", image);

    // 清理
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    free_image(image);

    printf("汉字图片已保存为 output.pgm\n");
    return 0;
}
