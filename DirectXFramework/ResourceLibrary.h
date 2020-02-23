#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Base/Graphics.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class Input;

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
	std::unordered_map<std::string, Vecf3> dimensions;

	// vertex buffer for sprite textures that require sprite frame sampling
	ID3D11Buffer* p_v_sprite_buffer_;
public:
	ResourceLibrary(std::shared_ptr<Graphics> gfx);
	void Initialize();
	Vecf3 GetDimensions(const std::string& key);
	DirectX::XMMATRIX GetTransform(const Vecf3& pos, const Vecf3& scale, const Vecf3& rotation, std::shared_ptr<Input> input);
	DirectX::XMMATRIX GetModelTransform(const Vecf3& pos, const Vecf3& scale, const Vecf3& rotation);
	/*____________________________________*/
	// MESH LOADING FUNCTIONS
	/*____________________________________*/
	// loads a .obj file with Positions and Texture Coordinates
	void AddPosTexModel(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile);
	// loads a .obj file with Positions, Normals, and Texture Coordinates
	void AddPosNormTexModel(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile);
	// loads a .obj file with Positions and Normals
	void AddPosNormModel(const std::string& mapkey, const std::string& objfile);
private:
	// loads a cube mesh with Positions and Texture Coordinates
	void GenPosTexCube();
	void GenPosNormTexCube();
	// loads a plane mesh with Positions and Texture Coordinates
	void GenPosTexPlane();
	// loads a cube with Position
	void GenPosNormCube();
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
	void DrawUntexturedCubeNorm(const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model, const DirectX::XMFLOAT4& colour);
	void DrawTexturedPlane(const std::string& key, const DirectX::XMMATRIX& transform);
	void DrawTexturedPlane(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMFLOAT4& color);
	void DrawTexturedPlaneSlice(const std::string& key, const DirectX::XMMATRIX& transform, const int& columns, const int& rows, const int& index);
	void DrawUnTexturedModelNorm(const std::string& key, const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& model);
private:
	void CreateShaderResourceView(const std::wstring& texturefile, const std::string& mapkey);
	void BindPosTexVBSubresource(PosTex* sr);;
};