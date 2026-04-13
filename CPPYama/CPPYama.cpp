#include <iostream>
#include <string>
#include <vector>

#include "Windows.h"
#include "Console/console.h"
#include "Canvas/canvas.h"
#include "types.h"

// Methods
// -- Core loop
void updateState(AppState& state);
void render(const AppState& state);
void processInput(AppState& state);

// -- Navigation
void navigateButtons(AppState& state, int dx, int dy);

int main(int argc, char* argv[])
{
    AppState state;
    state.running = true;
    
    console::enableAnsi();
    console::enableRawInput();
    
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
    Canvas c = canvas::make(CANVAS_WIDTH, CANVAS_HEIGHT);
    canvas::drawBox(c, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);         // outer frame
    for (int i = 1; i < CANVAS_WIDTH - 2; i++)
    {
        canvas::drawChar(c, i, 2, '-');
    }
    canvas::drawString(c, 2, 1, "Yama TUI");  // title
    
    for (int i = 0; i < state.buttons.size(); i++)
        canvas::drawButton(c, state.buttons[i], i == state.selectedButton);
    
    canvas::flush(c);
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