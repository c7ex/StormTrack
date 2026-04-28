#include "Area2d.hpp"

Area2d Area2d::operator + (const Area2d& other) {
    Area2d area;
    area.left = left + other.left;
    area.top = top + other.top;
    area.right = right + other.right;
    area.bottom = bottom + other.bottom;
    return area;
}

Area2d Area2d::operator - (const Area2d& other) {
    Area2d area;
    area.left = left - other.left;
    area.top = top - other.top;
    area.right = right - other.right;
    area.bottom = bottom - other.bottom;
    return area;
}

void Area2d::operator += (const Area2d& other) {
    left += other.left;
    top += other.top;
    right += other.right;
    bottom += other.bottom;
}

void Area2d::operator -= (const Area2d& other) {
    left -= other.left;
    top -= other.top;
    right -= other.right;
    bottom -= other.bottom;
}

RECT Area2d::ToRect() {
    return RECT{ left, top, right, bottom };
}

Area2d::Area2d(int left_init, int top_init, int right_init, int bottom_init) {
    left = left_init;
    top = top_init;
    right = right_init;
    bottom = bottom_init;
}

Area2d::Area2d(const RECT& rect) {
    left = rect.left;
    top = rect.top;
    right = rect.right;
    bottom = rect.bottom;
}

Area2d::Area2d() : left(0), top(0), right(0), bottom(0) {}
