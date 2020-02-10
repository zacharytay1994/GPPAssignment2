#include "ResourceLibrary.h"
#include <assert.h>

ResourceLibrary::ResourceLibrary(std::shared_ptr<Graphics> gfx)
	:
	gfx(gfx)
{
}

void ResourceLibrary::Initialize()
{
	GenPosTexCube();
	GenPosNormTexCube();
}

void ResourceLibrary::AddPosTexModel(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile)
{
	std::vector<PosTex> vs_data;
	Assimp::Importer importer;

	// create and map Vertex and Index Buffers
	vi_buffer_map.emplace(mapkey, VIBuffer());

	// read model into aiMesh*
	const auto pModel = importer.ReadFile(objfile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs
	);
	const auto pMesh = pModel->mMeshes[0];

	// check if has vertices, normals, and texturecoords
	assert(pMesh->HasPositions());
	assert(pMesh->HasTextureCoords(0));

	// push back vertices
	int size = pMesh->mNumVertices;
	vs_data.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		vs_data.push_back(
			{
				{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z },
				{ pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y }
			});
	}

	// fill created v_buffer in map
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_description.MiscFlags = 0u;
	buffer_description.ByteWidth = vs_data.size() * sizeof(PosTex);
	buffer_description.StructureByteStride = sizeof(PosTex);
	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = &(vs_data[0]);

	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &vi_buffer_map[mapkey].p_v_buffer_);

	// read indices from mesh
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

	vi_buffer_map[mapkey].index_count_ = indices.size();

	// fill created i_buffer in map
	D3D11_BUFFER_DESC index_buffer_desc = {};
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.CPUAccessFlags = 0u;
	index_buffer_desc.MiscFlags = 0u;
	index_buffer_desc.ByteWidth = indices.size() * sizeof(unsigned short);
	index_buffer_desc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA index_subresource_data = {};				// subresource used to create the buffer
	index_subresource_data.pSysMem = &(indices[0]);

	gfx->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &vi_buffer_map[mapkey].p_i_buffer_);

	// create shader resource view for texture
	CreateShaderResourceView(texturefile, mapkey);
}

void ResourceLibrary::AddPosNormTexModel(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile)
{
	std::vector<PosNormTex> vs_data;
	Assimp::Importer importer;

	// create and map Vertex and Index Buffers
	vi_buffer_map.emplace(mapkey, VIBuffer());

	// read model into aiMesh*
	const auto pModel = importer.ReadFile(objfile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs
	);
	const auto pMesh = pModel->mMeshes[0];

	// check if has vertices, normals, and texturecoords
	assert(pMesh->HasPositions());
	assert(pMesh->HasNormals());
	assert(pMesh->HasTextureCoords(0));

	// push back vertices
	int size = pMesh->mNumVertices;
	vs_data.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		vs_data.push_back(
			{ 
				DirectX::XMFLOAT3( pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z ),
				DirectX::XMFLOAT3( -pMesh->mNormals[i].x, -pMesh->mNormals[i].y, pMesh->mNormals[i].z ),
				DirectX::XMFLOAT2( pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y )
			});
	}

	// fill created v_buffer in map
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;		
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;					
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		
	buffer_description.MiscFlags = 0u;	
	buffer_description.ByteWidth = vs_data.size() * sizeof(PosNormTex);		
	buffer_description.StructureByteStride = sizeof(PosNormTex);	
	D3D11_SUBRESOURCE_DATA subresource_data = {};					
	subresource_data.pSysMem = &(vs_data[0]);							

	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &vi_buffer_map[mapkey].p_v_buffer_);
	
	// read indices from mesh
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

	vi_buffer_map[mapkey].index_count_ = indices.size();

	// fill created i_buffer in map
	D3D11_BUFFER_DESC index_buffer_desc = {};
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.CPUAccessFlags = 0u;
	index_buffer_desc.MiscFlags = 0u;
	index_buffer_desc.ByteWidth = indices.size() * sizeof(unsigned short);
	index_buffer_desc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA index_subresource_data = {};				// subresource used to create the buffer
	index_subresource_data.pSysMem = &(indices[0]);

	gfx->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &vi_buffer_map[mapkey].p_i_buffer_);

	// create shader resource view for texture
	CreateShaderResourceView(texturefile, mapkey);
}

