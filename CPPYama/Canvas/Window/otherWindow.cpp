#include "otherWindow.h"
#include "../canvas.h"

void otherWindow::render(Canvas& c, int x, int y, int w, int h) const
{
    canvas::drawString(c, x, y, "This is different content!");
}