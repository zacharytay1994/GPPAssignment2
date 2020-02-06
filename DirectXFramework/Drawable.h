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

class Bindable
{
public:
	virtual void Bind(std::shared_ptr<Graphics> gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(std::shared_ptr<Graphics> gfx) noexcept
	{
		return &(gfx->GetContext());
	}
	static ID3D11Device* GetDevice(std::shared_ptr<Graphics> gfx) noexcept
	{
		return&(gfx->GetDevice());
	}
};

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(std::shared_ptr<Graphics> gfx, const std::vector<V>& vertices)
		:
		stride(sizeof(V))
	{
		/*INFOMAN(gfx);*/

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		const UINT offset = 0u;
		ID3D11Buffer* tmp = &(gfx->GetVB());
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, &tmp, &stride, &offset);
	}
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(std::shared_ptr<Graphics> gfx, const std::vector<unsigned short>& indices)
		:
		count((UINT)indices.size())
	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(count * sizeof(unsigned short));
		ibd.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
	}
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}
	UINT GetCount() const noexcept
	{
		return count;
	}
protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};

class VertexShader : public Bindable
{
public:
	VertexShader(std::shared_ptr<Graphics> gfx, const std::wstring& path)
	{
		D3DReadFileToBlob(path.c_str(), &pBytecodeBlob);
		GetDevice(gfx)->CreateVertexShader(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		);
	}
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}
	ID3DBlob* GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};

class PixelShader : public Bindable
{
public:
	PixelShader(std::shared_ptr<Graphics> gfx, const std::wstring& path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(path.c_str(), &pBlob);
		GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	}
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};

class InputLayout : public Bindable
{
public:
	InputLayout(std::shared_ptr<Graphics> gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode)
	{
	}

	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

class Topology : public Bindable
{
public:
	Topology(std::shared_ptr<Graphics> gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		:
		type(type)
	{}
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		GetContext(gfx)->IASetPrimitiveTopology(type);
	}
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(std::shared_ptr<Graphics> gfx, const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(std::shared_ptr<Graphics> gfx, const C& consts, UINT slot = 0u)
		:
		slot(slot)
	{
		/*INFOMAN(gfx);*/

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}
	ConstantBuffer(std::shared_ptr<Graphics> gfx, UINT slot = 0u)
		:
		slot(slot)
	{
		/*INFOMAN(gfx);*/

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		Bindable::GetContext(gfx)->PSSetConstantBuffers(ConstantBuffer<C>::slot, 1u, ConstantBuffer<C>::pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		Bindable::GetContext(gfx)->VSSetConstantBuffers(ConstantBuffer<C>::slot, 1u, ConstantBuffer<C>::pConstantBuffer.GetAddressOf());
	}
};


class Drawable
{
private:
	std::shared_ptr<Graphics> graphics_;
	static std::vector<std::unique_ptr<Bindable>> staticBinds_;
	const class IndexBuffer* pIndexBuffer_ = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds_;

public:
	Drawable(std::shared_ptr<Graphics> gfx, DirectX::XMFLOAT3 material)
		:
		graphics_(gfx)
	{
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		staticBinds_.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
	{
		pIndexBuffer_ = ibuf.get();
		staticBinds_.push_back(std::move(ibuf));
	}

	void AddBind(std::unique_ptr<Bindable> bind)
	{
		binds_.push_back(std::move(bind));
	}

	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBinds_;
	}

	void Draw()
	{
		for (auto& b : binds_)
		{
			b->Bind(graphics_);
		}

		for (auto& b : GetStaticBinds())
		{
			b->Bind(graphics_);
		}

		graphics_->DrawIndexed();
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		 	   DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			   DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
protected:
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

class TransformCbuf : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
public:
	TransformCbuf(std::shared_ptr<Graphics> gfx, Drawable& parent, UINT slot = 0u)
		:
		parent(parent)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override
	{
		auto modelView = parent.GetTransformXM() /** gfx.GetCamera()*/;
		const Transforms tf =
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView /**
				gfx.GetProjection()*/
			)
		};
		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	Drawable& parent;
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
	TestObject(std::shared_ptr<Graphics> graphics_, std::shared_ptr<Input> input_, DirectX::XMFLOAT3 material, const std::string& objfile, const std::wstring& texture)
		:
		Drawable(graphics_, material),
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
		//vertices = std::vector<DirectX::XMFLOAT3> {
		//	// Back face
		//	{10.0f, -1.0f, -1.0f},
		//	{-1.0f, -1.0f, -1.0f},
		//	{1.0f, 1.0f, -1.0f},
		//	{-1.0f, 1.0f, -1.0f},

		//	// Front face
		//	{-1.0f, -1.0f, 1.0f},
		//	{1.0f, -1.0f, 1.0f},
		//	{-1.0f, 1.0f, 1.0f},
		//	{1.0f, 1.0f, 1.0f},

		//	// Right face
		//	{1.0f, -1.0f, 1.0f},
		//	{1.0f, -1.0f, -1.0f},
		//	{1.0f, 1.0f, 1.0f},
		//	{1.0f, 1.0f, -1.0f},

		//	// Left face
		//	{-1.0f, -1.0f, -1.0f},
		//	{-1.0f, -1.0f, 1.0f},
		//	{-1.0f, 1.0f, -1.0f},
		//	{-1.0f, 1.0f, 1.0f},

		//	// Top face
		//	{-1.0f, 1.0f, 1.0f},
		//	{1.0f, 1.0f, 1.0f},
		//	{-1.0f, 1.0f, -1.0f},
		//	{1.0f, 1.0f, -1.0f},

		//	// Bottom face
		//	{-1.0f, -1.0f, -1.0f},
		//	{1.0f, -1.0f, -1.0f},
		//	{-1.0f, -1.0f, 1.0f},
		//	{1.0f, -1.0f, 1.0f}
		//};
		/*for (int i = 0; i < 1000; i++) {
			vertices.push_back({ 1.0f, 1.0f, 1.0f });
		}*/

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