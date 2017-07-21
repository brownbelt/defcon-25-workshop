#pragma once

#include <Windows.h>
#include <gdiplus.h>

#include <memory>

#pragma comment(lib, "Gdiplus.lib")

static void save_bitmap(HBITMAP hBitmap, LPCWSTR wszFilePath)
{
	static const CLSID clsidPng = { 0x557cf406, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

	ULONG_PTR gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = { 0 };

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	{
		std::unique_ptr<Gdiplus::Bitmap> image(new Gdiplus::Bitmap(hBitmap, NULL));
		image->Save(wszFilePath, &clsidPng, NULL);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
}