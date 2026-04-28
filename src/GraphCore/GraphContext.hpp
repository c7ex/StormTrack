#ifndef GRAPHCONTEXT_HPP
#define GRAPHCONTEXT_HPP

#include"Windows.h"

#include"Vec2d.hpp"
#include"RelativeVec2d.hpp"
#include"TransformCoordinates.hpp"

class TransformCoordinates;

class GraphContext {
private:
    // from GraphState
    Size2d visible_area_;
    Position2d current_coord_;

    // from WindowState
    RECT          plot_area;
    Position2d    plot_reference_offset;
    RelativeVec2d plot_size;
    RelativeVec2d window_size;
    Position2d    mouse_position;

    // from TugboatState
    Position2d reference_position{ 0.0, 0.0 };

public:
    Size2d GetVisibleArea() const;


    void SetVisibleArea(Size2d new_visible_area);

    void SetVisibleArea(double width, double height);

    Position2d GetCurrentCoordinates() const;

    void UpdateCoordinates(const TransformCoordinates& coreEngine);

    /////

    RECT GetPlotArea() const;

    Position2d GetMousePosition() const;

    Size2d GetWindowSize();

    RelativeVec2d& GetWindowParam();

    RelativeVec2d& GetPlotParam();

    Size2d GetPlotSize() const;

    Position2d GetPlotReferenceOffset() const;

    void SetPlotArea(RECT rt);

    void SetPlotReferenceOffset(Position2d position);

    void UpdateMousePosition(const LPARAM& lParam);

    void UpdateMousePosition(double x, double y);

    /////

    Position2d GetReferencePosition() const;

    void SetReferencePosition(Position2d position);

    void SetOffsetReferencePosition(Offset2d offset);
};

#endif