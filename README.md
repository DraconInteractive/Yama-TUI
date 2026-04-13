# Yama TUI

A TUI (Text User Interface) application written in C++ for Windows, built from scratch using the Windows Console API and ANSI escape codes.

## Overview

Yama TUI implements a lightweight TUI framework with a canvas-based rendering system, keyboard navigation, and a windowed content area. The application renders entirely within the terminal using a cell buffer that is flushed to stdout each frame.

## Features

- Canvas-based renderer — all drawing is staged to a cell buffer before output, preventing flicker
- Spatial button navigation using arrow keys (nearest-neighbour algorithm)
- Highlighted selection state using ANSI reverse video
- Window system — switchable content areas driven by button actions
- Raw keyboard input via Windows Console API (no line buffering)

## Requirements

- Windows (uses `Windows.h` and the Win32 Console API)
- C++17 or later
- Visual Studio 2022 (v143 toolset)

## Building

Open `CPPYama.sln` in Visual Studio and build with **Debug** or **Release** configuration for x64.

## Project Structure

```
CPPYama/
  Canvas/
    canvas.h/.cpp           # Canvas struct and all draw primitives
    Window/
      baseWindow.h          # Abstract base class for windows
      windows.h             # Aggregate include for all windows
      homeWindow.h/.cpp     # Home window implementation
      otherWindow.h/.cpp    # Other window implementation
  Console/
    console.h/.cpp          # ANSI and raw input setup
  Input/
    input.h/.cpp            # (reserved for input utilities)
  Render/
    render.h/.cpp           # (reserved for render utilities)
  types.h                   # Canvas, Button, AppState structs; layout constants
  CPPYama.cpp               # Entry point, main loop, processInput, navigateButtons
```

## Architecture

The main loop follows a simple update/render/input cycle:

```
updateState() → render() → processInput()
```

**Canvas** — a flat `width × height` array of `char` cells with a parallel `bool` array for highlight state. Draw functions write into the canvas; `canvas::flush` emits it to stdout in one pass.

**Buttons** — each `Button` holds a label, an `(x, y)` position, and a `std::function<void(AppState&)>` action invoked on Enter.

**Windows** — classes derived from `baseWindow` with a single `render(Canvas&, x, y, w, h)` virtual method. The active window is rendered into the content area each frame.

## Key Bindings

| Key | Action |~~~~
|---|---|
| Arrow keys | Navigate buttons |
| Enter | Activate selected button |
| Escape | Quit |