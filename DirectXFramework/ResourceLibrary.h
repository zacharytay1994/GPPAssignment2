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
	void GenPosTexModelVI(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile);
	void GenPosNormTexModelVI(const std::string& mapkey, const std::string& objfile, const std::wstring& texturefile);

	void CreateShaderResourceView(const std::wstring& texturefile, const std::string& mapkey);
	
	void DrawIndexed(const std::string& key, const DirectX::XMMATRIX& transform);
	void BindResource(const std::string& key);
};