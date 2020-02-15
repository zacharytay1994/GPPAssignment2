#include "WorldLights.h"

WorldLights::WorldLights(const std::shared_ptr<Graphics> gfx, const std::shared_ptr<ResourceLibrary> rl, const std::shared_ptr<Input> input)
	:
	gfx(gfx),
	rl(rl),
	input(input)
{
}

void WorldLights::Update(const float& dt)
{
	BindLight();
	DirectX::XMFLOAT3 sun_pos;
	DirectX::XMStoreFloat3(&sun_pos, GetSunPosition());
	directional_light_ = { sun_pos.x, sun_pos.y, sun_pos.z };
	ExecuteDayNightCycle(dt);
}

void WorldLights::Draw()
{
	rl->DrawTexturedCube("sun", GetSunTransform());
}

void WorldLights::BindLight()
{
	gfx->UpdateCBLightSubresource({ { point_light_1_.x, point_light_1_.y, point_light_1_.z, 0.5f * world_light_scalar_ },
		{ point_light_2_.x, point_light_2_.y, point_light_2_.z, 0.5f * world_light_scalar_},
		{ directional_light_.x, directional_light_.y, directional_light_.z, sun_intensity_ * world_light_scalar_} });
}

void WorldLights::SetDirectionalLight(const Vecf3& pos)
{
	directional_light_ = pos;
}

DirectX::XMVECTOR WorldLights::GetSunPosition()
{
	DirectX::XMVECTOR zero = { 0.0f, 0.0f, 0.0f, 0.0f };
	return DirectX::XMVector3Transform(zero,
		DirectX::XMMatrixTranslation(sun_distance_, 0.0f, sun_distance_) *
		DirectX::XMMatrixRotationRollPitchYaw(-angle.x, angle.y, angle.z));
}

void WorldLights::SetPoint1(const Vecf3& pos)
{
	point_light_1_ = pos;
}

void WorldLights::SetPoint2(const Vecf3& pos)
{
	point_light_2_ = pos;
}

void WorldLights::SetWorldLightScale(const float& f)
{
	world_light_scalar_ = f;
}

float WorldLights::GetWorldLightScale()
{
	return world_light_scalar_;
}

Vecf3 WorldLights::GetDirectionalLight()
{
	return directional_light_;
}

DirectX::XMMATRIX WorldLights::GetSunTransform()
{
	// no scaling by 0
	return	DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(30.0f, 30.0f, 30.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) *
		DirectX::XMMatrixTranslation(sun_distance_, 0.0f, sun_distance_) *
		DirectX::XMMatrixRotationRollPitchYaw(-angle.x, angle.y, angle.z) *
		input->GetCameraMatrix() *
		DirectX::XMMatrixPerspectiveLH(1.0f, (float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, 1000.0f)
	);
}

void WorldLights::ExecuteDayNightCycle(const float& dt)
{
	if (angle.x < 3.142f) {
		is_day_ = true;
		angle.x += dt / day_cycle_;
		// calculate intensity
		float ratio = abs(angle.x - 1.5707963267f) / 1.5707963267f;
		sun_intensity_ = ((1.0f - ratio) + 0.2f) * intensity_multiplier_;
	}
	else {
		angle.x += dt / night_cycle_;
		is_day_ = false;
		sun_intensity_ = 0.0f;
		if (angle.x > 6.284f) {
			angle.x = 0.0f;
		}
	}
}
