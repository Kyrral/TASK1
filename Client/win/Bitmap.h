#pragma once
#include <Windows.h>
#include <iostream>

class Bitmap
{
public:
    //bmp from clipboard
    HBITMAP hBitmap;

    //info and data for .bmp file
    BITMAP bmp;
    BITMAPINFOHEADER bi;
    BITMAPFILEHEADER bmfHeader;

    //for .bmp main data size 
    DWORD dwSize;
    //pixel buffer
    LPSTR lpBitmap;

private:
    void getHBITMAP();

    void getBmpData();
    void setHeader();
    void setInfoHeader();
    void setHeaders();

    void getPixels();

public:
    void configureBmp();
    void finish();
};