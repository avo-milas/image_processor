//
// Created by avo_m on 25.03.2023.
//

#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#endif  // CPP_HSE_IMAGE_H

#pragma once
#include "color.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

class Image {
public:
    Image() = default;
    Image(int64_t height, int64_t width);
    ~Image();

    Color GetColor(int64_t x, int64_t y) const;
    void SetColor(const Color& color, int64_t x, int64_t y);
    void ExportImage(const char* path) const;
    void Read(const char* path);
    std::vector<Color> vec_colors;
    std::vector<std::vector<Color>> pix_matrix;
    int64_t width_;
    int64_t height_;
};
