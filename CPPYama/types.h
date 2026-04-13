#pragma once

constexpr int CANVAS_WIDTH  = 40;                                        
constexpr int CANVAS_HEIGHT = 20;

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

struct Canvas
{
    int width;
    int height;
    std::vector<char> cells;
    std::vector<bool> highlighted;
};
