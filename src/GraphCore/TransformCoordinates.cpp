#include "TransformCoordinates.hpp"

TransformCoordinates::TransformCoordinates(const GraphContext& ctx) : context(ctx) {}

Position2d TransformCoordinates::ConvertToPixelCoords(double world_x, double world_y) const {
    const Size2d visible_area = context.GetVisibleArea();
    const Position2d world_pos(world_x, world_y);
    const Offset2d world_offset = world_pos - context.GetReferencePosition();
    const Scale2d world_to_pixel = context.GetPlotSize() / visible_area;
    Offset2d pixel_offset = world_offset * world_to_pixel;
    pixel_offset.invert_y();
    Position2d pixel_coord = context.GetPlotReferenceOffset() + pixel_offset;
    return pixel_coord;
}

Position2d TransformCoordinates::ConvertToPixelCoords(const Position2d world_pos) const {
    const Size2d visible_area = context.GetVisibleArea();
    const Offset2d world_offset = world_pos - context.GetReferencePosition();
    const Scale2d world_to_pixel = context.GetPlotSize() / visible_area;
    Offset2d pixel_offset = world_offset * world_to_pixel;
    pixel_offset.invert_y();
    Position2d pixel_coord = context.GetPlotReferenceOffset() + pixel_offset;
    return pixel_coord;
}

Position2d TransformCoordinates::ConvertToWorldCoords(int pixel_x, int pixel_y) const {
    const Size2d visible_area = context.GetVisibleArea();
    const Position2d pixel_pos(pixel_x, pixel_y);
    const Offset2d pixel_offset = pixel_pos - context.GetPlotReferenceOffset();
    const Scale2d pixel_to_world = visible_area / context.GetPlotSize();
    Offset2d world_offset = pixel_offset * pixel_to_world;
    world_offset.invert_y();
    return (context.GetReferencePosition() + world_offset);
}