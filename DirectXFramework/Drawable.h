#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Base/Graphics.h"
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


class Drawable : public Entity
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

	void Render(const float& dt) override
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
public:
	TestObject(std::shared_ptr<Graphics> graphics_, DirectX::XMFLOAT3 material)
		:
		Drawable(graphics_, material)
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};

		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("Models\\suzanne.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];

		std::vector<DirectX::XMFLOAT3> vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back(
				{ pMesh->mVertices[i].x /** scale*/,pMesh->mVertices[i].y /** scale*/,pMesh->mVertices[i].z /** scale*/ }
				/**reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])*/
				);
		}

		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddStaticBind(std::make_unique<VertexBuffer>(graphics_, vertices));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics_, indices));

		auto pvs = std::make_unique<VertexShader>(graphics_, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(graphics_, L"PhongPS.cso"));

		// Create Pixel Shader
		/*Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(path.c_str(), &pBlob);
		graphics_GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);*/

		/*const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
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
	}
};