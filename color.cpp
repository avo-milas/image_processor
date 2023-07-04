//
// Created by avo_m on 25.03.2023.
//

#include "color.h"

Color::Color() {
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(double r, double g, double b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::~Color() {
}

Color& Color::operator=(const Color& c2) {
    r = c2.r;
    g = c2.g;
    b = c2.b;
    return *this;
}
Color Color::operator*(double k) const {
    return Color(r * k, g * k, b * k);
}

Color Color::operator+(const Color& col2) const {
    return Color(r + col2.r, g + col2.g, b + col2.b);
}
Color& Color::operator+=(const Color& col2) {
    r += col2.r;
    g += col2.g;
    b += col2.b;
    return *this;
}
