#pragma once
#include "baseWindow.h"

class otherWindow : public baseWindow
{
public:
    void render(Canvas& c) const override;
    void onShow(int x, int y, int w, int h) override;
    void onHide() override;
};
