#pragma once
#include <vector>

struct Canvas;
struct Button;

class baseWindow
{
public:
    std::vector<Button> windowButtons;
    virtual ~baseWindow() = default;
    virtual void render(Canvas& c) const = 0;
    virtual void onShow(int x, int y, int w, int h);
    virtual void onHide();

protected:
    int posX = 0;
    int posY = 0;
    int width = 0;
    int height = 0;
};
