#ifndef AXES_DESCRIPTION_HPP
#define AXES_DESCRIPTION_HPP

#include"vector"

#include "Area2d.hpp"

#include "AxesConfig.hpp"
#include "TransparentColor.hpp"
#include "TextFormatter.hpp"
#include "GridCalculator.hpp"

enum class GridDrawSelection {
    Horizontal,
    Vertical
};

struct GridPassContent {
    double alpha;
    Size2d step;
    COLORREF color;
    GridDrawSelection selection;
};

struct CoreContent {
    Position2d ref;
    RECT plot;
    Size2d area;
};

#endif