#include <locale.h>
#include "freetype_test.h"

GrayscaleImage *create_image(int width, int height) {
    setlocale(LC_ALL, "zh_CN.utf8");
    GrayscaleImage *image = (GrayscaleImage *) malloc(sizeof(GrayscaleImage));
    image->width = width;
    image->height = height;
    image->pitch = width;
    image->data = (unsigned char *) malloc(width * height);
    return image;
}

void clear_image(GrayscaleImage *image) {
    if (image) {
        memset(image->data, 255, image->width * image->height); // 背景设为白色
    }
}

void free_image(GrayscaleImage *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}

void render_glyph_to_image(FT_Face face, const char text[], GrayscaleImage *image) {
    wchar_t wchar[128] = {0};
    mbstowcs(wchar,text,strlen(text));

    int x_offset = 2; // X 轴偏移
    int y_offset = 10; // Y 轴偏移

    for (int i = 0; wchar[i] != '\0'; ++i) {
        unsigned int codepoint = wchar[i];  // 获取字符的 Unicode 码点
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
        x_offset += slot->bitmap.width + 2; // 每个字之间留些空白
    }
}

void save_image_as_pgm(const char *filename, GrayscaleImage *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file for saving image");
        return;
    }
    fprintf(file, "P5\n%d %d\n255\n", image->width, image->height);
    fwrite(image->data, 1, image->width * image->height, file);

    int gray_index = 0;
    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            // 计算该像素在数据中的位置
            int pixel_value = image->data[y * image->pitch + x];

            // 打印像素值
            //printf("Pixel at (%d, %d): %d\n", x, y, pixel_value);
            printf("%s", pixel_value != 255 ? "@" : " ");
            if (gray_index % 320 == 0) {
                printf("\n");
            }
            gray_index++;
        }
    }
    fclose(file);
}

