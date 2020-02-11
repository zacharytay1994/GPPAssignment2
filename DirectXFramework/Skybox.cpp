#include "Skybox.h"
#include <assert.h>

Skybox::Skybox(std::shared_ptr<ResourceLibrary> rl, std::shared_ptr<Input> input)
	:
	rl_(rl),
	input_(input)
{
}

DirectX::XMMATRIX Skybox::GetSkyboxTransform(const Vecf3& angle, const Vecf3& translate)
{
	Vecf3 cam_pos = input_->GetCameraPosition();
	assert(scale_ > 0.0f);
	return	DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(scale_, scale_, scale_) *
		DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z) *
		DirectX::XMMatrixTranslation(translate.x, translate.y, translate.z) *
		DirectX::XMMatrixRotationRollPitchYaw(0.78539816f, 0.0f, 0.78539816f) *
		DirectX::XMMatrixTranslation(cam_pos.x, cam_pos.y, cam_pos.z) *
		input_->GetCameraMatrix() *
		DirectX::XMMatrixPerspectiveLH(fov_, fov_ * (float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, scale_*2.0f)
	);
}

void Skybox::Render()
{
	// draw back face
	rl_->DrawTexturedPlane(background_key_, GetSkyboxTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.5f * scale_ }));
	// draw right face
	rl_->DrawTexturedPlane(background_key_, GetSkyboxTransform({ 0.0f, 1.5707963f, 0.0f }, { 0.5f * scale_, 0.0f, 0.0f }));
	// draw left face
	rl_->DrawTexturedPlane(background_key_, GetSkyboxTransform({ 0.0f, -1.5707963f, 0.0f }, { -0.5f * scale_, 0.0f, 0.0f }));
	// draw back face
	rl_->DrawTexturedPlane(background_key_, GetSkyboxTransform({ 0.0f, 3.1415926, 0.0f }, { 0.0f, 0.0f, -0.5f * scale_ }));
	// draw top face
	rl_->DrawTexturedPlane(background_key_, GetSkyboxTransform({ -1.5707963f, 0.0f, 0.0f }, { 0.0f, 0.5f * scale_, 0.0f }));
	// draw bottom face
	rl_->DrawTexturedPlane(background_key_, GetSkyboxTransform({ 1.5707963f, 0.0f, 0.0f }, { 0.0f, -0.5f * scale_, 0.0f }));
}
