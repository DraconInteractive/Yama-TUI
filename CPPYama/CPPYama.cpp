#include <iostream>
#include <string>
#include <vector>

#include "Windows.h"

// Struct Definitions
struct Canvas
{
    int width;
    int height;
    std::vector<char> cells;
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
        {"Home",    2, 4},
        {"Quit",    2, 5},
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

// Const Definitions
constexpr int CANVAS_WIDTH  = 40;
constexpr int CANVAS_HEIGHT = 20;

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
    drawString(c, 1, 1, "My Terminal App");  // title
    for (int i = 0; i < buttons.size(); i++)
        drawButton(c, buttons[i], i == state.selectedButton);
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
            
        }
        if (key == VK_ESCAPE)
        {
            
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
    // TODO setup cells
}

void drawChar(Canvas& c, int x, int y, char ch)
{
    if (x < 0 || y < 0 || x > c.width || y > c.height)
    {
        return;
    }
    c.cells[y * c.height + x] = ch;
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
    // TODO add this
}

void drawButton(Canvas& c, const Button& btn, bool selected)
{
    // TODO add this
}

void flushCanvas(const Canvas& c)
{
    std::cout << "\x1b[2J"; // clear screen
    std::cout << "\x1b[H";  // move cursor to top-left
    for (int y = 0; y < c.height; y++)
    {
        for (int x = 0; x < c.width; x++)
        {
            std::cout << c.cells[y * c.height + x];
        }
        std::cout << "\n";
    }
    std::cout.flush();
}

#pragma endregion 
