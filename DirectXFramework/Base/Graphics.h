#pragma once

#include "Surface.h"
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class Sprite;
class Graphics {
	friend class Sprite;
private:
	ID3D11Device*			p_device_ = nullptr;			// device representing a virtual adapter used to create resources
	IDXGISwapChain*			p_swap_chain_ = nullptr;		// pointer to interface to one or more framebuffer for storing rendered data before presenting
	ID3D11DeviceContext*	p_device_context_ = nullptr;	// pointer to interface which generates rendering commands
	ID3D11RenderTargetView* p_rtv_back_buffer_ = nullptr;	// render target: resource that can be written by pipeline, interfaces back buffer in swap chain
	ID3D11Buffer*			p_vertex_buffer_ = nullptr;
	ID3D11Buffer*			p_cb_transform_ = nullptr;				
	bool initialized_ = false;												   
	/*_______________________________________*/
	// Defined vertex data type to render a 2D Quad (for sprites)
	struct Vertex { float x; float y; float t_x; float t_y; };
	struct VertexBuffer { Vertex topleft; Vertex topright; Vertex bottomleft; Vertex bottomright; };
	/*_______________________________________*/
	// Defined buffer struct that represents a transformation matrix
	// to be used in the vertex shader stage as generic world, view, transform
	struct ConstantBuffer { DirectX::XMMATRIX transform; };
	/*_______________________________________*/
public:
	static const int viewport_width_ = 800, viewport_height_ = 600;
public:
	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void Initialize(HWND hwnd);			// general initialization of d3d device
	void InitSpritePipeline();			// configuring and initializing graphics pipeline to handle 2D Sprites
	void EndFrame();					// presents back buffer in swap chain
	// Get Set
	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetContext();
	VertexBuffer GetDefaultVB();
	bool Initialized();
	// Miscellaneous functions
	bool BindShaderResourceView(ID3D11ShaderResourceView*& srv);		  // binds a srv to the pipeline for pixel shader sampling, i.e. individual texture resource managed by sprites (no sprite batching yet)
	void UpdateVBVertexSubresource(const VertexBuffer& vb);				  // used to update buffer data, e.g. different texture coords for frame sampling (see Sprite::Update())
	void UpdateCBTransformSubresource(const ConstantBuffer& cb);		  // used to update buffer data, e.g. transformations per frame (see Sprite::GetTransform())
	void ClearBuffer();													  // fills back buffer with colour (e.g. white)
	void Draw();

	// Additional function
	void DrawLineVertices();
};