#include "../Base/Sprite.h"
#include <assert.h>

Sprite::Sprite(std::shared_ptr<Graphics> gfx, const std::wstring& filename, const int& width, const int& height, std::shared_ptr<SpriteResources>& sr)
	:
	gfx(gfx),
	sr_(sr),
	image_resource_(GetSurfaceFromSpriteResources(filename))
{
	SetWidth(width);
	SetHeight(height);
	InitializeSprite(0, 0, 1.0f, 1.0f, 0.0f);
}

Sprite::~Sprite()
{
	// sprite no longer in charge of releasing srv object
	// now handled by spriteresources
	/*if (sprite_data_.srv_sprite_ != nullptr) {
		sprite_data_.srv_sprite_->Release();
	}*/
	// just delete the pointer
	sprite_data_.srv_sprite_ = nullptr;
	delete sprite_data_.srv_sprite_;
}

Surface Sprite::GetSurface()
{
	return image_resource_;
}

dx::XMMATRIX Sprite::GetTransform()
{
	// no scaling by 0
	assert(sprite_data_.scale_x_ != 0.0f || sprite_data_.scale_y_ != 0.0f);
	return	dx::XMMatrixTranspose(
				dx::XMMatrixScaling(sprite_data_.scale_dimension_x_ * sprite_data_.scale_x_, sprite_data_.scale_dimension_y_ * sprite_data_.scale_y_, 1.0f) *
				dx::XMMatrixRotationZ(sprite_data_.angle_) *
				dx::XMMatrixScaling((float)Graphics::viewport_height_/(float)Graphics::viewport_width_,1.0f,1.0f) *
				dx::XMMatrixTranslation(-1.0f,1.0f,0.0f) *
				dx::XMMatrixTranslation(sprite_data_.world_xoffset_, sprite_data_.world_yoffset_, 0.0f)
	);
}

void Sprite::SetX(const int& x)
{
	sprite_data_.screen_xoffset_ = x;
	sprite_data_.world_xoffset_ = ((float)x / Graphics::viewport_width_) * 2.0f;
}

void Sprite::SetY(const int& y)
{
	sprite_data_.screen_yoffset_ = y;
	sprite_data_.world_yoffset_ = -((float) y / Graphics::viewport_height_) * 2.0f;
}

void Sprite::SetWidth(const int & width)
{
	assert(width > 0);
	sprite_data_.width_ = width;
	DimensionsToScale();
}

void Sprite::SetHeight(const int & height)
{
	//assert(height > 0);
	sprite_data_.height_ = height;
	DimensionsToScale();
}

void Sprite::SetScaleX(const float & scalex)
{
	assert(scalex > 0.0f);
	sprite_data_.scale_x_ = scalex;
}

void Sprite::SetScaleY(const float & scaley)
{
	assert(scaley > 0.0f);
	sprite_data_.scale_y_ = scaley;
}

void Sprite::SetAngle(const float & angle)
{
	sprite_data_.angle_ = angle;
}

void Sprite::FlipHorizontal(const bool & flip)
{
	// if flip is not redundant, i.e. already flipped
	if ((flip && !sprite_data_.flip_horizontal_) || (!flip && sprite_data_.flip_horizontal_)) {
		sprite_data_.flip_horizontal_ = flip;
		Coord tl = tc_.top_left;
		Coord tr = tc_.top_right;
		Coord bl = tc_.bottom_left;
		Coord br = tc_.bottom_right;
		tc_.top_left = tr;
		tc_.top_right = tl;
		tc_.bottom_left = br;
		tc_.bottom_right = bl;
		UpdateVB();
	}
}

void Sprite::FlipVertical(const bool & flip)
{
	// if flip is not redundant, i.e. already flipped
	if ((flip && !sprite_data_.flip_vertical_) || (!flip && sprite_data_.flip_vertical_)) {
		sprite_data_.flip_vertical_ = flip;
		Coord tl = tc_.top_left;
		Coord tr = tc_.top_right;
		Coord bl = tc_.bottom_left;
		Coord br = tc_.bottom_right;
		tc_.top_left = bl;
		tc_.top_right = br;
		tc_.bottom_left = tl;
		tc_.bottom_right = tr;
		UpdateVB();
	}
}

int Sprite::GetX()
{
	return sprite_data_.screen_xoffset_;
}

int Sprite::GetY()
{
	return sprite_data_.screen_yoffset_;
}

int Sprite::GetWidth()
{
	return sprite_data_.width_;
}

int Sprite::GetHeight()
{
	return sprite_data_.height_;
}

float Sprite::GetScaleX()
{
	return sprite_data_.scale_x_;
}

float Sprite::GetScaleY()
{
	return sprite_data_.scale_y_;
}

float Sprite::GetAngle()
{
	return sprite_data_.angle_;
}

void Sprite::SetColumns(const int & cols)
{
	assert(cols >= 0);
	sprite_sheet_columns_ = cols;
	unit_x_ = 1.0f / (float)cols;
}

void Sprite::SetRows(const int & rows)
{
	assert(rows >= 0);
	sprite_sheet_rows_ = rows;
	unit_y_ = 1.0f / (float)rows;
}

void Sprite::SetCurrentFrame(const int & frame)
{
	// frame cannot exceed total number of frames or fall below 0
	assert(frame >= 0 && frame < (sprite_sheet_columns_ * sprite_sheet_rows_));
	current_frame_ = frame;
}

void Sprite::SetFrameDelay(const float & framedelay)
{
	frame_delay_ = framedelay;
}

void Sprite::SetLoop(const bool & loop)
{
	loop_ = loop;
}

