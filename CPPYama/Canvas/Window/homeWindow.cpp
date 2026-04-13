#include "homeWindow.h"
#include "../canvas.h"

void homeWindow::render(Canvas& c, int x, int y, int w, int h) const
{
    canvas::drawString(c, x, y, "Welcome home");
}
