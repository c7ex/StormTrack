#include"TugboatState.hpp"

void TugboatState::StartDragging(GraphContext& context, const LPARAM lParam) {
    if (!is_active) {
        context.UpdateMousePosition(lParam);
        last_reference_position = context.GetReferencePosition();
        hold_position = context.GetMousePosition();
        is_active = true;
    }
}

void TugboatState::UpdateDrag(GraphContext& context, const LPARAM lParam) {
    if (is_active) {
        context.UpdateMousePosition(lParam);
        current_position = context.GetMousePosition();
        const Size2d visible_area = context.GetVisibleArea();
        const Scale2d pixel_scale = visible_area / context.GetPlotSize();
        Offset2d drag_offset = hold_position - current_position;
        Offset2d world_offset = pixel_scale * drag_offset;
        world_offset.invert_y();
        auto new_reference_position = last_reference_position + world_offset;
        context.SetReferencePosition(new_reference_position);
    }
}

void TugboatState::StopDragging(GraphContext& context, const LPARAM lParam) {
    UpdateDrag(context, lParam);
    is_active = false;
}

bool TugboatState::IsActive() const {
    return is_active;
}