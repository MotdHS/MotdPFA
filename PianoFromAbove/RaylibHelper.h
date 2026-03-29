#pragma once

namespace rl
{
#undef DrawText
#undef DrawTextEx
#include "raylib.h"
#include "rlgl.h"
}

rl::Color int_to_color(unsigned int rgb_int);
rl::Rectangle align_rectangle(rl::Rectangle rect);
void draw_triangle_gradient(rl::Vector2 p1, rl::Vector2 p2, rl::Vector2 p3, rl::Color c1, rl::Color c2, rl::Color c3);
