#pragma once

#include "Surface.h"
#include "../Vec3.h"
#include <vector>
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

enum class ShaderType {
	Textured,
	TexturedNormal,
	Untextured,
	UntexturedNormal
};

class Sprite;
class Graphics {
	friend class Sprite;
	friend class Cube;
private:
	ID3D11Device*			p_device_ = nullptr;			// device representing a virtual adapter used to create resources
	IDXGISwapChain*			p_swap_chain_ = nullptr;		// pointer to interface to one or more framebuffer for storing rendered data before presenting
	ID3D11DeviceContext*	p_device_context_ = nullptr;	// pointer to interface which generates rendering commands
	ID3D11RenderTargetView* p_rtv_back_buffer_ = nullptr;	// render target: resource that can be written by pipeline, interfaces back buffer in swap chain
	ID3D11Buffer*			p_vertex_buffer_ = nullptr;
	ID3D11Buffer*			p_index_buffer_ = nullptr;
	
	// <--- REFACTORED DESIGN
	ShaderType shader_type_ = ShaderType::Untextured;
	/*_______________________________________*/
	// SHADERS FOR TEXTURED/UNTEXTURES
	/*_______________________________________*/
	ID3D11VertexShader*		p_vs_textured_ = nullptr;
	ID3D11PixelShader*		p_ps_textured_ = nullptr;
	ID3D11VertexShader*		p_vs_texturedNorm_ = nullptr;
	ID3D11PixelShader*		p_ps_texturedNorm_ = nullptr;
	ID3D11VertexShader*		p_vs_untextured_ = nullptr;
	ID3D11PixelShader*		p_ps_untextured_ = nullptr;
	ID3D11VertexShader*		p_vs_untexturedNorm_ = nullptr;
	ID3D11PixelShader*		p_ps_untexturedNorm_ = nullptr;
	/*_______________________________________*/
	// INPUT LAYOUTS
	/*_______________________________________*/
	ID3D11InputLayout*		p_il_PosTex_ = nullptr;
	ID3D11InputLayout*		p_il_PosNormTex_ = nullptr;
	// --->

	ID3D11VertexShader*		p_vertex_shader_ = nullptr;
	ID3D11PixelShader*		p_pixel_shader_ = nullptr;

	ID3D11Buffer*			p_cb_transform_ = nullptr;	
	ID3D11DepthStencilView*	p_depth_stencil_view_ = nullptr;
	bool initialized_;
	bool has_depth_stencil_ = false;

	/*_______________________________________*/
	// Defined vertex data type to render a 2D Quad (for sprites)
	struct Vertex { float x; float y; float t_x; float t_y; };
	struct VertexBuffer { Vertex topleft; Vertex topright; Vertex bottomleft; Vertex bottomright; };
	// Defined vertex data type to render a 3D Cube
	struct CubeVertex { float x; float y; float z; float t_x; float t_y; };
	
	/*_______________________________________*/
	// Defined buffer struct that represents a transformation matrix
	// to be used in the vertex shader stage as generic world, view, transform
	struct ConstantBuffer { DirectX::XMMATRIX transform; DirectX::XMMATRIX model; };
	/*_______________________________________*/
public:
	static const int viewport_width_ = 800, viewport_height_ = 600;
public:
	struct CubeVertexBuffer {
		CubeVertex ftopleft; CubeVertex ftopright; CubeVertex fbottomleft; CubeVertex fbottomright;
		CubeVertex btopleft; CubeVertex btopright; CubeVertex bbottomleft; CubeVertex bbottomright;
		CubeVertex rtopleft; CubeVertex rtopright; CubeVertex rbottomleft; CubeVertex rbottomright;
		CubeVertex ltopleft; CubeVertex ltopright; CubeVertex lbottomleft; CubeVertex lbottomright;
		CubeVertex ttopleft; CubeVertex ttopright; CubeVertex tbottomleft; CubeVertex tbottomright;
		CubeVertex bttopleft; CubeVertex bttopright; CubeVertex btbottomleft; CubeVertex btbottomright;
	};


	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void Initialize(HWND hwnd);			// general initialization of d3d device
	void InitSpritePipeline();			// configuring and initializing graphics pipeline to handle 2D Sprites
	void InitCubePipeline();
	void EndFrame();					// presents back buffer in swap chain
	// Get Set
	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetContext();
	VertexBuffer GetDefaultVB();
	const CubeVertexBuffer GetDefaultCubeVB();
	bool Initialized();
	// Miscellaneous functions
	bool BindShaderResourceView(ID3D11ShaderResourceView*& srv);		  // binds a srv to the pipeline for pixel shader sampling, i.e. individual texture resource managed by sprites (no sprite batching yet)
	void UpdateVBVertexSubresource(const VertexBuffer& vb);				  // used to update buffer data, e.g. different texture coords for frame sampling (see Sprite::Update())
	void UpdateCubeVBVertexSubresource(const CubeVertexBuffer& cvb);
	void UpdateCBTransformSubresource(const ConstantBuffer& cb);		  // used to update buffer data, e.g. transformations per frame (see Sprite::GetTransform())
	void ClearBuffer();													  // fills back buffer with colour (e.g. white)
	void Draw();														  // draw call for sprites
	void DrawIndexed(int indexcount);

	void BindCubeVertices(const CubeVertexBuffer v);
	void BindCubeIndices(const unsigned short* i, int size);

	void BindModelVertices(std::vector<DirectX::XMFLOAT3>& v);
	void BindModelIndices(std::vector<unsigned short>& i);
	void BindVertexBuffer(ID3D11Buffer* vertices);
	void BindVertexBufferStride(ID3D11Buffer* vertices, const UINT& stride);
	void BindIndexBuffer(ID3D11Buffer* indices);
	ID3D11Device* GetGraphicsDevice();
	ID3D11DeviceContext* GetGraphicsDeviceContext();

	void InitializeShadersAndInputLayouts();

	// <--- REFACTORED FUNCTIONS
	// sets the use type of the pipeline, binds the corresponding shader and input layout
	void SetUseType(const ShaderType& type);
	// --->
};