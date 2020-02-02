#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Base/Graphics.h"
#include "ConstantBuffers.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include <memory>
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include <vector>
#include "VertexBuffer.h"
#include "VertexShader.h"

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
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
		};

		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("models\\suzanne.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];

		std::vector<Vertex> vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back({
				{ pMesh->mVertices[i].x /** scale*/,pMesh->mVertices[i].y /** scale*/,pMesh->mVertices[i].z /** scale*/ },
				*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])
				});
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

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
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

		AddBind(std::make_unique<TransformCbuf>(graphics_, *this));
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

};