#include "ResourceLibrary.h"
#include "Base/Input.h"
#include <assert.h>
#include <limits>

ResourceLibrary::ResourceLibrary(std::shared_ptr<Graphics> gfx)
	:
	gfx(gfx)
{
}

void ResourceLibrary::Initialize()
{
	GenPosTexCube();
	GenPosNormTexCube();
	GenPosTexPlane();
	GenPosNormCube();
}

Vecf3 ResourceLibrary::GetDimensions(const std::string& key)
{
	return dimensions[key];
}

DirectX::XMMATRIX ResourceLibrary::GetTransform(const Vecf3& pos, const Vecf3& scale, const Vecf3& rotation, std::shared_ptr<Input> input)
{
	// no scaling by 0
	return	DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
		input->GetCameraMatrix() *
		DirectX::XMMatrixPerspectiveLH(1.0f, (float)Graphics::viewport_height_ / (float)Graphics::viewport_width_, 0.5f, 1000.0f));
}

DirectX::XMMATRIX ResourceLibrary::GetModelTransform(const Vecf3& pos, const Vecf3& scale, const Vecf3& rotation)
{
	// no scaling by 0
	return	DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
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

	float min_x = (std::numeric_limits<float>::max)();
	float max_x = (std::numeric_limits<float>::min)();
	float min_y = (std::numeric_limits<float>::max)();
	float max_y = (std::numeric_limits<float>::min)();
	float min_z = (std::numeric_limits<float>::max)();
	float max_z = (std::numeric_limits<float>::min)();
	// push back vertices
	int size = pMesh->mNumVertices;
	vs_data.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		min_x = pMesh->mVertices[i].x < min_x ? pMesh->mVertices[i].x : min_x;
		min_y = pMesh->mVertices[i].y < min_y ? pMesh->mVertices[i].y : min_y;
		min_z = pMesh->mVertices[i].z < min_z ? pMesh->mVertices[i].z : min_z;

		max_x = pMesh->mVertices[i].x > max_x ? pMesh->mVertices[i].x : max_x;
		max_y = pMesh->mVertices[i].y > max_y ? pMesh->mVertices[i].y : max_y;
		max_z = pMesh->mVertices[i].z > max_z ? pMesh->mVertices[i].z : max_z;
		vs_data.push_back(
			{
				{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z },
				{ pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y }
			});
	}
	dimensions[mapkey] = { max_x - min_x, max_y - min_y, max_z - min_z };

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

	float min_x = (std::numeric_limits<float>::max)();
	float max_x = (std::numeric_limits<float>::min)();
	float min_y = (std::numeric_limits<float>::max)();
	float max_y = (std::numeric_limits<float>::min)();
	float min_z = (std::numeric_limits<float>::max)();
	float max_z = (std::numeric_limits<float>::min)();

	// push back vertices
	int size = pMesh->mNumVertices;
	vs_data.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		min_x = pMesh->mVertices[i].x < min_x ? pMesh->mVertices[i].x : min_x;
		min_y = pMesh->mVertices[i].y < min_y ? pMesh->mVertices[i].y : min_y;
		min_z = pMesh->mVertices[i].z < min_z ? pMesh->mVertices[i].z : min_z;

		max_x = pMesh->mVertices[i].x > max_x ? pMesh->mVertices[i].x : max_x;
		max_y = pMesh->mVertices[i].y > max_y ? pMesh->mVertices[i].y : max_y;
		max_z = pMesh->mVertices[i].z > max_z ? pMesh->mVertices[i].z : max_z;

		vs_data.push_back(
			{
				DirectX::XMFLOAT3( pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z ),
				DirectX::XMFLOAT3( pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z ),
				DirectX::XMFLOAT2( pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y )
			});
	}

	dimensions[mapkey] = { max_x - min_x, max_y - min_y, max_z - min_z };

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

void ResourceLibrary::AddPosNormModel(const std::string& mapkey, const std::string& objfile)
{
	std::vector<PosNorm> vs_data;
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

	// push back vertices
	int size = pMesh->mNumVertices;
	vs_data.reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		vs_data.push_back(
			{
				DirectX::XMFLOAT3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z),
				DirectX::XMFLOAT3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z)
			});
	}

	// fill created v_buffer in map
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_description.MiscFlags = 0u;
	buffer_description.ByteWidth = vs_data.size() * sizeof(PosNorm);
	buffer_description.StructureByteStride = sizeof(PosNorm);
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

