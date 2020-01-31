#include "Cube.h"

Cube::Cube(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, const std::wstring& filename, const Vecf3& scale)
	:
	gfx(gfx),
	input(input),
	image_resource_(filename)
{
	SetScaleX(scale.x);
	SetScaleY(scale.y);
	SetScaleZ(scale.z);
	InitializeCube(0, 0, 0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
}

Cube::~Cube()
{
	if (cube_data_.srv_sprite_ != nullptr) {
		cube_data_.srv_sprite_->Release();
	}
}

Surface Cube::GetSurface()
{
	return image_resource_;
}

dx::XMMATRIX Cube::GetTransform()
{
	// no scaling by 0
	assert(cube_data_.scale_x_ != 0.0f || cube_data_.scale_y_ != 0.0f || cube_data_.scale_z_ != 0.0f);
	return	dx::XMMatrixTranspose(
		dx::XMMatrixRotationRollPitchYaw(cube_data_.angle_x, cube_data_.angle_y, cube_data_.angle_z) *
		/*dx::XMMatrixRotationZ(cube_data_.angle_z) *
		dx::XMMatrixRotationX(cube_data_.angle_x) *
		dx::XMMatrixRotationY(cube_data_.angle_y) **/
		dx::XMMatrixScaling(cube_data_.scale_x_, cube_data_.scale_y_, cube_data_.scale_z_) *
		dx::XMMatrixTranslation(cube_data_.world_xoffset_, cube_data_.world_yoffset_, 8.0f) *
		input->GetCameraMatrix() *
		dx::XMMatrixPerspectiveLH(1.0f,(float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, 1000.0f) 
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
	cube_data_.scale_z_;
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

void Cube::Draw()
{
	assert(initialized_ == true);
	if (visible_) {
		gfx->BindShaderResourceView(cube_data_.srv_sprite_);
		gfx->UpdateCBTransformSubresource({ GetTransform() });
		gfx->DrawIndexed();
	}
}

void Cube::Update(const float& frametime)
{
}

void Cube::InitializeCube(const int& x, const int& y, const int& z, const float& scalex, const float& scaley, const float& scalez, const float& anglez, const float& anglex, const float& angley)
{
	SetX(x);
	SetY(y);
	SetScaleX(scalex);
	SetScaleY(scaley);
	SetAngleZ(anglez);
	SetAngleX(anglex);
	SetAngleY(angley);
	CreateShaderResourceView();
	initialized_ = true;
}

void Cube::CreateShaderResourceView()
{
	if (gfx->Initialized()) {
		D3D11_TEXTURE2D_DESC texture_description = {};
		texture_description.Width = image_resource_.GetWidth();
		texture_description.Height = image_resource_.GetHeight();
		texture_description.MipLevels = 1;
		texture_description.ArraySize = 1;
		texture_description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texture_description.SampleDesc.Count = 1;
		texture_description.SampleDesc.Quality = 0;
		texture_description.Usage = D3D11_USAGE_DEFAULT;
		texture_description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_description.CPUAccessFlags = 0;
		texture_description.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = image_resource_.GetBufferPointer();
		subresource_data.SysMemPitch = image_resource_.GetWidth() * sizeof(Color);
		ID3D11Texture2D* p_texture;
		gfx->GetDevice().CreateTexture2D(&texture_description, &subresource_data, &p_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_descriptor = {};
		srv_descriptor.Format = texture_description.Format;
		srv_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_descriptor.Texture2D.MostDetailedMip = 0;
		srv_descriptor.Texture2D.MipLevels = 1;
		if (p_texture != 0) {
			gfx->GetDevice().CreateShaderResourceView(p_texture, &srv_descriptor, &cube_data_.srv_sprite_);
		}
		p_texture->Release();
		gfx->BindShaderResourceView(cube_data_.srv_sprite_);
	}
}
