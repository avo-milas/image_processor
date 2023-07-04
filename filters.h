//
// Created by avo_m on 25.03.2023.
//

#ifndef CPP_HSE_FILTERS_H
#define CPP_HSE_FILTERS_H

#endif  // CPP_HSE_FILTERS_H

#include "image.h"
#include <cmath>
#include <functional>
#include <algorithm>
#include <math.h>

class Filter {
public:
    Filter() = default;
    explicit Filter(Image& im);
    Image* im_ref;

    static Color& NearestPix(int64_t x, int64_t y, Image& prev_image);

protected:
    void MatrixFilter(std::vector<int64_t> a) const;
    // проход по матрице фильтра слева-направо сверху-вниз
};

class Negative : public Filter {
public:
    explicit Negative(Image& im);
};

class Grayscale : public Filter {
public:
    explicit Grayscale(Image& im);
};

class Crop : public Filter {
public:
    explicit Crop(Image& im, int64_t n_w, int64_t n_h);
};

class Sharpening : public Filter {
public:
    explicit Sharpening(Image& im);
};

class EdgeDetection : public Filter {
public:
    explicit EdgeDetection(Image& im, double threshold);
};

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(Image& im, double sigma);
};

class Glass : public Filter {
public:
    explicit Glass(Image& im);
};