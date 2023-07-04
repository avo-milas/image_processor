//
// Created by avo_m on 25.03.2023.
//

#include "filters.h"
#include <iostream>

Filter::Filter(Image& im) {
    im_ref = &im;
}

Color& Filter::NearestPix(int64_t x, int64_t y, Image& prev_image) {
    if (0 <= x && x < prev_image.width_ && 0 <= y && y < prev_image.height_) {
        return prev_image.pix_matrix[y][x];
    }
    if (x <= -1) {
        if (y <= -1) {
            return prev_image.pix_matrix[0][0];
        } else if (y >= prev_image.height_) {
            return prev_image.pix_matrix[prev_image.height_ - 1][0];
        } else {
            return prev_image.pix_matrix[y][0];
        }
    } else if (x >= prev_image.width_) {
        if (y <= -1) {
            return prev_image.pix_matrix[0][prev_image.width_ - 1];
        } else if (y >= prev_image.height_) {
            return prev_image.pix_matrix[prev_image.height_ - 1][prev_image.width_ - 1];
        } else {
            return prev_image.pix_matrix[y][prev_image.width_ - 1];
        }
    } else if (y <= -1) {
        return prev_image.pix_matrix[0][x];
    } else {
        return prev_image.pix_matrix[prev_image.height_ - 1][x];
    }
}

void Filter::MatrixFilter(std::vector<int64_t> a) const {
    const int64_t cnt_of_near_pix = 8;
    std::vector<int64_t> delta_y = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    std::vector<int64_t> delta_x = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

    Image prev_image_1 = *im_ref;

    for (int64_t y = 0; y < im_ref->height_; ++y) {
        for (int64_t x = 0; x < im_ref->width_; ++x) {
            double new_r = 0;
            double new_g = 0;
            double new_b = 0;
            for (int64_t i = 0; i <= cnt_of_near_pix; ++i) {
                Color new_col = NearestPix(x + delta_x[i], y + delta_y[i], prev_image_1);
                new_r += static_cast<double>(a[i]) * new_col.r;
                new_g += static_cast<double>(a[i]) * new_col.g;
                new_b += static_cast<double>(a[i]) * new_col.b;
            }
            Color c(std::min(1.0, std::max(0.0, new_r)), std::min(1.0, std::max(0.0, new_g)),
                    std::min(1.0, std::max(0.0, new_b)));
            im_ref->pix_matrix[y][x] = c;
            im_ref->vec_colors[y * im_ref->width_ + x] = c;
        }
    }
}

// void Filter::ForPixInPic(Filter::Operation* operation) const {
//    for (int64_t y = 0; y < im_ref->height_; ++y) {
//        for (int64_t x = 0; x < im_ref->width_; ++x) {
//            Color new_col = operation(x, y, *im_ref);
//            im_ref->pix_matrix[y][x] = new_col;
//            im_ref->vec_colors[y * im_ref->width_ + x] = new_col;
//        }
//    }
//}

Negative::Negative(Image& im) : Filter(im) {
    for (int64_t y = 0; y < im_ref->height_; ++y) {
        for (int64_t x = 0; x < im_ref->width_; ++x) {
            Color col = im_ref->pix_matrix[y][x];
            Color new_col = Color(1 - col.r, 1 - col.g, 1 - col.b);
            im_ref->pix_matrix[y][x] = new_col;
            im_ref->vec_colors[y * im_ref->width_ + x] = new_col;
        }
    }
}

Grayscale::Grayscale(Image& im) : Filter(im) {
    const double gray_red = 0.299;
    const double gray_green = 0.587;
    const double gray_blue = 0.114;
    for (int64_t y = 0; y < im_ref->height_; ++y) {
        for (int64_t x = 0; x < im_ref->width_; ++x) {
            Color col = im_ref->pix_matrix[y][x];
            double new_col_d = gray_red * col.r + gray_green * col.g + gray_blue * col.b;
            Color new_col = Color(new_col_d, new_col_d, new_col_d);
            im_ref->pix_matrix[y][x] = new_col;
            im_ref->vec_colors[y * im_ref->width_ + x] = new_col;
        }
    }
}

Crop::Crop(Image& im, int64_t n_w, int64_t n_h) : Filter(im) {

    int64_t new_height = std::min(im.height_, n_h);
    int64_t new_width = std::min(im.width_, n_w);

    std::vector<std::vector<Color>> tmp_matrix;

    for (int64_t i = 0; i < new_height; ++i) {
        im.pix_matrix[im.height_ - 1 - i].resize(new_width);
    }

    for (int64_t i = 0; i < new_height; ++i) {
        tmp_matrix.push_back(im.pix_matrix[im.height_ - new_height + i]);
    }

    //        for (int64_t i =0; i < new_height; ++i) {
    //            tmp_matrix.push_back(im.pix_matrix[im.height_ - 1 - i]);
    //        }

    std::swap(im.pix_matrix, tmp_matrix);

    im.height_ = new_height;
    im.width_ = new_width;

    im.vec_colors.clear();
    for (int64_t i = 0; i < im.height_; ++i) {
        for (int64_t j = 0; j < im.width_; ++j) {
            im.vec_colors.push_back(im.pix_matrix[i][j]);
        }
    }
}

