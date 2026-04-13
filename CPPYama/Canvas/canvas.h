#pragma once
#include <string>
#include <vector>
#include "../types.h"

namespace canvas
{
    Canvas make(int w, int h);
    void drawChar(Canvas& c, int x, int y, char ch);
    void drawString(Canvas& c, int x, int y, const std::string& s);
    void drawBox(Canvas& c, int x, int y, int w, int h);
    void drawButton(Canvas& c, const Button& btn, bool selected);
    void flush(const Canvas& c);
};

