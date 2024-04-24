#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdio>

#include "../AScreenshot.h"

class LINScreenshot : public AScreenshot
{
    FILE* file = nullptr;

    Display* display = nullptr;
    Window window;
    XWindowAttributes attr;

    XImage* image;

    void initXServer();
    void getAttributes();
    void makeScreenshot();
    void writeToFile();

public:
    void takeScreenshot() override;
};