void ResourceLibrary::GenPosTexCube()
{
	// create and map Vertex and Index Buffers
	vi_buffer_map.emplace("TexturedCube", VIBuffer());

	// cube vertices and texture coordinates
	PosTex vs_data[] = {
		// Back face
		{{1.0f, -1.0f, -1.0f}, {2.0f / 3.0f, 0.0f}},
		{{-1.0f, -1.0f, -1.0f}, {1.0f / 3.0f, 0.0f}},
		{{1.0f, 1.0f, -1.0f}, {2.0f / 3.0f, 0.25f}},
		{{-1.0f, 1.0f, -1.0f}, {1.0f / 3.0f, 0.25f}},

		// Front face
		{{-1.0f, -1.0f, 1.0f}, {1.0f / 3.0f, 0.75f}},
		{{1.0f, -1.0f, 1.0f}, {2.0f / 3.0f, 0.75f}},
		{{-1.0f, 1.0f, 1.0f}, {1.0f / 3.0f, 0.5f}},
		{{1.0f, 1.0f, 1.0f}, {2.0f / 3.0f, 0.5f}},

		// Right face
		{{1.0f, -1.0f, 1.0f}, {1.0f, 0.5f}},
		{{1.0f, -1.0f, -1.0f}, {1.0f, 0.25f}},
		{{1.0f, 1.0f, 1.0f}, {2.0f / 3.0f, 0.5f}},
		{{1.0f, 1.0f, -1.0f}, {2.0f / 3.0f, 0.25f}},

		// Left face
		{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.25f}},
		{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.5f}},
		{{-1.0f, 1.0f, -1.0f}, {1.0f / 3.0f, 0.25f}},
		{{-1.0f, 1.0f, 1.0f}, {1.0f / 3.0f, 0.5f}},

		// Top face
		{{-1.0f, 1.0f, 1.0f}, {1.0f / 3.0f, 0.5f}},
		{{1.0f, 1.0f, 1.0f}, {2.0f / 3.0f, 0.5f}},
		{{-1.0f, 1.0f, -1.0f}, {1.0f / 3.0f, 0.25f}},
		{{1.0f, 1.0f, -1.0f}, {2.0f / 3.0f, 0.25f}},

		// Bottom face
		{{-1.0f, -1.0f, -1.0f}, {1.0f / 3.0f, 1.0f}},
		{{1.0f, -1.0f, -1.0f}, {2.0f / 3.0f, 1.0f}},
		{{-1.0f, -1.0f, 1.0f}, {1.0f / 3.0f, 0.75f}},
		{{1.0f, -1.0f, 1.0f}, {2.0f / 3.0f, 0.75f}}
	};

	// fill created v_buffer in map
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_description.MiscFlags = 0u;
	buffer_description.ByteWidth = sizeof(vs_data);
	buffer_description.StructureByteStride = sizeof(PosTex);
	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = vs_data;

	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &vi_buffer_map["TexturedCube"].p_v_buffer_);

	// create indices
	const unsigned short indices[] = {
		0,1,2, 2,1,3,
		4,5,6, 6,5,7,
		8,9,10, 10,9,11,
		12,13,14, 14,13,15,
		16,17,18, 18,17,19,
		20,21,22, 22,21,23
	};

	// fill created i_buffer in map
	D3D11_BUFFER_DESC index_buffer_desc = {};
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	index_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	index_buffer_desc.MiscFlags = 0u;
	index_buffer_desc.ByteWidth = sizeof(indices);
	index_buffer_desc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA index_subresource_data = {};			
	index_subresource_data.pSysMem = indices;

	gfx->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &vi_buffer_map["TexturedCube"].p_i_buffer_);

	vi_buffer_map["TexturedCube"].index_count_ = 36;
}

