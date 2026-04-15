#include <iostream>
#include <string>
#include <vector>

#include "Windows.h"
#include "Console/console.h"
#include "Canvas/canvas.h"
#include "Canvas/Window/windows.h"
#include "types.h"
#include "Canvas/Window/otherWindow.h"

// Methods
// -- Core loop
void updateState(AppState& state);
void render(const AppState& state);
void processInput(AppState& state);

// -- Windows & Navigation
void selectWindow(AppState& state, int window);
std::vector<Button> getAllButtons(AppState& state);
void navigateButtons(AppState& state, int dx, int dy);

int main(int argc, char* argv[])
{
    AppState state;
    state.windows.push_back(std::make_unique<homeWindow>());
    state.windows.push_back(std::make_unique<otherWindow>());

    console::enableAnsi();
    console::enableRawInput();

    selectWindow(state, 0);
    state.running = true;
    
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
    if (state.lastActiveWindow != state.activeWindow)
    {
        selectWindow(state, state.activeWindow);
    }
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

    state.windows[state.activeWindow]->render(c);

    // Draw buttons late so they sit on top of content
    for (int i = 0; i < state.allButtons.size(); i++)
        canvas::drawButton(c, state.allButtons[i], i == state.selectedButton);
    
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
        case VK_RETURN:
            state.allButtons[state.selectedButton].action(state);
            break;
        }
        // handle other keys here, e.g. VK_LEFT, VK_RIGHT, 'A', etc.
    }

}

#pragma endregion

#pragma region Windows & Navigation

void selectWindow(AppState& state, int window)
{
    if (state.lastActiveWindow != -1)
    {
        state.windows[state.lastActiveWindow]->onHide();
    }

    state.lastActiveWindow = state.activeWindow;
    state.activeWindow = window;
    
    state.windows[window]->onShow(CONTENT_X, CONTENT_Y, CONTENT_W, CONTENT_H);

    // Upate button registry as global buttons + window buttons
    state.allButtons = getAllButtons(state);
    
    // If selected button was in the old window, bring it back to global scope
    if (state.selectedButton >= static_cast<int>(state.globalButtons.size()))
    {
        state.selectedButton = 0;
    }
}

std::vector<Button> getAllButtons(AppState& state)
{
    std::vector<Button> combined = state.globalButtons;
    if (state.activeWindow != -1)
    {
        combined.insert(combined.end(), state.windows[state.activeWindow]->windowButtons.begin(), state.windows[state.activeWindow]->windowButtons.end());
    }
    return combined;
}

void navigateButtons(AppState& state, int dx, int dy)
{
    if (state.allButtons.empty() || (dx == 0 && dy == 0))
    {
        return;
    }

    // Get nearest neighbour
    const Button& current = state.allButtons[state.selectedButton];
    int bestIndex = -1;
    int bestScore = INT_MAX;

    for (int i = 0; i < static_cast<int>(state.allButtons.size()); i++)
    {
        if (i == state.selectedButton) continue;

        const Button& candidate = state.allButtons[i];
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