#pragma once
struct Canvas;

class baseWindow
{
public:
    virtual ~baseWindow() = default;
    virtual void render(Canvas& c, int x, int y, int w, int h) const = 0;
};
