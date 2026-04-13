#include <iostream>
#include <string>
#include <vector>

#include "Windows.h"

// Const Definitions
constexpr int CANVAS_WIDTH  = 40;
constexpr int CANVAS_HEIGHT = 20;

// Struct Definitions
struct Canvas
{
    int width;
    int height;
    std::vector<char> cells;
    std::vector<bool> highlighted;
};

struct Button
{
    std::string label;
    int x;
    int y;
};

struct AppState
{
    bool running = true;
    int window = 0;
    std::vector<std::string> windows = {"Home", "NotHome"};
    std::vector<Button> buttons = {
        {"Home",    CANVAS_WIDTH - 20, 1},
        { "Other",  CANVAS_WIDTH - 11, 1},
        {"Quit",    CANVAS_WIDTH - 10, CANVAS_HEIGHT - 2}, // 10 = [ Quit ] (8) + Buffer (2)
    };
    int selectedButton = 0;
};

// Methods
// -- Console setup
void enableAnsi();
void enableRawInput();

// -- Core loop
void updateState(AppState& state);
void render(const AppState& state);
void processInput(AppState& state);

// -- Rendering
Canvas makeCanvas(int w, int h);
void drawChar(Canvas& c, int x, int y, char ch);
void drawString(Canvas& c, int x, int y, const std::string& s);
void drawBox(Canvas& c, int x, int y, int w, int h);
void drawButton(Canvas& c, const Button& btn, bool selected);
void flushCanvas(const Canvas& c);

// -- Navigation
void navigateButtons(AppState& state, int dx, int dy);

int main(int argc, char* argv[])
{
    AppState state;
    state.running = true;
    
    enableAnsi();
    enableRawInput();
    
    while (state.running)
    {
        updateState(state);
        render(state);
        processInput(state);
    }

    std::cout << "\x1b[?25h";
    return 0;
}

#pragma region Core

void updateState(AppState& state)
{
    
}

void render(const AppState& state)
{
    Canvas c = makeCanvas(CANVAS_WIDTH, CANVAS_HEIGHT);
    drawBox(c, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);         // outer frame
    for (int i = 1; i < CANVAS_WIDTH - 2; i++)
    {
        drawChar(c, i, 2, '-');
    }
    drawString(c, 2, 1, "Yama TUI");  // title
    
    for (int i = 0; i < state.buttons.size(); i++)
        drawButton(c, state.buttons[i], i == state.selectedButton);
    
    flushCanvas(c);
}

void processInput(AppState& state)
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);                         
    INPUT_RECORD record;
    DWORD count;

    // Peek first so we don't block if no input is ready
    DWORD available;
    GetNumberOfConsoleInputEvents(hIn, &available);
    if (available == 0) return;

    ReadConsoleInput(hIn, &record, 1, &count);

    if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
    {
        WORD key = record.Event.KeyEvent.wVirtualKeyCode;
        switch (key)
        {
        case VK_ESCAPE:
            state.running = false;
            break;
        case VK_UP:
            navigateButtons(state, 0, -1);
            break;
        case VK_DOWN:
            navigateButtons(state, 0, 1);
            break;
        case VK_LEFT:
            navigateButtons(state, -1, 0);
            break;
        case VK_RIGHT:
            navigateButtons(state, 1, 0);
            break;
        }
        // handle other keys here, e.g. VK_LEFT, VK_RIGHT, 'A', etc.
    }

}

#pragma endregion

#pragma region Console Setup

void enableRawInput()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hIn, &mode);
    mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hIn, mode);
}

void enableAnsi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}

#pragma endregion

#pragma region Rendering

Canvas makeCanvas(int w, int h)
{
    Canvas newCanvas;
    newCanvas.width = w;
    newCanvas.height = h;
    newCanvas.cells.assign(w * h, ' ');
    newCanvas.highlighted.assign(w * h, false);
    return newCanvas;
}

void drawChar(Canvas& c, int x, int y, char ch)
{
    if (x < 0 || y < 0 || x >= c.width || y >= c.height)
    {
        return;
    }
    c.cells[y * c.width + x] = ch;
}

void drawString(Canvas& c, int x, int y, const std::string& s)
{
    for (int i = 0; i < s.length(); i++)
    {
        drawChar(c, x + i, y, s[i]);
    }
}

void drawBox(Canvas& c, int x, int y, int w, int h)
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

void drawButton(Canvas& c, const Button& btn, bool selected)
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

void flushCanvas(const Canvas& c)
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

#pragma endregion 

#pragma region Navigation

void navigateButtons(AppState& state, int dx, int dy)
{
    if (state.buttons.empty() || (dx == 0 && dy == 0))
    {
        return;
    }

    // Get nearest neighbour
    const Button& current = state.buttons[state.selectedButton];
    int bestIndex = -1;
    int bestScore = INT_MAX;

    for (int i = 0; i < static_cast<int>(state.buttons.size()); i++)
    {
        if (i == state.selectedButton) continue;

        const Button& candidate = state.buttons[i];
        int relX = candidate.x - current.x;
        int relY = candidate.y - current.y;

        // Validate direction
        if (dy != 0)
        {
            if (dy > 0 && relY <= 0) continue;
            if (dy < 0 && relY >= 0) continue;
        }
        if (dx != 0)
        {
            if (dx > 0 && relX <= 0) continue;
            if (dx < 0 && relX >= 0) continue;
        }

        // Disincentivised diagonals
        /*
        int score = (dy != 0)
              ? abs(relY) * 100 + abs(relX)
              : abs(relX) * 100 + abs(relY);
              */
        int score = abs(relY) + abs(relX);
        if (score < bestScore)
        {
            bestScore = score;
            bestIndex = i;
        }
    }

    if (bestIndex != -1)
    {
        state.selectedButton = bestIndex;
    }
}

#pragma endregion