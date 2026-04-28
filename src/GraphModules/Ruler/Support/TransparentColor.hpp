#ifndef TRANSPARENTCOLOR_H
#define TRANSPARENTCOLOR_H

#include <windows.h>

class TransparentColor {
public:
    static COLORREF Mix(COLORREF bgColor, COLORREF textColor, double alpha);

private:
    static constexpr double MIN_RANGE = 0.0;
    static constexpr double MAX_RANGE = 1.0;
};

#endif