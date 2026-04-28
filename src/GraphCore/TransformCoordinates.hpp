#ifndef TRANSFORM_COORDINATES_HPP
#define TRANSFORM_COORDINATES_HPP

#include"Vec2d.hpp"
#include"GraphContext.hpp"

class GraphContext;

class TransformCoordinates {
private:
	const GraphContext& context;
	
public:
	TransformCoordinates(const GraphContext& ctx);

	Position2d ConvertToPixelCoords(double world_x, double world_y) const;
	Position2d ConvertToPixelCoords(Position2d world_pos) const;
	Position2d ConvertToWorldCoords(int pixel_x, int pixel_y) const;

public:

};

#endif