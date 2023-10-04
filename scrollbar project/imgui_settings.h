#include "imgui.h"

namespace c {

	inline ImVec4 accent_color = ImColor(230, 220, 105, 255);
	inline ImVec4 accent_color_low = ImColor(230, 220, 105, 100);

	namespace bg
	{
		inline ImVec4 background = ImColor(13, 12, 18, 230);
		inline ImVec4 Platinum = ImColor(27, 25, 31, 210);
		inline ImVec2 size = ImVec2(822, 500);
		inline float rounding = 6.f;
	}

	namespace child
	{
		inline ImVec4 background = ImColor(46, 41, 55, 100);
		inline ImVec4 border = ImColor(46, 41, 55, 255);
		inline float rounding = 6.f;
	}

	namespace tabs
	{
		inline ImVec4 border = ImColor(4, 8, 19, 255);
		inline ImVec4 border_invisible = ImColor(0, 4, 15, 0);
		inline float rounding = 30.f;
	}

	namespace checkbox
	{
		inline ImVec4 gradient_color0 = ImColor(46, 41, 55, 255);
		inline ImVec4 gradient_color1 = ImColor(33, 30, 38, 255);
		inline ImVec4 background_invisible = ImColor(39, 39, 50, 0);
		inline ImVec4 mark_active = ImColor(0, 0, 0, 255);
		inline ImVec4 mark_inactive = ImColor(0, 0, 0, 0);
		inline ImVec4 border = ImColor(39, 39, 50, 255);
		inline float rounding = 2.f;
	}

	namespace slider
	{
		inline ImVec4 background = ImColor(46, 41, 55, 255);
		inline ImVec4 circle = ImColor(36, 31, 45, 255);
		inline float rounding = 30.f;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(33, 30, 38, 255);
		inline ImVec4 border = ImColor(39, 39, 50, 255);
		inline float rounding = 2.f;
	}

	namespace input
	{
		inline ImVec4 background = ImColor(33, 30, 38, 255);
		inline ImVec4 border = ImColor(39, 39, 50, 255);
		inline float rounding = 2.f;
	}

	namespace combo
	{
		inline ImVec4 background = ImColor(33, 30, 38, 255);
		inline ImVec4 border = ImColor(39, 39, 50, 255);
		inline float rounding = 2.f;
	}

	namespace button
	{
		inline ImVec4 gradient_color0 = ImColor(46, 41, 55, 255);
		inline ImVec4 gradient_color1 = ImColor(33, 30, 38, 255);
		inline ImVec4 background_invisible = ImColor(39, 39, 50, 0);
		inline ImVec4 border = ImColor(39, 39, 50, 255);
		inline float rounding = 2.f;
	}

	namespace keybind
	{
		inline ImVec4 background = ImColor(46, 41, 55, 255);
		inline ImVec4 border = ImColor(39, 39, 50, 255);
		inline float rounding = 2.f;
	}

	namespace scrollbar
	{
		inline ImVec4 background = ImColor(230, 220, 105, 255);
		inline ImVec4 border = ImColor(30, 30, 38, 255);
		inline float rounding = 30.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(98, 97, 109, 255);
		inline ImVec4 text = ImColor(68, 67, 79, 255);
	}

}