#pragma once
#include "ResourceLibrary.h"
#include "Base/Input.h"

class Skybox {
private:
	std::shared_ptr<ResourceLibrary> rl_;
	std::shared_ptr<Input> input_;
	float fov_ = 1.0f;
	float scale_ = 1500.0f;
	std::string background_key_ = "spacebox";
	float sky_shade_ = 0.8f;
public:
	Skybox(std::shared_ptr<ResourceLibrary> rl, std::shared_ptr<Input> input);
	DirectX::XMMATRIX GetSkyboxTransform(const Vecf3& angle, const Vecf3& translate);
	void SetSkyShade(const float& f);
	void Render();
};