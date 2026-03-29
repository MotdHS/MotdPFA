#pragma once

namespace rl
{
#undef DrawText
#undef DrawTextEx
#include "raylib.h"
#include "rlgl.h"
	rl::Color IntToColor(unsigned int rgb_int);
	rl::Rectangle AlignRectangle(rl::Rectangle rect);
	void SetLimitFPS(bool bLimitFPS);
	void DrawTriangleGradient(rl::Vector2 p1, rl::Vector2 p2, rl::Vector2 p3, rl::Color c1, rl::Color c2, rl::Color c3);
	void DrawRectangleRecGradientH(rl::Rectangle rect, rl::Color left, rl::Color right);
	void DrawRectangleRecGradientV(rl::Rectangle rect, rl::Color top, rl::Color bottom);
}

namespace rlc  // Whenever I'm lazy to rewrite the functions
{
	void DrawSkew(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
		unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4);
	void DrawSkew(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int color);

	void DrawRect(float x, float y, float cx, float cy, unsigned int color);
	void DrawRect(float x, float y, float cx, float cy,
		unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4);

	int GetBufferWidth();
	int GetBufferHeight();
}
