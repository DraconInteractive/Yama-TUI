#pragma once
#include "baseWindow.h"

class homeWindow : public baseWindow
{
public:
    void render(Canvas& c, int x, int y, int w, int h) const override;
};
