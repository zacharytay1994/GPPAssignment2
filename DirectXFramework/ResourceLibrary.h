#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Base/Graphics.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

struct PosTex {
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT2 texture_coords_;
};

struct PosNormTex {
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT3 normal_;
	DirectX::XMFLOAT2 texture_coords_;
};

struct PosNorm {
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT3 normal_;
};

struct VIBuffer {
public:
	ID3D11Buffer* p_v_buffer_;
	ID3D11Buffer* p_i_buffer_;
	int index_count_;
};

class ResourceLibrary {
private:
	std::shared_ptr<Graphics> gfx;
	std::unordered_map<std::string, VIBuffer> vi_buffer_map;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> srv_map;
public:
	ResourceLibrary(std::shared_ptr<Graphics> gfx);
	void Initialize();
	/*____________________________________*/
	// MESH LOADING FUNCTIONS
	/*____________________________________*/
	// loads a .obj file with Positions and Texture Coordinates
	void AddPosTexModel(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile);
	// loads a .obj file with Positions, Normals, and Texture Coordinates
	void AddPosNormTexModel(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile);
private:
	// loads a cube mesh with Positions and Texture Coordinates
	void GenPosTexCube();
	void GenPosNormTexCube();
	// loads a plane mesh with Positions and Texture Coordinates
	void GenPosTexPlane();
public:
	// loads a texture to wrap a cube
	void AddCubeTexture(const std::string& mapkey, const std::wstring& texturefile);
	/*____________________________________*/
	// DRAWING FUNCTIONS
	/*____________________________________*/
	void DrawModel(const std::string& key, const DirectX::XMMATRIX& transform);												// draws textured model with no normals
	void DrawModelNorm(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model);			// draws textured model with normals
	void DrawTexturedCube(const std::string& key, const DirectX::XMMATRIX& transform);										// draws textured cube with no normals
	void DrawTexturedCubeNorm(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model);	// draws textured cube with normals
	void DrawTexturedPlane(const std::string& key, const DirectX::XMMATRIX& transform);
private:
	void CreateShaderResourceView(const std::wstring& texturefile, const std::string& mapkey);
};