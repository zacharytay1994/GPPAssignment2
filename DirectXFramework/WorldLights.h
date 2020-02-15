#pragma once

#include "Vec3.h"
#include "Base/Graphics.h"
#include "ResourceLibrary.h"
#include "Base/Input.h"

#include <memory>

class WorldLights {
private:
	std::shared_ptr<Graphics> gfx;
	std::shared_ptr<Input> input;
	std::shared_ptr<ResourceLibrary> rl;
	Vecf3 point_light_1_ = { 0.0f, 0.0f, 0.0f }; // Light belonging to the train
	Vecf3 point_light_2_ = { 0.0f, 0.0f, 0.0f }; // Light belonging to the player

	float day_cycle_ = 5.0f;
	float night_cycle_ = 2.0f;
	bool is_day_ = true;

	// Sun variables
	Vecf3 angle = { 0.0f, 0.0f, 0.0f };
	float sun_distance_ = 500.0f;
	float sun_intensity_ = 1.0f;
	float intensity_multiplier_ = 2.0f;
	Vecf3 directional_light_ = { 0.0f, 0.0f, 0.0f }; // Sun light
public:
	WorldLights(const std::shared_ptr<Graphics> gfx, const std::shared_ptr<ResourceLibrary> rl, const std::shared_ptr<Input> input);
	void Update(const float& dt);
	void Draw();
	void BindLight();
	void SetDirectionalLight(const Vecf3& pos);
	DirectX::XMVECTOR GetSunPosition();
	void SetPoint1(const Vecf3& pos);
	void SetPoint2(const Vecf3& pos);
	Vecf3 GetDirectionalLight();
	DirectX::XMMATRIX GetSunTransform();
	void ExecuteDayNightCycle(const float& dt);
};