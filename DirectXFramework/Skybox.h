#pragma once
#include "ResourceLibrary.h"
#include "Base/Input.h"

class Skybox {
private:
	std::shared_ptr<ResourceLibrary> rl_;
	std::shared_ptr<Input> input_;
	float fov_ = 0.6f;
	float scale_ = 1500.0f;
	std::string background_key_ = "spacebox";
public:
	Skybox(std::shared_ptr<ResourceLibrary> rl, std::shared_ptr<Input> input);
	DirectX::XMMATRIX GetSkyboxTransform(const Vecf3& angle, const Vecf3& translate);
	void Render();
};