#include"RelativeVec2d.hpp"

RelativeVec2d::RelativeVec2d() {
	start_values = Vec2d(1., 1.);
	current_values = start_values;
	scales = current_values / start_values;
}

RelativeVec2d::RelativeVec2d(Vec2d values) {
	start_values = values;
	current_values = start_values;
	scales = current_values / start_values;
}

void RelativeVec2d::SetStartValues(Vec2d values) {
	start_values = values;
	current_values = start_values;
	scales = current_values / start_values;
}

void RelativeVec2d::SetStartValues(double x, double y) {
	start_values.x = x;
	start_values.y = y;
	current_values = start_values;
	scales = current_values / start_values;
}

void RelativeVec2d::UpdateScales(Vec2d values) {
	scales = values;
	current_values = start_values * scales;
}

void RelativeVec2d::UpdateCurrentValues(Vec2d values) {
	current_values = values;
	scales = current_values / start_values;
}

void  RelativeVec2d::UpdateCurrentValues(double x, double y) {
	current_values.x = x;
	current_values.y = y;
	scales = current_values / start_values;
}

Vec2d RelativeVec2d::GetCurrentValues() const {
	return current_values;
}

Vec2d RelativeVec2d::GetScales() const {
	return scales;
}