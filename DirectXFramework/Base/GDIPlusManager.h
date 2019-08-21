#pragma once

#include <Windows.h>

class GDIPlusManager {
private:
	static ULONG_PTR token;
	static int ref_count_;
public:
	GDIPlusManager();
	~GDIPlusManager();
};