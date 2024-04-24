#include "LINScreenshot.h"

void LINScreenshot::initXServer() {
    display = XOpenDisplay(NULL);
}

void LINScreenshot::getAttributes() {
    window = DefaultRootWindow(display);
    XGetWindowAttributes(display, window, &attr);
}

void LINScreenshot::makeScreenshot() {
    image = XGetImage(display, window, 0, 0, attr.width, attr.height, AllPlanes, ZPixmap);
    ++number;
}

void LINScreenshot::writeToFile() {

    file = fopen(makeName().c_str(), "wb");

    fprintf(file, "P6\n%d %d\n255\n", attr.width, attr.height);

    // Write pixel data
    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            unsigned long pixel = XGetPixel(image, x, y);
            fputc(pixel >> 16, file); // Red
            fputc(pixel >> 8, file);  // Green
            fputc(pixel, file);       // Blue
        }
    }

    fclose(file);
    XCloseDisplay(display);
    delete image;
    image = nullptr;

    std::cout << "The screenshot #" << number << " is saved" << std::endl;
}

void LINScreenshot::takeScreenshot() {
    initXServer();
    getAttributes();
    makeScreenshot();
    writeToFile();
}