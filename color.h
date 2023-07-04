//
// Created by avo_m on 25.03.2023.
//

#ifndef CPP_HSE_COLOR_H
#define CPP_HSE_COLOR_H

#endif  // CPP_HSE_COLOR_H

struct Color {
    double r, g, b;

    Color();
    Color(double r, double g, double b);
    ~Color();
    Color& operator=(const Color& color);
    Color operator*(double k) const;
    Color operator+(const Color& col2) const;
    Color& operator+=(const Color& col2);
};
