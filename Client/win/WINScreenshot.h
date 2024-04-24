#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

#include "Bitmap.h"
#include "../AScreenshot.h"

class WINScreenshot : public AScreenshot
{
	Bitmap bitmap;

	void saveToFolder();

public:
	void takeScreenshot() override;
};