Sharpening::Sharpening(Image& im) : Filter(im) {
    const int64_t c5 = 5;
    Filter::MatrixFilter({0, -1, 0, -1, c5, -1, 0, -1, 0});
}

EdgeDetection::EdgeDetection(Image& im, double threshold) : Filter(im) {
    Grayscale gr(im);
    Filter::MatrixFilter({0, -1, 0, -1, 4, -1, 0, -1, 0});

    for (int64_t i = 0; i < im.height_; ++i) {
        for (int64_t j = 0; j < im.width_; ++j) {
            double nc = im.pix_matrix[i][j].r > threshold ? 1.0 : 0.0;
            Color c(nc, nc, nc);
            im.pix_matrix[i][j] = c;
            im.vec_colors[i * im.width_ + j] = c;
        }
    }
}

GaussianBlur::GaussianBlur(Image& im, double sigma) : Filter(im) {
    int radius = ceil(3 * sigma);
    int vec_size = static_cast<int>(2 * radius + 1);
    std::vector<double> vec_coefficients(vec_size);
    double k1 = 1.0 / sqrt((2 * M_PI * sigma * sigma));

    // подсчет коэффициентов в зависимости от расстояния
    for (int q = 0; q <= radius; ++q) {
        double k2 = k1 * exp(-(q * q) / (2 * sigma * sigma));
        vec_coefficients[radius - q] = k2;
        vec_coefficients[radius + q] = k2;
    }

    Image prev_image = *im_ref;
    // 1 проход (по горизонтали)
    for (int64_t y = 0; y < im_ref->height_; ++y) {
        for (int64_t x = 0; x < im_ref->width_; ++x) {
            Color new_col = Color(0, 0, 0);
            for (int q = -radius; q <= radius; ++q) {
                new_col += Filter::NearestPix(x + q, y, prev_image) * vec_coefficients[radius + q];
            }
            Color new_col_true = Color(std::min(1.0, std::max(0.0, new_col.r)), std::min(1.0, std::max(0.0, new_col.g)),
                                       std::min(1.0, std::max(0.0, new_col.b)));
            im_ref->pix_matrix[y][x] = new_col_true;
            im_ref->vec_colors[y * im_ref->width_ + x] = new_col_true;
        }
    }

    // новый prev_im
    prev_image = *im_ref;
    //     2 проход (вертикально)
    for (int64_t y = 0; y < im_ref->height_; ++y) {
        for (int64_t x = 0; x < im_ref->width_; ++x) {
            Color new_col = Color(0, 0, 0);
            for (int q = -radius; q <= radius; ++q) {
                new_col += Filter::NearestPix(x, y + q, prev_image) * vec_coefficients[radius + q];
            }
            Color new_col_true = Color(std::min(1.0, std::max(0.0, new_col.r)), std::min(1.0, std::max(0.0, new_col.g)),
                                       std::min(1.0, std::max(0.0, new_col.b)));
            im_ref->pix_matrix[y][x] = new_col;
            im_ref->vec_colors[y * im_ref->width_ + x] = new_col;
        }
    }
}

Glass::Glass(Image& im) : Filter(im) {
    const int c100 = 100;
    int diameter = std::max(1, static_cast<int>(im_ref->width_) / c100);

    Image prev_image = *im_ref;

    for (int64_t y = 0; y < im_ref->height_; ++y) {
        for (int64_t x = 0; x < im_ref->width_; ++x) {

            int delta_h = rand() % diameter;
            int delta_w = rand() % diameter;

            int new_x = std::min(static_cast<int>(prev_image.width_ - 1), std::max(static_cast<int>(x + delta_w), 0));
            int new_y = std::min(static_cast<int>(prev_image.height_ - 1), std::max(static_cast<int>(y + delta_h), 0));

            double const green_cf = 1.2;
            double const blue_cf = 1.5;
            Color new_col = Color(prev_image.pix_matrix[new_y][new_x].r,
                                  std::min(1.0, green_cf * prev_image.pix_matrix[new_y][new_x].g),
                                  std::min(1.0, blue_cf * prev_image.pix_matrix[new_y][new_x].b));
            im_ref->pix_matrix[y][x] = new_col;
            im_ref->vec_colors[y * im_ref->width_ + x] = new_col;
        }
    }
}
