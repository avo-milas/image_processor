#include "image.h"
#include "filters.h"
#include <iostream>
int main(int argc, char** argv) {
    //    const int64_t h = 800;
    //    const int64_t w = 600;

    //    Image image1(h, w);
    //
    //    for (int64_t y = 0; y < h; y++) {
    //        for (int64_t x = 0; x < w; x++) {
    //            image1.SetColor(Color(static_cast<double>(x) / static_cast<double>(w), 1.0f - static_cast<double>(x) /
    //            static_cast<double>(w), static_cast<double>(y) / static_cast<double>(h)), x, y);
    //        }
    //    }
    //
    //    image1.ExportImage("/home/alina/image_creat_new.bmp");
    //
    //    Image copy1(0, 0);  // добавить дефолт контруктор
    //
    //    copy1.Read("/home/alina/image.bmp");
    //
    //    copy1.ExportImage("/home/alina/image_r.bmp");

    //        Image lena;
    //
    //        lena.Read("/home/alina/lenna.bmp");
    //        Crop sh_lenna(lena, 999, 1999);
    //        lena.ExportImage("/home/alina/lena_wc_cr.bmp");

    if (argc <= 1) {
        std::cout << "Работа с изображениями формата BMP" << std::endl;
        std::cout << std::endl;
        std::cout << "Формат аргументов командной строки:" << std::endl;
        std::cout
            << "{имя программы} {путь к входному файлу} {путь к выходному файлу} [-{имя фильтра 1} [параметр фильтра "
               "1] [параметр фильтра 2] ...] [-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ..."
            << std::endl;
        std::cout << std::endl;
        std::cout << "Применение фильтров {название} (команда [параметры]):" << std::endl;
        std::cout << "Crop (-crop width height)" << std::endl;
        std::cout << "Grayscale (-gs)" << std::endl;
        std::cout << "Negative (-neg)" << std::endl;
        std::cout << "Sharpening (-sharp)" << std::endl;
        std::cout << "Edge Detection (-edge threshold)" << std::endl;
        std::cout << "Gaussian Blur (-blur sigma)" << std::endl;
        std::cout << "Glass (-glass)" << std::endl;
        return 0;
    }
    const char* path_in = reinterpret_cast<const char*>(argv[1]);
    const char* path_out = reinterpret_cast<const char*>(argv[2]);

    Image image;

    try {
        image.Read(path_in);
    } catch (std::invalid_argument& e) {
        std::cout << e.what();
        return 0;
    } catch (...) {
        std::cout << "Файл не может быть прочитан";
        return 0;
    }

    int64_t i = 3;
    while (i < argc) {
        std::string filter = static_cast<std::string>(argv[i]);
        if (filter == "-crop") {
            try {
                std::string arg1 = argv[++i];
                int width = std::stoi(arg1);
                std::string arg2 = argv[++i];
                int height = std::stoi(arg2);
                Crop cr_im(image, width, height);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else if (filter == "-gs") {
            try {
                Grayscale gs_im(image);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else if (filter == "-neg") {
            try {
                Negative neg_im(image);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else if (filter == "-sharp") {
            try {
                Sharpening sharp_im(image);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else if (filter == "-edge") {
            try {
                std::string arg1 = argv[++i];
                double threshold = std::stof(arg1);
                EdgeDetection ed_im(image, threshold);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else if (filter == "-blur") {
            try {
                std::string arg1 = argv[++i];
                double sigma = std::stof(arg1);
                GaussianBlur gb_im(image, sigma);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else if (filter == "-glass") {
            try {
                Glass gl(image);
            } catch (...) {
                std::cout << "Некорректные аргументы для фильтра";
                return 0;
            }
        } else {
            std::cout << "Введенные данные некорректны";
            return 0;
        }
        ++i;
    }

    try {
        image.ExportImage(path_out);
    } catch (...) {
        std::cout << "Файл не может быть экспортирован";
        return 0;
    }

    return 0;
}
