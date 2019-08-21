#pragma once

#include "Color.h"
#include <string>
class Surface {
private:
	Color* p_pixels_ = nullptr;
	int width_;
	int height_;
public:
	Surface(const std::wstring& filename);
	Surface(int width, int height);
	Surface(const Surface& rhs);
	~Surface();
	Surface& operator=(const Surface& rhs);
	void PutPixel(int x, int y, Color c);
	Color GetPixel(int x, int y) const;
	int GetWidth() const;
	int GetHeight() const;
	Color* GetBufferPointer() const;
	void BakeAlpha();
};