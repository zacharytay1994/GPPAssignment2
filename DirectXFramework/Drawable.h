#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Base/Graphics.h"
#include "Base/Input.h"
#include "D3dcompiler.h"
#include "Entity.h"
#include <memory>
#include <vector>
#include <wrl.h>

class Drawable : public Entity
{
private:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;

	Vecf3 position_ = Vecf3(0.0f, 0.0f, 0.0f);

public:
	Drawable(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> in)
		:
		graphics_(gfx),
		input_(in)
	{
		// Create & set buffers
		ID3DBlob* p_blob;						// binary large object, i.e. some data
		D3DReadFileToBlob(L"Shaders/ModelPixelShader.cso", &p_blob);
		graphics_->GetDevice().CreatePixelShader(
			p_blob->GetBufferPointer(),			// pointer to compiled shader 
			p_blob->GetBufferSize(),			// size of compiled shader
			nullptr,							// ignore: no class linkage
			&p_pixel_shader_					// ignore: address pointer to pixel shader
		);

		D3DReadFileToBlob(L"Shaders/ModelVertexShader.cso", &p_blob);
		graphics_->GetDevice().CreateVertexShader(
			p_blob->GetBufferPointer(),			 // same as pixel shader
			p_blob->GetBufferSize(),			 // same as pixel shader
			nullptr,							 // same as pixel shader
			&p_vertex_shader_					 // same as pixel shader
		);
	}

	void SetPosition(const Vecf3& position)
	{
		position_ = position;
	}

	void Render(const float& dt = 0) override
	{
		// Set shaders
		graphics_->GetContext().VSSetShader(
			p_vertex_shader_,					 // same as pixel shader
			nullptr,							 // same as pixel shader
			0u									 // same as pixel shader
		);

		graphics_->GetContext().PSSetShader(
			p_pixel_shader_,					// pointer to pixel shader
			nullptr,							// ignore: null no class instance
			0u									// ignore: 0 class instances interfaces
		);

		// Bind vertices
		graphics_->BindVertexBuffer(p_vertex_buffer_);
		// Bind indices
		graphics_->BindIndicesBuffer(p_index_buffer_);
		graphics_->UpdateCBTransformSubresource({ GetTransform(0) });
		graphics_->DrawIndexed(index_count_);
	}

	DirectX::XMMATRIX GetTransform(const float& dt)
	{
		// no scaling by 0
		//assert(cube_data_.scale_x_ != 0.0f || cube_data_.scale_y_ != 0.0f || cube_data_.scale_z_ != 0.0f);
		return DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(temp_scale_, temp_scale_, temp_scale_) *
			DirectX::XMMatrixRotationRollPitchYaw(temp_x, 0, 0) *
			/*dx::XMMatrixRotationZ(cube_data_.angle_z) *
			dx::XMMatrixRotationX(cube_data_.angle_x) *
			dx::XMMatrixRotationY(cube_data_.angle_y) **/
			DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z) *
			input_->GetCameraMatrix(dt) *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, 1000.0f)
		);
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		 	   DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			   DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
protected:
	// temp
	float temp_x = 0.0f;
	float temp_scale_ = 1.0f;
	float temp_pos_x_ = 0.0f;
	float temp_pos_y_ = 0.0f;

	// buffers
	ID3D11Buffer* p_vertex_buffer_ = nullptr;
	ID3D11Buffer* p_index_buffer_ = nullptr;

	// shaders
	ID3D11VertexShader* p_vertex_shader_ = nullptr;
	ID3D11PixelShader* p_pixel_shader_ = nullptr;

	int index_count_ = 0;

	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};

class TestObject : public Drawable {
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tx;
	};
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	ID3D11Buffer* p_vertex_buffer_ = nullptr;
	ID3D11Buffer* p_index_buffer_ = nullptr;

	ID3D11ShaderResourceView* srv_sprite_ = nullptr;	// pointer to sprite image resource ready for pixel shader sampling
	Surface			image_resource_;

public:
	float temp_x = 0.0f;
	float temp_scale_ = 1.0f;
	float temp_pos_x_ = 0.0f;
	float temp_pos_y_ = 0.0f;
	int index_count_ = 0;
	
	/*std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned short> indices;*/

