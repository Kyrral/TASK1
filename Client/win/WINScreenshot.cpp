#include "WINScreenshot.h"

void WINScreenshot::takeScreenshot()
{
	++number;
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);

	Sleep(1000);

    saveToFolder();
}

void WINScreenshot::saveToFolder()
{
	bitmap.configureBmp();

	FILE* file = fopen(makeName().c_str(), "wb");
	if (file == nullptr) std::cout << "Error in creating file" << std::endl;

   //WRITING HEADERS
   fwrite(&bitmap.bmfHeader, sizeof(BITMAPFILEHEADER), 1, file);
   fwrite(&bitmap.bi, sizeof(BITMAPINFOHEADER), 1, file);
   if (fwrite(bitmap.lpBitmap, bitmap.dwSize, 1, file) != 1) std::cerr << "pixel err\n";

   bitmap.finish();

   fclose(file);
}