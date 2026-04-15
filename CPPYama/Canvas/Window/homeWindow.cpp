#include "homeWindow.h"
#include "../canvas.h"

void homeWindow::render(Canvas& c) const
{
    canvas::drawString(c, posX, posY, currentDisplay); // TODO make drawString overload that takes rect and supports wrapping
}

void homeWindow::onShow(int x, int y, int w, int h)
{
    baseWindow::onShow(x, y, w, h);
    currentDisplay = "Welcome home";
    windowButtons = {
        // { "^", x + (w / 2) + 3, y, [](AppState& s) { }},
        // { "v", x + (w / 2) + 3, y + h - 2, [](AppState& s) { }}, 
        { "Test", x + (w / 2), y + (h / 2), [this](AppState& s) { currentDisplay = "Test has been pressed!"; }}, 
    };
}

void homeWindow::onHide()
{
    
}


