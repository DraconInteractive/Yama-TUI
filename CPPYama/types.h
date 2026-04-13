#pragma once
#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "Canvas/Window/baseWindow.h"

// Total canvas
constexpr int CANVAS_WIDTH  = 40;                                        
constexpr int CANVAS_HEIGHT = 20;

// Window Bounds
constexpr int CONTENT_X = 2;
constexpr int CONTENT_Y = 3;
constexpr int CONTENT_W = CANVAS_WIDTH - 3;
constexpr int CONTENT_H = CANVAS_HEIGHT - 3;

struct AppState;
struct Button;
struct Canvas;
struct Window;

struct Button
{
    std::string label;
    int x;
    int y;
    std::function<void(AppState&)> action;
};

struct AppState
{
    bool running = true;
    int activeWindow = 0;
    std::vector<std::unique_ptr<baseWindow>> windows;
    std::vector<Button> buttons = {
        {"Home",    CANVAS_WIDTH - 20, 1, [](AppState& s) { s.activeWindow = 0; }},
        { "Other",  CANVAS_WIDTH - 11, 1, [](AppState& s) { s.activeWindow = 1; }},
        {"Quit",    CANVAS_WIDTH - 10, CANVAS_HEIGHT - 2, [](AppState& s){ s.running = false; }}, // 10 = [ Quit ] (8) + Buffer (2)
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

struct Window
{
    std::string name;
};
