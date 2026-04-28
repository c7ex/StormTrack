#ifndef _LEGENDITEM_HPP
#define _LEGENDITEM_HPP

#include<vector>
#include<windows.h>

#include"Vec2d.hpp"
#include"RaiiWinApi.hpp"
#include"GraphContext.hpp"
#include"TransformCoordinates.hpp"

#include"ConfigUI.hpp"

class GraphContext;
class TransformCoordinates;

struct LegendProperties {
	bool active = true;
	COLORREF color;
	std::wstring caption;
};

class LegendItem {
private:
	std::vector<LegendProperties> items;

public:
    void Add(LegendProperties newItem);
	void Draw(HDC hdc, GraphContext& context);

    void DrawBackground(HDC hdc, const RECT& rect);
	void DrawFrame(HDC hdc, const RECT& rect);
    void DrawColorBox(HDC hdc, int x, int y, int size, const LegendProperties& item);
    void DrawCaptions(HDC hdc, int x, int y, const std::wstring& text);
	void HitCheckAndToggle(GraphContext& context, HWND hwnd);

	bool IsActive(int index) const;
	COLORREF GetColor(int index) const;
	size_t GetCountTrace() const;
};

#endif