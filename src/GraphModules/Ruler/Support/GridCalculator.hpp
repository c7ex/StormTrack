#ifndef GRIDCALCULATOR_H
#define GRIDCALCULATOR_H

#include "Vec2d.hpp"
#include "AxesConfig.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

class GridCalculator {
public:
    static Size2d CalculateCurrentStepGrid(Size2d span);
    static Size2d CalculateLowStepGrid(Size2d current_step);
    static Size2d CalculateAlphaColorGrid(Size2d span, Size2d step);

private:
    static constexpr double BASE = 10.0;
    static constexpr double SUBDIVISION = 10.0;

    static constexpr double OPTIMAL_LINES = 14.0;
    static constexpr double ALPHA_FACTOR = 7.0;
    static constexpr double MIN_ALPHA = 0.0;
    static constexpr double MAX_ALPHA = 1.0;

    static constexpr double LINES_THRESHOLD = 10.0;
};

#endif