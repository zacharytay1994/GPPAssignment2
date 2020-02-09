#include "Cube.h"
#include "D3dcompiler.h"

Cube::Cube(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, const std::string& filename, const std::shared_ptr<ResourceLibrary> rl)
	:
	gfx(gfx),
	input(input),
	texture_key_(filename),
	rl_(rl)
{
	InitializeCube(0, 0, 0, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f);
}

Cube::~Cube()
{
}

dx::XMMATRIX Cube::GetTransform(const float& dt)
{
	// no scaling by 0
	assert(cube_data_.scale_x_ != 0.0f || cube_data_.scale_y_ != 0.0f || cube_data_.scale_z_ != 0.0f);
	return	dx::XMMatrixTranspose(
		dx::XMMatrixRotationRollPitchYaw(cube_data_.angle_x, cube_data_.angle_y, cube_data_.angle_z) *
		dx::XMMatrixScaling(cube_data_.scale_x_, cube_data_.scale_y_, cube_data_.scale_z_) *
		dx::XMMatrixTranslation(cube_data_.world_xoffset_, cube_data_.world_yoffset_, cube_data_.world_zoffset_) *
		input->GetCameraMatrix(dt) *
		dx::XMMatrixPerspectiveLH(1.0f,(float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, 1000.0f) 
	);
}

dx::XMMATRIX Cube::GetModelTransform()
{
	// no scaling by 0
	assert(cube_data_.scale_x_ != 0.0f || cube_data_.scale_y_ != 0.0f || cube_data_.scale_z_ != 0.0f);
	return	dx::XMMatrixTranspose(
		dx::XMMatrixRotationRollPitchYaw(cube_data_.angle_x, cube_data_.angle_y, cube_data_.angle_z) *
		dx::XMMatrixScaling(cube_data_.scale_x_, cube_data_.scale_y_, cube_data_.scale_z_) *
		dx::XMMatrixTranslation(cube_data_.world_xoffset_, cube_data_.world_yoffset_, cube_data_.world_zoffset_)
	);
}

void Cube::SetX(const float& x)
{
	cube_data_.world_xoffset_ = x;
}

void Cube::SetY(const float& y)
{
	cube_data_.world_yoffset_ = y;
}

void Cube::SetZ(const float& z)
{
	cube_data_.world_zoffset_ = z;
}

void Cube::SetWidth(const int& width)
{
	cube_data_.width_ = width;
}

void Cube::SetHeight(const int& height)
{
	cube_data_.height_ = height;
}

void Cube::SetDepth(const int& depth)
{
	cube_data_.depth_ = depth;
}

void Cube::SetScaleX(const float& scalex)
{
	cube_data_.scale_x_ = scalex;
}

void Cube::SetScaleY(const float& scaley)
{
	cube_data_.scale_y_ = scaley;
}

void Cube::SetScaleZ(const float& scalez)
{
	cube_data_.scale_z_ = scalez;
}

void Cube::SetAngleZ(const float& angle)
{
	cube_data_.angle_z = angle;
}

void Cube::SetAngleX(const float& angle)
{
	cube_data_.angle_x = angle;
}

void Cube::SetAngleY(const float& angle)
{
	cube_data_.angle_y = angle;
}

float Cube::GetX()
{
	return cube_data_.world_xoffset_;
}

float Cube::GetY()
{
	return cube_data_.world_yoffset_;
}

float Cube::GetZ()
{
	return cube_data_.world_zoffset_;
}

int Cube::GetWidth()
{
	return cube_data_.width_;
}

int Cube::GetHeight()
{
	return cube_data_.height_;
}

int Cube::GetDepth()
{
	return cube_data_.depth_;
}

float Cube::GetScaleX()
{
	return cube_data_.scale_x_;
}

float Cube::GetScaleY()
{
	return cube_data_.scale_y_;
}

float Cube::GetScaleZ()
{
	return cube_data_.scale_z_;
}

float Cube::GetAngleZ()
{
	return cube_data_.angle_z;
}

float Cube::GetAngleX()
{
	return cube_data_.angle_x;
}

float Cube::GetAngleY()
{
	return cube_data_.angle_y;
}

void Cube::SetVisible(const bool& visible)
{
	visible_ = visible;
}

bool Cube::Visible()
{
	return visible_;
}

void Cube::Draw(const float& dt)
{
	assert(initialized_ == true);
	if (visible_) {
		rl_->DrawTexturedCubeNorm(texture_key_, GetTransform(0), GetModelTransform());
	}
}

void Cube::Update(const float& frametime)
{
}

void Cube::InitializeCube(const int& x, const int& y, const int& z, const float& scalex, const float& scaley, const float& scalez, const float& anglez, const float& anglex, const float& angley)
{
	SetX(x);
	SetY(y);
	SetZ(z);
	SetScaleX(scalex);
	SetScaleY(scaley);
	SetScaleZ(scalez);
	SetAngleZ(anglez);
	SetAngleX(anglex);
	SetAngleY(angley);
	initialized_ = true;
}