public:
	TestObject(std::shared_ptr<Graphics> graphics_, std::shared_ptr<Input> input_, DirectX::XMFLOAT3 material, const std::string& objfile, const std::wstring& texture)
		:
		Drawable(graphics_, input_),
		graphics_(graphics_),
		input_(input_),
		image_resource_(texture)
	{
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile(objfile,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_FlipUVs
		);
		const auto pMesh = pModel->mMeshes[0];

		int size = pMesh->mNumVertices;
		vertices.reserve(pMesh->mNumVertices);
		//int sizetx = (pMesh->mTextureCoords[0]).size();
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back(
				{ { pMesh->mVertices[i].x /** scale*/,pMesh->mVertices[i].y /** scale*/,pMesh->mVertices[i].z /** scale*/ },
				{ pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y }
				});
				//{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale }
				///**reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])*/
				//);
		}

		D3D11_BUFFER_DESC buffer_description = {};
		buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// how buffer will be bound to pipeline, i.e. type
		buffer_description.Usage = D3D11_USAGE_DYNAMIC;					// how buffer is expected to be read from and written to
		buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// cpu access, allow cpu to change buffer content
		buffer_description.MiscFlags = 0u;								// 0 i.e. unused
		buffer_description.ByteWidth = vertices.size() * sizeof(Vertex);				// size of buffer
		buffer_description.StructureByteStride = sizeof(Vertex);	// size of each element in buffer
		D3D11_SUBRESOURCE_DATA subresource_data = {};					// subresource used to create the buffer
		subresource_data.pSysMem = &(vertices[0]);							// pointer to initialization data

		graphics_->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &p_vertex_buffer_);

		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
		index_count_ = pMesh->mNumFaces * 3;
		int test1 = indices.size();
		int test2 = sizeof(unsigned short);
		D3D11_BUFFER_DESC index_buffer_desc = {};
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		index_buffer_desc.CPUAccessFlags = 0u;
		index_buffer_desc.MiscFlags = 0u;
		index_buffer_desc.ByteWidth = indices.size() * sizeof(unsigned short);
		index_buffer_desc.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA index_subresource_data = {};				// subresource used to create the buffer
		index_subresource_data.pSysMem = &(indices[0]);

		graphics_->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &p_index_buffer_);
		/*indices = std::vector<unsigned short>{
		0,1,2, 2,1,3,
		4,5,6, 6,5,7,
		8,9,10, 10,9,11,
		12,13,14, 1,1,1,
		16,17,18, 1,1,1,
		20,21,22, 1, 1, 1
		};*/
		/*for (int i = 0; i < 1000; i++) {
			indices.push_back(0);
		}*/
		//// Set IED
		//graphics_->SetModelIED();

		// Bind vertices
		//graphics_->BindVertexBuffer(p_vertex_buffer_);

		// Bind indices
		//graphics_->BindIndicesBuffer(p_index_buffer_);

		/*AddStaticBind(std::make_unique<VertexBuffer>(graphics_, vertices));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics_, indices));

		auto pvs = std::make_unique<VertexShader>(graphics_, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(graphics_, L"PhongPS.cso"));*/

		// Create Pixel Shader
		/*Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(path.c_str(), &pBlob);
		graphics_GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);*/

		/*
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};

		AddStaticBind(std::make_unique<InputLayout>(graphics_, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(graphics_, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;
		pmc.color = material;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(graphics_, pmc, 1u));

		AddBind(std::make_unique<TransformCbuf>(graphics_, *this));*/
		CreateShaderResourceView();
	}

	void Draw()
	{
		// Bind vertices
		graphics_->BindVertexBuffer(p_vertex_buffer_);
		// Bind indices
		graphics_->BindIndicesBuffer(p_index_buffer_);
		graphics_->BindShaderResourceView(srv_sprite_);
		graphics_->UpdateCBTransformSubresource({ GetTransform(0) });
		graphics_->DrawIndexed(index_count_);
	}
	
	DirectX::XMMATRIX GetTransform(const float& dt)
	{
		// no scaling by 0
		//assert(cube_data_.scale_x_ != 0.0f || cube_data_.scale_y_ != 0.0f || cube_data_.scale_z_ != 0.0f);
		return DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(temp_scale_, temp_scale_, temp_scale_) *
			DirectX::XMMatrixRotationRollPitchYaw(temp_x, 0, 0) *
			/*dx::XMMatrixRotationZ(cube_data_.angle_z) *
			dx::XMMatrixRotationX(cube_data_.angle_x) *
			dx::XMMatrixRotationY(cube_data_.angle_y) **/
			DirectX::XMMatrixTranslation(temp_pos_x_, temp_pos_y_, 10) *
			input_->GetCameraMatrix(dt) *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, 1000.0f)
		);
	}

	void CreateShaderResourceView() {
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
		graphics_->GetDevice().CreateTexture2D(&texture_description, &subresource_data, &p_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_descriptor = {};
		srv_descriptor.Format = texture_description.Format;
		srv_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_descriptor.Texture2D.MostDetailedMip = 0;
		srv_descriptor.Texture2D.MipLevels = 1;
		if (p_texture != 0) {
			graphics_->GetDevice().CreateShaderResourceView(p_texture, &srv_descriptor, &srv_sprite_);
		}
		p_texture->Release();
		graphics_->BindShaderResourceView(srv_sprite_);
	}
};