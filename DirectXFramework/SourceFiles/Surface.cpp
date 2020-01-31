#pragma comment(lib, "Gdiplus.lib")

#include "../Base/Surface.h"

#include <Windows.h>
#include <algorithm>

namespace GDIplus {
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <assert.h>
#include <fstream>

Surface::Surface(const std::wstring & filename)
{
	// file name at least 4 characters long
	if (filename.length() < 4) {
		std::string narrow(filename.begin(), filename.end());
		throw std::runtime_error("Surface::Surface filename must be at least 4 characters long: " + narrow);
	}
	// open image file with gdiplus (not just bitmap although its called bitmap)
	Gdiplus::Bitmap bitmap(filename.c_str());
	// check if file loaded successfully
	if (bitmap.GetLastStatus() != Gdiplus::Ok) {
		std::string narrow(filename.begin(), filename.end());
		throw std::runtime_error("Surface::Surface failed to load file: " + narrow);
	}
	// allocate Surface resources and set dimensions
	width_ = bitmap.GetWidth();
	height_ = bitmap.GetHeight();
	p_pixels_ = new Color[width_ * height_];
	// test if pixel format is alpha, and save result
	const bool isAlpha = Gdiplus::IsAlphaPixelFormat(bitmap.GetPixelFormat()) == TRUE;
	// loop through image dimensions, copy gdi resource to surface
	for (int y = 0; y < height_; y++) {
		for (int x = 0; x < width_; x++) {
			// to extract color from gdi bitmap
			Gdiplus::Color pixel;
			bitmap.GetPixel(x, y, &pixel);
			// write to surface, with alpha if exists
			if (isAlpha) {
				PutPixel(x, y, { pixel.GetA(), pixel.GetR(), pixel.GetG(), pixel.GetB() });
			}
			else {
				PutPixel(x, y, { pixel.GetR(), pixel.GetG(), pixel.GetB() });
			}
		}
	}
	// check to see whether filename starts with "rgba_", if matches
	if (filename.find(L"alpha_") != std::wstring::npos) {
		BakeAlpha();
	}
}

Surface::Surface(int width, int height)
	:
	width_(width),
	height_(height),
	p_pixels_(new Color[width_ * height_])
{
}

Surface::Surface(const Surface& rhs)
	:
	Surface(rhs.width_, rhs.height_)
{
	const int n_pixels = width_ * height_;
	for (int i = 0; i < n_pixels; i++) {
		p_pixels_[i] = rhs.p_pixels_[i];
	}
}

Surface::~Surface()
{
	delete[] p_pixels_;
	p_pixels_ = nullptr;
}

Surface& Surface::operator=(const Surface& rhs)
{
	if (this != &rhs) {
		width_ = rhs.width_;
		height_ = rhs.height_;

		delete[] p_pixels_;
		p_pixels_ = new Color[width_*height_];

		const int n_pixels = width_ * height_;
		for (int i = 0; i < n_pixels; i++) {
			p_pixels_[i] = rhs.p_pixels_[i];
		}
	}
	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width_);
	assert(y >= 0);
	assert(y < height_);
	p_pixels_[y*width_ + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width_);
	assert(y >= 0);
	assert(y < height_);
	return p_pixels_[y*width_ + x];
}

int Surface::GetWidth() const
{
	return width_;
}

int Surface::GetHeight() const
{
	return height_;
}

Color * Surface::GetBufferPointer() const
{
	return p_pixels_;
}

void Surface::BakeAlpha()
{
	const int n_pixels = width_ * height_;
	for (int i = 0; i < n_pixels; i++) {
		Color pixel = p_pixels_[i];
		const int alpha = pixel.GetA();
		// multiply alpha with color channels
		pixel.SetR(int(pixel.GetR() * ((float)alpha / 255.0f)));
		pixel.SetG(int(pixel.GetG() * ((float)alpha / 255.0f)));
		pixel.SetB(int(pixel.GetB() * ((float)alpha / 255.0f)));
		// ammend surface with baked alpha colors
		p_pixels_[i] = pixel;
	}
}