void ResourceLibrary::GenPosNormTexCube()
{
	// create and map Vertex and Index Buffers
	vi_buffer_map.emplace("TexturedNormCube", VIBuffer());

	// cube vertices and texture coordinates
	PosNormTex vs_data[] = {
		// Back face
		{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {2.0f / 3.0f, 0.0f}},
		{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f / 3.0f, 0.0f}},
		{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {2.0f / 3.0f, 0.25f}},
		{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f / 3.0f, 0.25f}},

		// Front face
		{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f / 3.0f, 0.75f}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {2.0f / 3.0f, 0.75f}},
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f / 3.0f, 0.5f}},
		{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {2.0f / 3.0f, 0.5f}},

		// Right face
		{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f}},
		{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.25f}},
		{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {2.0f / 3.0f, 0.5f}},
		{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {2.0f / 3.0f, 0.25f}},

		// Left face
		{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.25f}},
		{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.5f}},
		{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f / 3.0f, 0.25f}},
		{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f / 3.0f, 0.5f}},

		// Top face
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f / 3.0f, 0.5f}},
		{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {2.0f / 3.0f, 0.5f}},
		{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f / 3.0f, 0.25f}},
		{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {2.0f / 3.0f, 0.25f}},

		// Bottom face
		{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f / 3.0f, 1.0f}},
		{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {2.0f / 3.0f, 1.0f}},
		{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f / 3.0f, 0.75f}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {2.0f / 3.0f, 0.75f}}
	};

	// fill created v_buffer in map
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_description.MiscFlags = 0u;
	buffer_description.ByteWidth = sizeof(vs_data);
	buffer_description.StructureByteStride = sizeof(PosNormTex);
	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = vs_data;

	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &vi_buffer_map["TexturedNormCube"].p_v_buffer_);

	// create indices
	const unsigned short indices[] = {
		0,1,2, 2,1,3,
		4,5,6, 6,5,7,
		8,9,10, 10,9,11,
		12,13,14, 14,13,15,
		16,17,18, 18,17,19,
		20,21,22, 22,21,23
	};

	// fill created i_buffer in map
	D3D11_BUFFER_DESC index_buffer_desc = {};
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	index_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	index_buffer_desc.MiscFlags = 0u;
	index_buffer_desc.ByteWidth = sizeof(indices);
	index_buffer_desc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA index_subresource_data = {};
	index_subresource_data.pSysMem = indices;

	gfx->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &vi_buffer_map["TexturedNormCube"].p_i_buffer_);

	vi_buffer_map["TexturedNormCube"].index_count_ = 36;
}

void ResourceLibrary::AddCubeTexture(const std::string& mapkey, const std::wstring& texturefile)
{
	CreateShaderResourceView(texturefile, mapkey);
}

void ResourceLibrary::DrawModel(const std::string& key, const DirectX::XMMATRIX& transform)
{
	gfx->SetUseType(ShaderType::Textured);
	gfx->BindVertexBufferStride(vi_buffer_map[key].p_v_buffer_, 20u);
	gfx->BindIndexBuffer(vi_buffer_map[key].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, DirectX::XMMatrixIdentity() });
	gfx->DrawIndexed(vi_buffer_map[key].index_count_);
}

void ResourceLibrary::DrawModelNorm(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model)
{
	gfx->SetUseType(ShaderType::TexturedNormal);
	gfx->BindVertexBufferStride(vi_buffer_map[key].p_v_buffer_, 32u);
	gfx->BindIndexBuffer(vi_buffer_map[key].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, model });
	gfx->DrawIndexed(vi_buffer_map[key].index_count_);
}

void ResourceLibrary::DrawTexturedCube(const std::string& key, const DirectX::XMMATRIX& transform)
{
	gfx->SetUseType(ShaderType::Textured);
	gfx->BindVertexBufferStride(vi_buffer_map["TexturedCube"].p_v_buffer_, 20u);
	gfx->BindIndexBuffer(vi_buffer_map["TexturedCube"].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, DirectX::XMMatrixIdentity() });
	gfx->DrawIndexed(vi_buffer_map["TexturedCube"].index_count_);
}

void ResourceLibrary::DrawTexturedCubeNorm(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model)
{
	gfx->SetUseType(ShaderType::TexturedNormal);
	gfx->BindVertexBufferStride(vi_buffer_map["TexturedNormCube"].p_v_buffer_, 32u);
	gfx->BindIndexBuffer(vi_buffer_map["TexturedNormCube"].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, model });
	gfx->DrawIndexed(vi_buffer_map["TexturedNormCube"].index_count_);
}

void ResourceLibrary::CreateShaderResourceView(const std::wstring& texturefile, const std::string& mapkey)
{
	// create surface image resource
	Surface image_resource(texturefile);

	// create new srv in map
	ID3D11ShaderResourceView* srv = nullptr;
	srv_map.emplace(mapkey, srv);

	// fill srv with surface
	D3D11_TEXTURE2D_DESC texture_description = {};
	texture_description.Width = image_resource.GetWidth();
	texture_description.Height = image_resource.GetHeight();
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
	subresource_data.pSysMem = image_resource.GetBufferPointer();
	subresource_data.SysMemPitch = image_resource.GetWidth() * sizeof(Color);
	ID3D11Texture2D* p_texture;
	gfx->GetDevice().CreateTexture2D(&texture_description, &subresource_data, &p_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_descriptor = {};
	srv_descriptor.Format = texture_description.Format;
	srv_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_descriptor.Texture2D.MostDetailedMip = 0;
	srv_descriptor.Texture2D.MipLevels = 1;
	if (p_texture != 0) {
		gfx->GetDevice().CreateShaderResourceView(p_texture, &srv_descriptor, &srv_map[mapkey]);
	}
	p_texture->Release();
}

