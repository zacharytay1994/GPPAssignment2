#pragma comment(lib, "gdiplus.lib")

#include "../Base/GDIPlusManager.h"
#include <algorithm>
#include <gdiplus.h>

namespace Gdiplus {
	using std::min;
	using std::max;
}

ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::ref_count_ = 0;

GDIPlusManager::GDIPlusManager()
{
	// if gdiplus is not yet initialized
	if (ref_count_++ == 0) {
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}

GDIPlusManager::~GDIPlusManager()
{
	// if nothing else is referencing gdiplus
	if (--ref_count_ == 0) {
		Gdiplus::GdiplusShutdown(token);
	}
}
