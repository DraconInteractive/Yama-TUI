#include "canvas.h"

#include <iostream>

Canvas canvas::make(int w, int h)
{
    Canvas newCanvas;
    newCanvas.width = w;
    newCanvas.height = h;
    newCanvas.cells.assign(w * h, ' ');
    newCanvas.highlighted.assign(w * h, false);
    return newCanvas;
}

void canvas::drawChar(Canvas& c, int x, int y, char ch)
{
    if (x < 0 || y < 0 || x >= c.width || y >= c.height)
    {
        return;
    }
    c.cells[y * c.width + x] = ch;
}

void canvas::drawString(Canvas& c, int x, int y, const std::string& s)
{
    for (int i = 0; i < s.length(); i++)
    {
        drawChar(c, x + i, y, s[i]);
    }
}

void canvas::drawBox(Canvas& c, int x, int y, int w, int h)
{
    // Corners
    drawChar(c, x,         y,         '+');                              
    drawChar(c, x + w - 1, y,         '+');           
    drawChar(c, x,         y + h - 1, '+');
    drawChar(c, x + w - 1, y + h - 1, '+');

    // Top and bottom edges
    for (int i = 1; i < w - 1; i++)
    {
        drawChar(c, x + i, y,         '-');
        drawChar(c, x + i, y + h - 1, '-');
    }

    // Left and right edges
    for (int i = 1; i < h - 1; i++)
    {
        drawChar(c, x,         y + i, '|');
        drawChar(c, x + w - 1, y + i, '|');
    }
}

void canvas::drawButton(Canvas& c, const Button& btn, bool selected)
{
    const std::string text = "[ " + btn.label + " ]";
    drawString(c, btn.x, btn.y, text);

    if (selected)
    {
        for (int i = 0; i < static_cast<int>(text.length()); i++)
        {
            if (btn.x + i < c.width)
                c.highlighted[btn.y * c.width + btn.x + i] = true;
        }
    }
}

void canvas::flush(const Canvas& c)
{
    std::cout << "\x1b[?25l"; // hide cursor
    // std::cout << "\x1b[2J"; // clear screen
    std::cout << "\x1b[H";  // move cursor to top-left
    bool doHighlight = false;
    
    for (int y = 0; y < c.height; y++)
    {
        for (int x = 0; x < c.width; x++)
        {
            bool cellHighlight = c.highlighted[y * c.width + x];
            if (cellHighlight && !doHighlight)
            {
                doHighlight = true;
                std::cout << "\x1b[7m";
            }
            else if (!cellHighlight && doHighlight)
            {
                doHighlight = false;
                std::cout << "\x1b[0m";
            }

            std::cout << c.cells[y * c.width + x];
        }
        std::cout << "\n";
    }
    std::cout << "\x1b[?25h"; // show cursor
    std::cout.flush();
}