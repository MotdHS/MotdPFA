#include <cmath>
#include "RaylibHelper.h"
#include <algorithm>

rl::Color rl::IntToColor(unsigned int rgb_int)
{
    return rl::Color({
        (unsigned char)(rgb_int >> 16 & 0xff),
        (unsigned char)(rgb_int >> 8 & 0xff),
        (unsigned char)(rgb_int & 0xff),
        (unsigned char)(255 - (rgb_int >> 24 & 0xff))
    });
}

rl::Rectangle rl::AlignRectangle(rl::Rectangle rect) {
    float left = std::round(rect.x);
    float top = std::round(rect.y);
    float right = std::round(rect.x + rect.width);
    float bottom = std::round(rect.y + rect.height);
    
    return {
        left, top, right - left, bottom - top
    };
}

void rl::DrawTriangleGradient(rl::Vector2 p1, rl::Vector2 p2, rl::Vector2 p3, rl::Color c1, rl::Color c2, rl::Color c3)
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

void rl::DrawRectangleRecGradientH(rl::Rectangle rect, rl::Color left, rl::Color right)
{
    rl::DrawRectangleGradientEx(rect, left, left, right, right);
}

void rl::DrawRectangleRecGradientV(rl::Rectangle rect, rl::Color top, rl::Color bottom)
{
    rl::DrawRectangleGradientEx(rect, top, bottom, bottom, top);
}

rl::Color rl::MultiplyColor(rl::Color color, float factor)
{
    return {
        static_cast<unsigned char>(std::clamp(std::round(color.r * factor), 0.0f, 255.0f)),
        static_cast<unsigned char>(std::clamp(std::round(color.g * factor), 0.0f, 255.0f)),
        static_cast<unsigned char>(std::clamp(std::round(color.b * factor), 0.0f, 255.0f)),
        color.a
    };
}

rl::Color rl::MultiplyAlpha(rl::Color color, float factor)
{
    return {
        color.r,
        color.g,
        color.b,
        static_cast<unsigned char>(std::clamp(std::round(color.a * factor), 0.0f, 255.0f))
    };
}

void rl::SetLimitFPS(bool bLimitFPS)
{
    if (bLimitFPS)
        rl::SetWindowState(rl::ConfigFlags::FLAG_VSYNC_HINT);
    else
        rl::ClearWindowState(rl::ConfigFlags::FLAG_VSYNC_HINT);
}

void rlc::DrawSkew(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int color)
{
    rlc::DrawSkew(x1, y1, x2, y2, x3, y3, x4, y4, color, color, color, color);
}

void rlc::DrawSkew(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
    unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4)
{
    rl::DrawTriangleGradient({ x1, y1 }, { x3, y3 }, { x2, y2 }, rl::IntToColor(c1), rl::IntToColor(c3), rl::IntToColor(c2));
    rl::DrawTriangleGradient({ x1, y1 }, { x4, y4 }, { x3, y3 }, rl::IntToColor(c1), rl::IntToColor(c4), rl::IntToColor(c3));
}

void rlc::DrawRect(float x, float y, float cx, float cy, unsigned int color)
{
    rlc::DrawRect(x, y, cx, cy, color, color, color, color);
}

void rlc::DrawRect(float x, float y, float cx, float cy,
    unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4)
{
    rl::DrawRectangleGradientEx(rl::AlignRectangle({ x, y, cx, cy }),
        rl::IntToColor(c1),
        rl::IntToColor(c4),
        rl::IntToColor(c3),
        rl::IntToColor(c2)
    );
}

int rlc::GetBufferWidth() {
    return rl::GetRenderWidth();
}

int rlc::GetBufferHeight() {
    return rl::GetRenderHeight();
}
