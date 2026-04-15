#include "otherWindow.h"
#include "../canvas.h"

void otherWindow::render(Canvas& c) const
{
    canvas::drawString(c, posX, posY, "This is different content!");
}

void otherWindow::onShow(int x, int y, int w, int h)
{
    baseWindow::onShow(x, y, w, h);
}

void otherWindow::onHide()
{
    
}

