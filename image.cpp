//
// Created by avo_m on 25.03.2023.
//

// #pragma once
#include "image.h"

Image::Image(int64_t height, int64_t width) {
    height_ = height;
    width_ = width;
    vec_colors = std::vector<Color>(height * width);
    pix_matrix = std::vector<std::vector<Color>>(height, std::vector<Color>(width));
}

Image::~Image() = default;

Color Image::GetColor(int64_t x, int64_t y) const {
    return vec_colors[y * width_ + x];
}

void Image::SetColor(const Color& color, int64_t x, int64_t y) {
    vec_colors[y * width_ + x] = color;
    pix_matrix[y][x] = color;
}

void Image::ExportImage(const char* path) const {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        throw std::exception();
    }

    unsigned char padding[3] = {0, 0, 0};
    const int64_t cnt_padding = (4 - ((width_ * 3) % 4)) % 4;
    const int64_t file_header_size = 14;
    const int64_t dib_header_size = 40;
    const int64_t whole_file_size = file_header_size + dib_header_size + width_ * height_ * 3 + cnt_padding * height_;

    unsigned char file_header[file_header_size];

    // у любого bmp файла такое начало - кодировка
    file_header[0] = 'B';
    file_header[1] = 'M';

    // размер файла
    const int64_t c_b_size = 8;

    const int64_t f_header_c = 2;

    file_header[f_header_c] = whole_file_size;
    file_header[f_header_c + 1] = whole_file_size >> c_b_size;
    file_header[f_header_c + 2] = whole_file_size >> (c_b_size * 2);
    file_header[f_header_c + 3] = whole_file_size >> (c_b_size * 3);

    // не используется, какие-то резервы 1, 2

    const int64_t f_header_c2 = 6;

    file_header[f_header_c2] = 0;
    file_header[f_header_c2 + 1] = 0;
    file_header[f_header_c2 + 2] = 0;
    file_header[f_header_c2 + 3] = 0;

    // pixel data offset

    const int64_t pix_data_c = 10;

    file_header[pix_data_c] = file_header_size + dib_header_size;
    file_header[pix_data_c + 1] = 0;
    file_header[pix_data_c + 2] = 0;
    file_header[pix_data_c + 3] = 0;

    unsigned char dib_header[dib_header_size];

    // размер файла

    dib_header[0] = dib_header_size;
    dib_header[1] = 0;
    dib_header[2] = 0;
    dib_header[3] = 0;

    // ширина картинки
    const int64_t w_c = 4;

    dib_header[w_c] = width_;
    dib_header[w_c + 1] = width_ >> c_b_size;
    dib_header[w_c + 2] = width_ >> (c_b_size * 2);
    dib_header[w_c + 3] = width_ >> (c_b_size * 3);

    // высота картинки
    const int64_t h_c = 8;

    dib_header[h_c] = height_;
    dib_header[h_c + 1] = height_ >> c_b_size;
    dib_header[h_c + 2] = height_ >> (c_b_size * 2);
    dib_header[h_c + 3] = height_ >> (c_b_size * 3);

    // planes

    const int64_t planes_c = 12;

    dib_header[planes_c] = 1;
    dib_header[planes_c + 1] = 0;

    // bits per pixel (RGB)

    const int64_t bpp_c = 14;

    dib_header[bpp_c] = c_b_size * 3;
    dib_header[bpp_c + 1] = 0;

    // сжатие и тд (чего нет)
    const int64_t other_c = 16;
    dib_header[other_c] = 0;
    dib_header[other_c + 1] = 0;
    dib_header[other_c + 2] = 0;
    dib_header[other_c + 3] = 0;
    dib_header[other_c + 4] = 0;

    const int64_t other_c2 = 21;

    dib_header[other_c2] = 0;
    dib_header[other_c2 + 1] = 0;
    dib_header[other_c2 + 2] = 0;
    dib_header[other_c2 + 3] = 0;
    dib_header[other_c2 + 4] = 0;

    const int64_t other_c3 = 26;
    dib_header[other_c3] = 0;
    dib_header[other_c3 + 1] = 0;
    dib_header[other_c3 + 2] = 0;
    dib_header[other_c3 + 3] = 0;
    dib_header[other_c3 + 4] = 0;

    const int64_t other_c4 = 31;

    dib_header[other_c4] = 0;
    dib_header[other_c4 + 1] = 0;
    dib_header[other_c4 + 2] = 0;
    dib_header[other_c4 + 3] = 0;
    dib_header[other_c4 + 4] = 0;

    const int64_t other_c5 = 36;

    dib_header[other_c5] = 0;
    dib_header[other_c5 + 1] = 0;
    dib_header[other_c5 + 2] = 0;
    dib_header[other_c5 + 3] = 0;

    f.write(reinterpret_cast<char*>(file_header), file_header_size);
    f.write(reinterpret_cast<char*>(dib_header), dib_header_size);

    const double to_char_const = 255.0f;

    for (int64_t y = 0; y < height_; ++y) {
        for (int64_t x = 0; x < width_; ++x) {
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * to_char_const);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * to_char_const);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * to_char_const);

            unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(padding), cnt_padding);
    }

    f.close();

    std::cout << "BMP-изображение было создано\n";
}

void Image::Read(const char* path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);

    if (!f.is_open()) {
        throw std::exception();
    }

    const int64_t file_header_size = 14;
    const int64_t dib_header_size = 40;

    unsigned char file_header[file_header_size];
    f.read(reinterpret_cast<char*>(file_header), file_header_size);

    if (file_header[0] != 'B' || file_header[1] != 'M') {
        f.close();
        throw std::invalid_argument("Предоставлен файл формата, отличного от BMP\n");
    }

    unsigned char dib_header[dib_header_size];
    f.read(reinterpret_cast<char*>(dib_header), dib_header_size);

    const int64_t c_b_size = 8;
    const int64_t c5 = 5;
    //    int64_t whole_file_size = file_header[2] + (file_header[3] << c_b_size) + (file_header[4] << c_b_size * 2) +
    //    (file_header[c5] << c_b_size * 3);

    width_ = dib_header[4] + (dib_header[c5] << c_b_size) + (dib_header[c5 + 1] << (c_b_size * 2)) +
             (dib_header[c5 + 2] << (c_b_size * 3));
    height_ = dib_header[c_b_size] + (dib_header[c_b_size + 1] << c_b_size) +
              (dib_header[c_b_size + 2] << (c_b_size * 2)) + (dib_header[c_b_size + 3] << (c_b_size * 3));

    vec_colors.resize(width_ * height_);

    const double to_char_const = 255.0f;

    const int64_t padding_cnt = (4 - ((width_ * 3) % 4)) % 4;

    for (int64_t y = 0; y < height_; ++y) {
        for (int64_t x = 0; x < width_; ++x) {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);

            vec_colors[y * width_ + x].r = static_cast<double>(color[2]) / to_char_const;
            vec_colors[y * width_ + x].g = static_cast<double>(color[1]) / to_char_const;
            vec_colors[y * width_ + x].b = static_cast<double>(color[0]) / to_char_const;
        }
        f.ignore(padding_cnt);
    }

    pix_matrix.clear();
    for (int64_t i = 0; i < height_; ++i) {
        std::vector<Color> tmp;
        for (int64_t j = 0; j < width_; ++j) {
            tmp.push_back(vec_colors[i * width_ + j]);
        }
        pix_matrix.push_back(tmp);
    }

    f.close();
    std::cout << "BMP-изображение было прочитано\n";
}
