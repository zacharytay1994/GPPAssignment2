#include "SpriteResources.h"
#include "Base/Surface.h"

SpriteResources::SpriteResources(std::shared_ptr<Graphics>& graphics)
	:
	gfx(graphics)
{
	InitMap();
}

void SpriteResources::InitMap()
{
	for (int i = 0; i < resource_count_; i++) {
		CreateShaderResourceView(surface_array_[i], i);
	}
}

void SpriteResources::CreateShaderResourceView(const Surface& surface, const int& index)
{
	// create shader resource
	if (gfx->Initialized()) {
		D3D11_TEXTURE2D_DESC texture_description = {};
		texture_description.Width = surface.GetWidth();
		texture_description.Height = surface.GetHeight();
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
		subresource_data.pSysMem = surface.GetBufferPointer();
		subresource_data.SysMemPitch = surface.GetWidth() * sizeof(Color);
		ID3D11Texture2D* p_texture;
		gfx->GetDevice().CreateTexture2D(&texture_description, &subresource_data, &p_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_descriptor = {};
		srv_descriptor.Format = texture_description.Format;
		srv_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_descriptor.Texture2D.MostDetailedMip = 0;
		srv_descriptor.Texture2D.MipLevels = 1;
		if (p_texture != 0) {
			gfx->GetDevice().CreateShaderResourceView(p_texture, &srv_descriptor, &(srv_array_[index]));
		}
		p_texture->Release();
	}
}

Surface & SpriteResources::GetSurfaceReference(const std::wstring & filename)
{
	return surface_array_[index_map_[filename]];
}

ID3D11ShaderResourceView *& SpriteResources::GetSRVReference(const std::wstring & filename)
{
	return srv_array_[index_map_[filename]];
}