void ResourceLibrary::GenPosTexPlane()
{
	// create and map Vertex and Index Buffers
	vi_buffer_map.emplace("TexturedPlane", VIBuffer());

	// cube vertices and texture coordinates
	PosTex vs_data[] = {
		// Front face
		{{-1.0f, -1.0f, 0.5f}, {0.0f, 1.0f}},
		{{1.0f, -1.0f, 0.5f}, {1.0f, 1.0f}},
		{{-1.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
		{{1.0f, 1.0f, 0.5f}, {1.0f, 0.0f}}
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

	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &vi_buffer_map["TexturedPlane"].p_v_buffer_);
	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &p_v_sprite_buffer_);

	// create indices
	const unsigned short indices[] = {
		1,0,2, 1,2,3
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

	gfx->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &vi_buffer_map["TexturedPlane"].p_i_buffer_);

	vi_buffer_map["TexturedPlane"].index_count_ = 6;
}

void ResourceLibrary::GenPosNormCube()
{
	// create and map Vertex and Index Buffers
	vi_buffer_map.emplace("TexturedCube", VIBuffer());

	// cube vertices and texture coordinates
	PosNorm vs_data[] = {
		// Back face
		{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
		{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
		{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},
		{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}},

		// Front face
		{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

		// Right face
		{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
		{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},

		// Left face
		{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

		// Top face
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
		{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},

		// Bottom face
		{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
		{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}
	};

	// fill created v_buffer in map
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_description.MiscFlags = 0u;
	buffer_description.ByteWidth = sizeof(vs_data);
	buffer_description.StructureByteStride = sizeof(PosNorm);
	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = vs_data;

	gfx->GetGraphicsDevice()->CreateBuffer(&buffer_description, &subresource_data, &vi_buffer_map["UntexturedCube"].p_v_buffer_);

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

	gfx->GetGraphicsDevice()->CreateBuffer(&index_buffer_desc, &index_subresource_data, &vi_buffer_map["UntexturedCube"].p_i_buffer_);

	vi_buffer_map["UntexturedCube"].index_count_ = 36;
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

void ResourceLibrary::DrawUntexturedCubeNorm(const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model, const DirectX::XMFLOAT4& colour)
{
	gfx->SetUseType(ShaderType::UntexturedNormal);
	gfx->BindVertexBufferStride(vi_buffer_map["UntexturedCube"].p_v_buffer_, 24u);
	gfx->BindIndexBuffer(vi_buffer_map["UntexturedCube"].p_i_buffer_);
	gfx->UpdateCBTransformSubresource({ transform, model });
	gfx->UpdateCBColourSubresource({colour.x, colour.y, colour.z, colour.w});
	gfx->DrawIndexed(vi_buffer_map["UntexturedCube"].index_count_);
}

void ResourceLibrary::DrawTexturedPlane(const std::string& key, const DirectX::XMMATRIX& transform)
{
	gfx->SetUseType(ShaderType::Textured);
	gfx->BindVertexBufferStride(vi_buffer_map["TexturedPlane"].p_v_buffer_, 20u);
	gfx->BindIndexBuffer(vi_buffer_map["TexturedPlane"].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, DirectX::XMMatrixIdentity() });
	gfx->UpdateCBColourSubresource({ 0.0f, 0.0f, 0.0f, 0.0f });
	gfx->DrawIndexed(vi_buffer_map["TexturedPlane"].index_count_);
}

void ResourceLibrary::DrawTexturedPlane(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMFLOAT4& color)
{
	gfx->SetUseType(ShaderType::Textured);
	gfx->BindVertexBufferStride(vi_buffer_map["TexturedPlane"].p_v_buffer_, 20u);
	gfx->BindIndexBuffer(vi_buffer_map["TexturedPlane"].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, DirectX::XMMatrixIdentity() });
	gfx->UpdateCBColourSubresource({ color.x, color.y, color.z, color.w });
	gfx->DrawIndexed(vi_buffer_map["TexturedPlane"].index_count_);
}

void ResourceLibrary::DrawTexturedPlaneSlice(const std::string& key, const DirectX::XMMATRIX& transform, const int& columns, const int& rows, const int& index)
{
	float column_spacing = 1.0f / columns;
	float row_spacing = 1.0f / rows;
	float y_offset = (int)(index / columns);
	float x_offset = (index - 1) % columns;
	float y_top = y_offset * row_spacing;
	float y_bottom = y_offset * row_spacing + row_spacing;
	float x_left = x_offset * column_spacing;
	float x_right = x_offset * column_spacing + column_spacing;

	// slice up texture base don texture coordinates
	PosTex vs_data[] = {
		// Front face
		{{-1.0f, -1.0f, 0.5f}, {x_left, y_bottom}},
		{{1.0f, -1.0f, 0.5f}, {x_right, y_bottom}},
		{{-1.0f, 1.0f, 0.5f}, {x_left, y_top}},
		{{1.0f, 1.0f, 0.5f}, {x_right, y_top}}
	};
	BindPosTexVBSubresource(vs_data);
	gfx->SetUseType(ShaderType::Textured);
	gfx->BindVertexBufferStride(p_v_sprite_buffer_, 20u);
	gfx->BindIndexBuffer(vi_buffer_map["TexturedPlane"].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, DirectX::XMMatrixIdentity() });
	gfx->DrawIndexed(vi_buffer_map["TexturedPlane"].index_count_);
}

void ResourceLibrary::DrawUnTexturedModelNorm(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model)
{
	gfx->SetUseType(ShaderType::UntexturedNormal);
	gfx->BindVertexBufferStride(vi_buffer_map[key].p_v_buffer_, 24u);
	gfx->BindIndexBuffer(vi_buffer_map[key].p_i_buffer_);
	gfx->BindShaderResourceView(srv_map[key]);
	gfx->UpdateCBTransformSubresource({ transform, model });
	gfx->DrawIndexed(vi_buffer_map[key].index_count_);
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

void ResourceLibrary::BindPosTexVBSubresource(PosTex* sr)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	gfx->GetGraphicsDeviceContext()->Map(p_v_sprite_buffer_, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_subresource);
	memcpy(mapped_subresource.pData, sr, sizeof(PosTex) * 4);
	gfx->GetGraphicsDeviceContext()->Unmap(p_v_sprite_buffer_, 0u);

	const UINT stride = sizeof(PosTex);
	const UINT offset = 0u;
	gfx->GetGraphicsDeviceContext()->IASetVertexBuffers(
		0u,								   // start slot, multiple vertex buffers can be bound to various slots
		1u,								   // number of buffers
		&p_v_sprite_buffer_,			   // buffer to be bound
		&stride,						   // pointer to array of stride values, i.e. 1 stride for each buffer, pointer to UINT is the same as an array of 1
		&offset							   // pointer to array of offset values, 0u offset for first and only buffer
	);
}

