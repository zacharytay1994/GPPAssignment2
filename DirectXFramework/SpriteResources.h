#pragma once

#include <unordered_map>
#include <d3d11.h>
#include <memory>

#include "Base/Surface.h"
#include "Base/Graphics.h"

class SpriteResources {
private:
	// graphics reference
	std::shared_ptr<Graphics> gfx;

	static const int resource_count_ = 2;
	Surface surface_array_[resource_count_] = {
		Surface(L"Images/adventuregirlidle.png"),
		Surface(L"Images/adventuregirlrun.png")};
	ID3D11ShaderResourceView* srv_array_[resource_count_];
	std::unordered_map<std::wstring, int> index_map_{
	{L"Images/adventuregirlidle.png", 0},
	{L"Images/adventuregirlrun.png",1},
	};
	std::unordered_map<std::string, int> test_map_{
		{"something",0},
		{"something2",1},
	};
public:
	SpriteResources(std::shared_ptr<Graphics>& graphics);
	void InitMap();
	void CreateShaderResourceView(const Surface& surface, const int& index);

	// Getters Setters
	Surface& GetSurfaceReference(const std::wstring& filename);
	ID3D11ShaderResourceView*& GetSRVReference(const std::wstring& filename);
};