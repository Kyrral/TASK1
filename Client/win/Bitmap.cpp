#include "Bitmap.h"

void Bitmap::getBmpData()
{
    getHBITMAP();
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
}

void Bitmap::getHBITMAP()
{
    OpenClipboard(nullptr);
    hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    if (hBitmap == nullptr) std::cout << "no bitmap\n";
}

void Bitmap::setHeader()
{
        bmfHeader.bfType = 'MB';
        bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmp.bmWidthBytes * bmp.bmHeight;
        bmfHeader.bfReserved1 = 0;
        bmfHeader.bfReserved2 = 0;
        bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

void Bitmap::setInfoHeader()
{
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = bmp.bmWidth;
        bi.biHeight = -bmp.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0; 
}

void Bitmap::getPixels()
{
    dwSize = bmp.bmWidthBytes * bmp.bmHeight;
    lpBitmap = new char[dwSize];

    GetBitmapBits(hBitmap, dwSize, lpBitmap);
}

void Bitmap::setHeaders() {
    setHeader();
    setInfoHeader();
}

void Bitmap::configureBmp() {
    getBmpData();
    setHeaders();
    getPixels();
}

void Bitmap::finish()
{
    delete[] lpBitmap;
    CloseClipboard();
}