#pragma once

#include "Vec3.h"
#include "Base/Graphics.h"
#include "ResourceLibrary.h"
#include "Base/Input.h"
#include "Skybox.h"

#include <memory>

class WorldLights {
private:
	std::shared_ptr<Graphics> gfx;
	std::shared_ptr<Input> input;
	std::shared_ptr<ResourceLibrary> rl;
	std::shared_ptr<Skybox> sb_;
	Vecf3 point_light_1_ = { 0.0f, 0.0f, 0.0f }; // Light belonging to the train
	Vecf3 point_light_2_ = { 0.0f, 0.0f, 0.0f }; // Light belonging to the player
	Vecf3 point_light_3_ = { 0.0f, 0.0f, 0.0f }; // Light belonging to the player2

	float day_cycle_ = 30.0f;
	float night_cycle_ = 30.0f;
	float sky_shade_ = 0.8f;
	bool is_day_ = true;

	// Sun variables
	Vecf3 angle = { 0.0f, 0.0f, 0.0f };
	float sun_distance_ = 500.0f;
	float sun_intensity_ = 1.0f;
	float intensity_multiplier_ = 2.0f;
	Vecf3 directional_light_ = { 0.0f, 0.0f, 0.0f }; // Sun light
	float world_light_scalar_ = 1.0f;
public:
	WorldLights(const std::shared_ptr<Graphics> gfx, const std::shared_ptr<ResourceLibrary> rl, const std::shared_ptr<Input> input,
		const std::shared_ptr<Skybox> sb);
	void Update(const float& dt);
	void Draw();
	void BindLight();
	void SetDirectionalLight(const Vecf3& pos);
	DirectX::XMVECTOR GetSunPosition();
	void SetPoint1(const Vecf3& pos);
	void SetPoint2(const Vecf3& pos);
	void SetPoint3(const Vecf3& pos);
	void SetWorldLightScale(const float& f);
	float GetWorldLightScale();
	Vecf3 GetDirectionalLight();
	DirectX::XMMATRIX GetSunTransform();
	void ExecuteDayNightCycle(const float& dt);
};