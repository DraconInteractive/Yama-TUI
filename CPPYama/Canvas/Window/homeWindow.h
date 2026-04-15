#pragma once
#include "baseWindow.h"
#include <string>

class homeWindow : public baseWindow
{
public:
    void render(Canvas& c) const override;
    void onShow(int x, int y, int w, int h) override;
    void onHide() override;

private:
    std::string currentDisplay;
};
