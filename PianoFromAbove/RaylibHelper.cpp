#include <cmath>
#include "RaylibHelper.h"

rl::Color int_to_color(unsigned int rgb_int)
{
    return rl::Color({
        (unsigned char)(rgb_int >> 16 & 0xff),
        (unsigned char)(rgb_int >> 8 & 0xff),
        (unsigned char)(rgb_int & 0xff),
        (unsigned char)(255 - (rgb_int >> 24 & 0xff))
    });
}

rl::Rectangle align_rectangle(rl::Rectangle rect) {
    float left = std::round(rect.x);
    float top = std::round(rect.y);
    float right = std::round(rect.x + rect.width);
    float bottom = std::round(rect.y + rect.height);
    
    return {
        left, top, right - left, bottom - top
    };
}

void draw_triangle_gradient(rl::Vector2 p1, rl::Vector2 p2, rl::Vector2 p3, rl::Color c1, rl::Color c2, rl::Color c3)
{
    rl::rlBegin(RL_TRIANGLES);
        rl::rlColor4ub(c1.r, c1.g, c1.b, c1.a);
        rl::rlVertex2f(p1.x, p1.y);

        rl::rlColor4ub(c2.r, c2.g, c2.b, c2.a);
        rl::rlVertex2f(p2.x, p2.y);

        rl::rlColor4ub(c3.r, c3.g, c3.b, c3.a);
        rl::rlVertex2f(p3.x, p3.y);
    rl::rlEnd();
}
