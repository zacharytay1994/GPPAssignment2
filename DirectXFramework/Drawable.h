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
	}

	void SetPosition(const Vecf3& position)
	{
		position_ = position;
	}

	void Render(const float& dt = 0) override
	{
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
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned short> indices;

public:
	TestObject(std::shared_ptr<Graphics> graphics_, std::shared_ptr<Input> input_, const std::string& objfile, const float& scale = 1.0f)
		:
		Drawable(graphics_, input_)
	{
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile(objfile,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];

		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back(
				{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale }
				/**reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])*/
				);
		}

		D3D11_BUFFER_DESC buffer_description = {};
		buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// how buffer will be bound to pipeline, i.e. type
		buffer_description.Usage = D3D11_USAGE_DYNAMIC;					// how buffer is expected to be read from and written to
		buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// cpu access, allow cpu to change buffer content
		buffer_description.MiscFlags = 0u;								// 0 i.e. unused
		buffer_description.ByteWidth = vertices.size() * sizeof(DirectX::XMFLOAT3);				// size of buffer
		buffer_description.StructureByteStride = sizeof(DirectX::XMFLOAT3);	// size of each element in buffer
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
	}

};