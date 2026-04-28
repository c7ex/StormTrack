#include "RenderCache.hpp"

void RenderCache::Reset() {
	caches.clear();
}

void RenderCache::extractPixelsX(GraphContext& context) {
	pixelsX = context.GetPlotParam().GetCurrentValues().x;
}

void RenderCache::calculateVisibleRangeX(GraphContext& context) {
	Size2d area = context.GetVisibleArea();
	Position2d pos = context.GetReferencePosition();
	double min_x = static_cast<double>(pos.x);
	double max_x = static_cast<double>(pos.x) + static_cast<double>(area.x);
	double range_x = max_x - min_x;
	vrX = { min_x, max_x, range_x };
}

void RenderCache::calculateRangeXInPixelX() {
	rangeXInPixelX = vrX.range_x / static_cast<double>(pixelsX);
}

double RenderCache::calculateCompressedScale(const LinearData& ldata) {
	double step = (ldata.getParameters()).step;
	double count_points_data_in_pixelX = rangeXInPixelX / step;
	return count_points_data_in_pixelX;
}

void RenderCache::calculateEffectiveIndices(const LinearData& ldata, const Position2d ref, const Size2d area)
{
	// extract
	LinearDataParameters data_parameters = ldata.getParameters();

	// boards
	auto left_index_board = 0;
	auto right_index_board = ldata.size() - 1;

	// calculated effective indexes with +1 correct
	auto left_index = static_cast<int>((vrX.min_value_x - data_parameters.offset) / data_parameters.step);
	auto right_index = static_cast<int>((vrX.max_value_x - data_parameters.offset) / data_parameters.step);

	left_index--;
	right_index++;

	// index protect
	if (left_index < left_index_board) left_index = left_index_board;
	if (right_index > right_index_board) right_index = right_index_board;

	// plot protect
	double xl = ldata[left_index].x;
	double xr = ldata[right_index].x;
	double left_plot_limit = ref.x;
	double right_plot_limit = ref.x + area.x;

	if (xl > right_plot_limit) {
		ei.min_index = 0;
		ei.max_index = 0;
		return;
	}

	if (xr < left_plot_limit) {
		ei.min_index = 0;
		ei.max_index = 0;
		return;
	}

	// update
	ei.min_index = left_index;
	ei.max_index = right_index;
}

void RenderCache::CreateScenarioCacheDirect(const TransformCoordinates& coreEngine, const LinearData& data) {
	TraceCache trace;
	trace.trace_id = trace_id;
	trace.is_active = true;
	trace.min_x = (std::numeric_limits<double>::max)();
	trace.max_x = -(std::numeric_limits<double>::max)();
	trace.min_y = (std::numeric_limits<double>::max)();
	trace.max_y = -(std::numeric_limits<double>::max)();
	trace.is_compressed = false;

	size_t start = static_cast<size_t>(ei.min_index);
	size_t end = static_cast<size_t>(ei.max_index);
	size_t count = end - start + 1;

	trace.points.reserve(count);

	for (size_t i = start; i <= end; ++i) {
		Vec2d p = coreEngine.ConvertToPixelCoords(data[i]);
		trace.points.push_back(p);

		trace.min_x = (std::min)(trace.min_x, p.x);
		trace.max_x = (std::max)(trace.max_x, p.x);
		trace.min_y = (std::min)(trace.min_y, p.y);
		trace.max_y = (std::max)(trace.max_y, p.y);
	}

	caches.push_back(std::move(trace));
}

void RenderCache::CreateScenarioCacheCompressed(const TransformCoordinates& coreEngine, const LinearData& data, double compressedScale) {	
	TraceCache trace;
	trace.trace_id = trace_id;
	trace.is_active = true;
	trace.min_x = (std::numeric_limits<double>::max)();
	trace.max_x = -(std::numeric_limits<double>::max)();
	trace.min_y = (std::numeric_limits<double>::max)();
	trace.max_y = -(std::numeric_limits<double>::max)();
	trace.is_compressed = true;

	double float_index = ei.min_index;

	while (float_index < ei.max_index) {
		int l_idx = static_cast<int>(float_index);
		float_index += compressedScale;
		int r_idx = static_cast<int>(float_index);
		if (r_idx > ei.max_index) r_idx = static_cast<int>(ei.max_index);

		double min_y = data[l_idx].y;
		double max_y = min_y;
		for (int i = l_idx + 1; i <= r_idx; ++i) {
			double y = data[i].y;
			if (y < min_y) min_y = y;
			else if (y > max_y) max_y = y;
		}

		int m_idx = (l_idx + r_idx) >> 1;
		double x = data[m_idx].x;

		Vec2d p1 = coreEngine.ConvertToPixelCoords(x, min_y);
		Vec2d p2 = coreEngine.ConvertToPixelCoords(x, max_y);

		trace.points.push_back(p1);
		trace.points.push_back(p2);

		trace.min_x = (std::min)(trace.min_x, (std::min)(p1.x, p2.x));
		trace.max_x = (std::max)(trace.max_x, (std::max)(p1.x, p2.x));
		trace.min_y = (std::min)(trace.min_y, (std::min)(p1.y, p2.y));
		trace.max_y = (std::max)(trace.max_y, (std::max)(p1.y, p2.y));
	}

	caches.push_back(std::move(trace));
}

void RenderCache::CachesManager(GraphContext& context, const TransformCoordinates& coreEngine, const LinearData& ldata)
{
	if (ldata.size() == 0) return;

	Position2d ref = context.GetReferencePosition();
	Size2d area = context.GetVisibleArea();
	calculateEffectiveIndices(ldata, ref, area);

	if (ei.min_index >= ei.max_index) return;

	double compressedScale = calculateCompressedScale(ldata);

	constexpr double visible_points_in_pixel = 2.;
	if (compressedScale < visible_points_in_pixel) {
		CreateScenarioCacheDirect(coreEngine, ldata);
	}
	else {
		CreateScenarioCacheCompressed(coreEngine, ldata, compressedScale);
	}
}

const std::vector<TraceCache>& RenderCache::GetCaches() const { return caches; }

void RenderCache::GenerateRenderCacheData(
	GraphContext& context,
	const TransformCoordinates& coreEngine,
	const LegendItem& li,
	const std::vector<LinearData>& data_pull) {

	// clear caches
	Reset();

	// get X-pixels of current plot
	extractPixelsX(context);

	// get current X-range coordinates in plot (min/max)
	calculateVisibleRangeX(context);

	// get rangeX in PixelX
	calculateRangeXInPixelX();

	for (int i = 0; i < data_pull.size(); ++i) {
		bool active_data = li.IsActive(i);
		trace_id = i;
		if (active_data) {
			CachesManager(context, coreEngine, data_pull[i]);
		}
	}
}