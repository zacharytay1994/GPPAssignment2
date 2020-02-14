#pragma once

#include "ResourceLibrary.h"
#include "Vec3.h"

#include <memory>

class GUI {
private:
	std::shared_ptr<ResourceLibrary> rl_;
public:
	GUI(std::shared_ptr<ResourceLibrary> rl);
	DirectX::XMMATRIX GetTransform(const Vecf3& position, const Vecf3& scale);
	void Draw();
};