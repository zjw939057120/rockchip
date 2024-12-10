#include "freetype_test.h"

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
    if (FT_Set_Pixel_Sizes(face, 0, 28)) {
        fprintf(stderr, "Could not set font size\n");
        return 1;
    }

    // 创建灰度图像
    GrayscaleImage *image = create_image(320, 48);
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
