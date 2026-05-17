#ifndef RAII_WINAPI_HPP
#define RAII_WINAPI_HPP

#include"windows.h"

// RAII-WINAPI classes
// -> HPEN
// -> HBRUSH
// -> HFONT
// -> HRGN
// -> HBITMAP
namespace rwa {
	class PEN {
		HDC context = nullptr;
		HPEN new_object = nullptr;
		HPEN old_object = nullptr;

	public:
		PEN(HDC hdc, int style, int width, COLORREF color) {
			context = hdc;
			new_object = CreatePen(style, width, color);
			if (new_object) {
				old_object = (HPEN)SelectObject(hdc, new_object);
			}
		}

		~PEN() noexcept {
			if (context) {
				if (new_object) {
					SelectObject(context, old_object);
					DeleteObject(new_object);
				}
			}
		}

		PEN(const PEN&) = delete;
		PEN& operator=(const PEN&) = delete;
	};

	class BRUSH {
		HDC context = nullptr;
		HBRUSH new_object = nullptr;
		HBRUSH old_object = nullptr;

	public:
		HBRUSH get() {
			return new_object;
		}

		BRUSH(HDC hdc, COLORREF color) {
			context = hdc;
			new_object = CreateSolidBrush(color);
			if (new_object) {
				old_object = (HBRUSH)SelectObject(context, new_object);
			}
		}

		~BRUSH() noexcept {
			if (context) {
				if (new_object) {
					SelectObject(context, old_object);
					DeleteObject(new_object);
				}
			}
		}

		BRUSH(const BRUSH&) = delete;
		BRUSH& operator=(const BRUSH&) = delete;
	};

	class FONT {
		HDC context = nullptr;
		HFONT old_object = nullptr;
		HFONT created_font = nullptr; //

	public:
		FONT(HDC hdc, int font) {
			context = hdc;
			HFONT new_object = (HFONT)GetStockObject(font);
			if (new_object) {
				old_object = (HFONT)SelectObject(hdc, new_object);
				SetBkMode(hdc, TRANSPARENT);
			}
		}

		FONT(HDC hdc, LOGFONT logfont) {
			context = hdc;
			created_font = CreateFontW(logfont.lfHeight, logfont.lfWidth, logfont.lfEscapement, logfont.lfOrientation,
				logfont.lfWeight, logfont.lfItalic, logfont.lfUnderline, logfont.lfStrikeOut,
				logfont.lfCharSet, logfont.lfOutPrecision, logfont.lfClipPrecision,
				logfont.lfQuality, logfont.lfPitchAndFamily, logfont.lfFaceName);
			if (created_font) {
				old_object = (HFONT)SelectObject(hdc, created_font);
				SetBkMode(hdc, TRANSPARENT);
			}
			else {
				old_object = nullptr;
			}
		}

		FONT(HDC hdc, int height, int width, int escapement, int orientation, //
			int weight, bool italic, bool underline, bool strikeout,
			DWORD charset, DWORD out_precision, DWORD clip_precision,
			DWORD quality, DWORD pitch_family, const wchar_t* face_name)
		{
			context = hdc;
			created_font = CreateFontW(height, width, escapement, orientation,
				weight, italic, underline, strikeout,
				charset, out_precision, clip_precision,
				quality, pitch_family, face_name);
			if (created_font) {
				old_object = (HFONT)SelectObject(hdc, created_font);
				SetBkMode(hdc, TRANSPARENT);
			}
			else {
				old_object = nullptr;
			}
		}

		~FONT() {
			if (context && old_object) {
				SelectObject(context, old_object);
			}
			if (created_font) {
				DeleteObject(created_font);
			}
		}

		FONT(const FONT&) = delete;
		FONT& operator=(const FONT&) = delete;
	};

	class RGN {
		HDC context = nullptr;
		HRGN object = nullptr;

	public:
		RGN(HDC hdc, RECT* rect) {
			context = hdc;
			object = CreateRectRgnIndirect(rect);
			SelectClipRgn(hdc, object);
		}

		~RGN() noexcept {
			Free();
		}

		void Free() {
			if (context) {
				if (object) {
					SelectClipRgn(context, NULL);
					DeleteObject(object);
					object = nullptr;
					context = nullptr;
				}
			}
		}

		RGN(const RGN&) = delete;
		RGN& operator=(const RGN&) = delete;
	};
}

#endif