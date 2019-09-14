#include "../Base/Camera.h"
#include "../Base/Sprite.h"

DirectX::XMMATRIX Camera::camera_translation_ = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX Camera::camera_rotation_ = DirectX::XMMatrixIdentity();

Camera::Camera() 
{
}

Camera::~Camera()
{
}

void Camera::FollowSprite(Sprite & sprite)
{
	camera_x_world_ = sprite.GetWorldX();
	camera_y_world_ = -sprite.GetWorldY();
	camera_translation_ = DirectX::XMMatrixTranslation(-sprite.GetWorldX() + 1.0f, -sprite.GetWorldY() - 1.0f, 0.0f);
}

float Camera::GetCameraXWorld()
{
	return camera_x_world_;
}

float Camera::GetCameraYWorld()
{
	return camera_y_world_;
}