void Sprite::SetVisible(const bool & visible)
{
	visible_ = visible;
}

int Sprite::GetCurrentFrame()
{
	return current_frame_;
}

bool Sprite::Loop()
{
	return loop_;
}

bool Sprite::Visible()
{
	return visible_;
}

void Sprite::ResetAnimation()
{
	SetCurrentFrame(start_frame_);
}

void Sprite::Draw()
{
	assert(initialized_ == true);
	if (visible_) {
		gfx->BindShaderResourceView(sprite_data_.srv_sprite_);
		gfx->UpdateVBVertexSubresource(vb);
		gfx->UpdateCBTransformSubresource({ GetTransform() });
		gfx->Draw();
	}
}

void Sprite::Update(const float & frametime)
{
	// if sprite has multiple frames, i.e. animated
	if (end_frame_ - start_frame_ > 0) {
		animation_timer_ += frametime;
		if (animation_timer_ > frame_delay_) {
			animation_timer_ -= frame_delay_;
			current_frame_++;
			if (current_frame_ < start_frame_ || current_frame_ > end_frame_) {
				if (loop_ == true) {
					current_frame_ = start_frame_;
				}
				else {
					current_frame_ = end_frame_;
					animation_complete_ = true;
				}
			}
			// synonymous to setRect() in previous framework
			tc_ = FrameToTexcoord(current_frame_);
			UpdateVB();
		}
	}
}

void Sprite::InitializeSprite(const int & x, const int & y, const float & scalex, const float & scaley, const float & angle)
{
	SetX(x);
	SetY(y);
	SetScaleX(scalex);
	SetScaleY(scaley);
	SetAngle(angle);
	FlipHorizontal(false);
	FlipVertical(false);
	//CreateShaderResourceView();
	GetSetSRVFromSpriteResources();
	initialized_ = true;
}

void Sprite::InitializeAnimation(const int & cols, const int & rows, const int & startframe, const int & endframe, const float & framedelay, const bool & loop)
{
	SetColumns(cols);
	SetRows(rows);
	assert(startframe >= 0 && startframe < (cols * rows));
	start_frame_ = startframe;
	SetCurrentFrame(startframe);
	assert(endframe >= 0 && endframe < (cols * rows));
	end_frame_ = endframe;
	SetFrameDelay(framedelay);
	SetLoop(loop);
}

void Sprite::CreateShaderResourceView()
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
		gfx->GetDevice().CreateShaderResourceView(p_texture, &srv_descriptor, &sprite_data_.srv_sprite_);
		p_texture->Release();
		gfx->BindShaderResourceView(sprite_data_.srv_sprite_);
	}
}

void Sprite::DimensionsToScale()
{
	sprite_data_.scale_dimension_x_ = (((float)sprite_data_.width_ / 2.0f) / ((float)Graphics::viewport_height_ / (float)Graphics::viewport_width_)) / (Graphics::viewport_width_ / 2.0f);
	sprite_data_.scale_dimension_y_ = ((float)sprite_data_.height_ / 2.0f) / (Graphics::viewport_height_ / 2.0f);
}

void Sprite::UpdateVB()
{
	vb.topleft = { -1.0f,1.0f,tc_.top_left.x,tc_.top_left.y };
	vb.topright = { 1.0f,1.0f,tc_.top_right.x,tc_.top_right.y };
	vb.bottomleft = { -1.0f,-1.0f,tc_.bottom_left.x,tc_.bottom_left.y };
	vb.bottomright = { 1.0f,-1.0f,tc_.bottom_right.x,tc_.bottom_right.y };
}

TexCoord Sprite::FrameToTexcoord(const int& frame)
{
	// images have at least 1 column
	assert(sprite_sheet_columns_ > 0);
	int row = int(frame / sprite_sheet_columns_);
	int col = frame % sprite_sheet_columns_;
	float start_coord_x = col * unit_x_;
	float start_coord_y = row * unit_y_;
	// check flip orientation of sprite and return corresponding vertex order
	if (sprite_data_.flip_horizontal_ && sprite_data_.flip_vertical_) {
		return { {start_coord_x + unit_x_, start_coord_y + unit_y_},
			{start_coord_x, start_coord_y + unit_y_},
			{start_coord_x + unit_x_, start_coord_y},
			{start_coord_x, start_coord_y} };
	}
	else if (sprite_data_.flip_horizontal_) {
		return { {start_coord_x + unit_x_, start_coord_y},
			{start_coord_x, start_coord_y},
			{start_coord_x + unit_x_, start_coord_y + unit_y_},
			{start_coord_x, start_coord_y + unit_y_} };
	}
	else if (sprite_data_.flip_vertical_) {
		return { {start_coord_x, start_coord_y + unit_y_},
			{start_coord_x + unit_x_, start_coord_y + unit_y_},
			{start_coord_x, start_coord_y},
			{start_coord_x + unit_x_, start_coord_y} };
	}
	return { {start_coord_x, start_coord_y},
			{start_coord_x + unit_x_, start_coord_y},
			{start_coord_x, start_coord_y + unit_y_},
			{start_coord_x + unit_x_, start_coord_y + unit_y_} };
}

Surface& Sprite::GetSurfaceFromSpriteResources(const std::wstring& filename)
{
	sprite_data_.sprite_name_ = filename;
	return sr_->GetSurfaceReference(filename);
}

void Sprite::GetSetSRVFromSpriteResources()
{
	// sprite name must be initialized first
	assert(sprite_data_.sprite_name_ != L"");
	sprite_data_.srv_sprite_ = sr_->GetSRVReference(sprite_data_.sprite_name_);
}
