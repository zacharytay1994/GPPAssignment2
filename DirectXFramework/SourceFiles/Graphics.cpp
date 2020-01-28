#include "../Base/Graphics.h"
#include "../Base/Sprite.h"

#include <array>
#include <d3dcompiler.h>
#pragma comment(lib, "D3d11.lib")
#pragma comment(lib, "D3DCompiler.lib") // to load shader .cso files

Graphics::Graphics()
{}

Graphics::~Graphics()
{
	if (p_device_context_ != nullptr) {
		p_device_context_->Release();
	}
	if (p_swap_chain_ != nullptr) {
		p_swap_chain_->Release();
	}
	if (p_device_ != nullptr) {
		p_device_->Release();
	}
	if (p_rtv_back_buffer_ != nullptr) {
		p_rtv_back_buffer_->Release();
	}
	if (p_vertex_buffer_ != nullptr) {
		p_vertex_buffer_->Release();
	}
	if (p_cb_transform_ != nullptr) {
		p_cb_transform_->Release();
	}
}

void Graphics::Initialize(HWND hwnd)
{
	// create and configure swap chain description
	DXGI_SWAP_CHAIN_DESC sc_description = { 0 };
	sc_description.BufferDesc.Width = 0;												 // 0 takes window size
	sc_description.BufferDesc.Height = 0;												 // 0 takes window size
	sc_description.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;						 // pixel format layout BlueGreenRedAlpha, RGBA can also be used
	sc_description.BufferDesc.RefreshRate.Numerator = 0;								 // default
	sc_description.BufferDesc.RefreshRate.Denominator = 0;								 // default
	sc_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					 // no scaling needed
	sc_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	 // default
	sc_description.SampleDesc.Count = 1;												 // no anti-aliasing
	sc_description.SampleDesc.Quality = 0;												 // no anti-aliasing
	sc_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						 // use buffer as render target for pipeline
	sc_description.BufferCount = 1;														 // 1 back and front buffer
	sc_description.OutputWindow = hwnd;													 // window handle
	sc_description.Windowed = TRUE;														 // is window
	sc_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								 // discards contents of visible buffer when presenting back buffer as new display
	sc_description.Flags = 0;															 // no flags

	// create device, swap chain and context
	D3D11CreateDeviceAndSwapChain(
		nullptr,						 // *pAdapter: use default adapter
		D3D_DRIVER_TYPE_HARDWARE,		 // DriverType: hardware accelerated graphics
		nullptr,						 // Software: not using software driver type
		0,								 // Flags: no flags
		nullptr,						 // *pFeatureLevels: default order of feature level 11.0 > 10.1 > 10.0 > 9.3 > 9.2 > 9.1
		0,								 // FeatureLevels, number of elements in pFeatureLevel, default so 0
		D3D11_SDK_VERSION,				 // SDKVersion
		&sc_description,				 // *pSwapChainDesc
		&p_swap_chain_,					 // **ppSwapChain, to be filled
		&p_device_,						 // **ppDevice, to be filled
		nullptr,						 // *pFeatureLevel
		&p_device_context_				 // **ppImmediateContext, to be filled
	);

	// Set render target view of swap chain back buffer
	// fill resource with back buffer
	ID3D11Resource* p_back_buffer = nullptr;			
	p_swap_chain_->GetBuffer(							
		0,												// DXGI_SWAP_EFFECT_DISCARD method can only access first buffer in swap chain
		__uuidof(ID3D11Resource),						// type of interface used to manipulate buffer, type: REFIID
		reinterpret_cast<void**>(&p_back_buffer)		// pointer to back-buffer interface, type: void**
		);
	// create render target with back buffer resource
	p_device_->CreateRenderTargetView(					
		p_back_buffer,									// resource that represents a render target
		nullptr,										// default descripton, access all subresources in mipmap level 0
		&p_rtv_back_buffer_								// pointer to render target view, to be filled
	);
	p_back_buffer->Release();
	initialized_ = true;
}

void Graphics::InitSpritePipeline()
{
	if (!initialized_) {
		return;
	}
	/*_______________________________________*/
	// p_device_ : mainly fills D3D object through a passed in pointer
	// p_device_context : does stuff relative to its instance, like binding components
	// Flow of setup is usually DEVICE CREATES/FILLS object and CONTEXT BINDS object.
	/*_______________________________________*/
	// CREATE AND BIND VERTEX BUFFER
	// i.e. The pipeline takes in a vertex data at the first stage (input assembler/IA)
	// here we define the vertex data and the buffer of vertices that is to be passed in
	// This data will be passed to the vertex shader and generate an output.
	/*_______________________________________*/
	// Default vertex struct for sprites is a simple quad made up of 4 vertices
	const VertexBuffer vertices = GetDefaultVB();
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// how buffer will be bound to pipeline, i.e. type
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;					// how buffer is expected to be read from and written to
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// cpu access, allow cpu to change buffer content
	buffer_description.MiscFlags = 0u;								// 0 i.e. unused
	buffer_description.ByteWidth = sizeof(vertices);				// size of buffer
	buffer_description.StructureByteStride = sizeof(Vertex);		// size of each element in buffer
	D3D11_SUBRESOURCE_DATA subresource_data = {};					// subresource used to create the buffer
	subresource_data.pSysMem = &vertices;							// pointer to initialization data

	p_device_->CreateBuffer(&buffer_description, &subresource_data, &p_vertex_buffer_);
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	p_device_context_->IASetVertexBuffers(
		0u,								   // start slot, multiple vertex buffers can be bound to various slots
		1u,								   // number of buffers
		&p_vertex_buffer_,				   // buffer to be bound
		&stride,						   // pointer to array of stride values, i.e. 1 stride for each buffer, pointer to UINT is the same as an array of 1
		&offset							   // pointer to array of offset values, 0u offset for first and only buffer
	);

	/*_______________________________________*/
	// CREATE AND BIND PIXEL SHADER
	// .cso (compiled shader object) file is generated by .hlsl (high level shading language)
	/*_______________________________________*/
	ID3D11PixelShader* p_pixel_shader;
	ID3DBlob* p_blob;						// binary large object, i.e. some data
	D3DReadFileToBlob(L"Shaders/SpritePixelShader.cso", &p_blob);
	p_device_->CreatePixelShader(			
		p_blob->GetBufferPointer(),			// pointer to compiled shader 
		p_blob->GetBufferSize(),			// size of compiled shader
		nullptr,							// ignore: no class linkage
		&p_pixel_shader						// ignore: address pointer to pixel shader
	);
	p_device_context_->PSSetShader(
		p_pixel_shader,						// pointer to pixel shader
		nullptr,							// ignore: null no class instance
		0u									// ignore: 0 class instances interfaces
	);
	p_pixel_shader->Release();

	/*_______________________________________*/
	// CREATE AND BIND VERTEX SHADER
	/*_______________________________________*/
	ID3D11VertexShader* p_vertex_shader;
	D3DReadFileToBlob(L"Shaders/SpriteVertexShader.cso", &p_blob);
	p_device_->CreateVertexShader(
		p_blob->GetBufferPointer(),			 // same as pixel shader
		p_blob->GetBufferSize(),			 // same as pixel shader
		nullptr,							 // same as pixel shader
		&p_vertex_shader					 // same as pixel shader
	);
	p_device_context_->VSSetShader(
		p_vertex_shader,					 // same as pixel shader
		nullptr,							 // same as pixel shader
		0u									 // same as pixel shader
	);
	p_vertex_shader->Release();

	/*_______________________________________*/
	// CONFIGURE AND BIND INPUT LAYOUT
	// Input layout used to describe input buffer data for input-assembler stage (IA)
	// After the input-assembler stage is the vertex shader stage where the
	// input vertices will be passed to.
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-input-assembler-stage
	/*_______________________________________*/
	ID3D11InputLayout* p_input_layout;
	const D3D11_INPUT_ELEMENT_DESC input_element_description[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	p_device_->CreateInputLayout(
		input_element_description,						// specifies how input vertex data structure should be read as types, and processed by the vertex shader through semantic names e.g. "Position"
		(UINT)std::size(input_element_description),		// number of input data types
		p_blob->GetBufferPointer(),						// pointer to compiled shader
		p_blob->GetBufferSize(),						// size of compiled shader
		&p_input_layout									// pointer to input layout object, to be filled
	);
	p_device_context_->IASetInputLayout(p_input_layout);
	p_input_layout->Release();
	p_blob->Release();

	/*_______________________________________*/
	// CREATE AND BIND VERTEX CONSTANT BUFFER
	// Additional data we might like to bind to various stages in the pipeline.
	// Can be used to store repeated data that all vertices in processing would share.
	// E.g. world, view, and projection transformations, are the same between all vertices of the same mesh
	/*_______________________________________*/
	// defined data struct that stores a transformation matrix
	const ConstantBuffer cb = { dx::XMMatrixIdentity() };

	// shares similar setup process as vertex buffer
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(cb);
	bd.StructureByteStride = {};
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = &cb;
	p_device_->CreateBuffer(&bd, &sd, &p_cb_transform_);
	// choose which stage to bind buffer to, i.e. which stage is going to use the data
	p_device_context_->VSSetConstantBuffers(0u, 1u, &p_cb_transform_);

	/*_______________________________________*/
	// BIND RENDER TARGET
	// RenderTargetView object created using the swap chain's backbuffer.
	// Defines where the pipeline will render to, i.e. fill the buffer with pixel data (color)
	// The RenderTargetView was created during the initialization phase after creating the swap chain.
	/*_______________________________________*/
	p_device_context_->OMSetRenderTargets(
		1u,
		&p_rtv_back_buffer_,
		nullptr);

	/*_______________________________________*/
	// SET PRIMITIVE TOPOLOGY OF INPUT ASSEMBLER
	// How the vertices should be interpreted as
	// e.g. point list, line list, line strip, etc...
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies
	/*_______________________________________*/
	p_device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	/*_______________________________________*/
	// CONFIGURE AND BIND VIEWPORT
	// The output area in the defined window
	/*_______________________________________*/
	D3D11_VIEWPORT view_port;
	view_port.Width = (FLOAT)viewport_width_;
	view_port.Height = (FLOAT)viewport_height_;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	p_device_context_->RSSetViewports(1u, &view_port);

	/*_______________________________________*/
	// CREATE AND BIND SAMPLER STATE
	// How textures are sampled in the pixel shader stage and
	// how rasterization is handled e.g. wrap, mirror, clamp etc..
	// e.g. with/without antialiasing on minification/magnification/miplevels etc..
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
	/*_______________________________________*/
	D3D11_SAMPLER_DESC sampler_description = {};
	sampler_description.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR; // point sampling for mini and magni -fication, linear interpolation for mip levels
	sampler_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ID3D11SamplerState* p_sampler_state;
	p_device_->CreateSamplerState(&sampler_description, &p_sampler_state);
	p_device_context_->PSSetSamplers(0u, 1u, &p_sampler_state);
	p_sampler_state->Release();
}

void Graphics::InitCubePipeline()
{
	if (!initialized_) {
		return;
	}
	/*_______________________________________*/
	// p_device_ : mainly fills D3D object through a passed in pointer
	// p_device_context : does stuff relative to its instance, like binding components
	// Flow of setup is usually DEVICE CREATES/FILLS object and CONTEXT BINDS object.
	/*_______________________________________*/
	// CREATE AND BIND VERTEX BUFFER
	// i.e. The pipeline takes in a vertex data at the first stage (input assembler/IA)
	// here we define the vertex data and the buffer of vertices that is to be passed in
	// This data will be passed to the vertex shader and generate an output.
	/*_______________________________________*/
	// Default vertex struct for sprites is a simple quad made up of 4 vertices
	const CubeVertexBuffer vertices = GetDefaultCubeVB();
	D3D11_BUFFER_DESC buffer_description = {};
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// how buffer will be bound to pipeline, i.e. type
	buffer_description.Usage = D3D11_USAGE_DYNAMIC;					// how buffer is expected to be read from and written to
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// cpu access, allow cpu to change buffer content
	buffer_description.MiscFlags = 0u;								// 0 i.e. unused
	buffer_description.ByteWidth = sizeof(vertices);				// size of buffer
	buffer_description.StructureByteStride = sizeof(CubeVertex);	// size of each element in buffer
	D3D11_SUBRESOURCE_DATA subresource_data = {};					// subresource used to create the buffer
	subresource_data.pSysMem = &vertices;							// pointer to initialization data

	p_device_->CreateBuffer(&buffer_description, &subresource_data, &p_vertex_buffer_);
	const UINT stride = sizeof(CubeVertex);
	const UINT offset = 0u;
	p_device_context_->IASetVertexBuffers(
		0u,								   // start slot, multiple vertex buffers can be bound to various slots
		1u,								   // number of buffers
		&p_vertex_buffer_,				   // buffer to be bound
		&stride,						   // pointer to array of stride values, i.e. 1 stride for each buffer, pointer to UINT is the same as an array of 1
		&offset							   // pointer to array of offset values, 0u offset for first and only buffer
	);

	// Create index buffer
	const unsigned short indices[] = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	}; 
	ID3D11Buffer* pIndexBuffer;
	D3D11_BUFFER_DESC index_buffer_desc = {};
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.CPUAccessFlags = 0u;
	index_buffer_desc.MiscFlags = 0u;
	index_buffer_desc.ByteWidth = sizeof(indices);
	index_buffer_desc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA index_subresource_data = {};				// subresource used to create the buffer
	index_subresource_data.pSysMem = indices;

	p_device_->CreateBuffer(&index_buffer_desc, &index_subresource_data, &pIndexBuffer);
	p_device_context_->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);

	pIndexBuffer->Release();

	/*_______________________________________*/
	// CREATE AND BIND PIXEL SHADER
	// .cso (compiled shader object) file is generated by .hlsl (high level shading language)
	/*_______________________________________*/
	ID3D11PixelShader* p_pixel_shader;
	ID3DBlob* p_blob;						// binary large object, i.e. some data
	D3DReadFileToBlob(L"Shaders/CubePixelShader.cso", &p_blob);
	p_device_->CreatePixelShader(
		p_blob->GetBufferPointer(),			// pointer to compiled shader 
		p_blob->GetBufferSize(),			// size of compiled shader
		nullptr,							// ignore: no class linkage
		&p_pixel_shader						// ignore: address pointer to pixel shader
	);
	p_device_context_->PSSetShader(
		p_pixel_shader,						// pointer to pixel shader
		nullptr,							// ignore: null no class instance
		0u									// ignore: 0 class instances interfaces
	);
	p_pixel_shader->Release();

	/*_______________________________________*/
	// CREATE AND BIND VERTEX SHADER
	/*_______________________________________*/
	ID3D11VertexShader* p_vertex_shader;
	D3DReadFileToBlob(L"Shaders/CubeVertexShader.cso", &p_blob);
	p_device_->CreateVertexShader(
		p_blob->GetBufferPointer(),			 // same as pixel shader
		p_blob->GetBufferSize(),			 // same as pixel shader
		nullptr,							 // same as pixel shader
		&p_vertex_shader					 // same as pixel shader
	);
	p_device_context_->VSSetShader(
		p_vertex_shader,					 // same as pixel shader
		nullptr,							 // same as pixel shader
		0u									 // same as pixel shader
	);
	p_vertex_shader->Release();

	/*_______________________________________*/
	// CONFIGURE AND BIND INPUT LAYOUT
	// Input layout used to describe input buffer data for input-assembler stage (IA)
	// After the input-assembler stage is the vertex shader stage where the
	// input vertices will be passed to.
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-input-assembler-stage
	/*_______________________________________*/
	ID3D11InputLayout* p_input_layout;
	const D3D11_INPUT_ELEMENT_DESC input_element_description[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	p_device_->CreateInputLayout(
		input_element_description,						// specifies how input vertex data structure should be read as types, and processed by the vertex shader through semantic names e.g. "Position"
		(UINT)std::size(input_element_description),		// number of input data types
		p_blob->GetBufferPointer(),						// pointer to compiled shader
		p_blob->GetBufferSize(),						// size of compiled shader
		&p_input_layout									// pointer to input layout object, to be filled
	);
	p_device_context_->IASetInputLayout(p_input_layout);
	p_input_layout->Release();
	p_blob->Release();

	/*_______________________________________*/
	// CREATE AND BIND VERTEX CONSTANT BUFFER
	// Additional data we might like to bind to various stages in the pipeline.
	// Can be used to store repeated data that all vertices in processing would share.
	// E.g. world, view, and projection transformations, are the same between all vertices of the same mesh
	/*_______________________________________*/
	// defined data struct that stores a transformation matrix
	const ConstantBuffer cb = { dx::XMMatrixIdentity() };

	// shares similar setup process as vertex buffer
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(cb);
	bd.StructureByteStride = {};
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = &cb;
	p_device_->CreateBuffer(&bd, &sd, &p_cb_transform_);
	// choose which stage to bind buffer to, i.e. which stage is going to use the data
	p_device_context_->VSSetConstantBuffers(0u, 1u, &p_cb_transform_);

	/*_______________________________________*/
	// BIND RENDER TARGET
	// RenderTargetView object created using the swap chain's backbuffer.
	// Defines where the pipeline will render to, i.e. fill the buffer with pixel data (color)
	// The RenderTargetView was created during the initialization phase after creating the swap chain.
	/*_______________________________________*/
	p_device_context_->OMSetRenderTargets(
		1u,
		&p_rtv_back_buffer_,
		nullptr);

	/*_______________________________________*/
	// SET PRIMITIVE TOPOLOGY OF INPUT ASSEMBLER
	// How the vertices should be interpreted as
	// e.g. point list, line list, line strip, etc...
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies
	/*_______________________________________*/
	p_device_context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*_______________________________________*/
	// CONFIGURE AND BIND VIEWPORT
	// The output area in the defined window
	/*_______________________________________*/
	D3D11_VIEWPORT view_port;
	view_port.Width = (FLOAT)viewport_width_;
	view_port.Height = (FLOAT)viewport_height_;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	p_device_context_->RSSetViewports(1u, &view_port);

	/*_______________________________________*/
	// CREATE AND BIND SAMPLER STATE
	// How textures are sampled in the pixel shader stage and
	// how rasterization is handled e.g. wrap, mirror, clamp etc..
	// e.g. with/without antialiasing on minification/magnification/miplevels etc..
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
	/*_______________________________________*/
	D3D11_SAMPLER_DESC sampler_description = {};
	sampler_description.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR; // point sampling for mini and magni -fication, linear interpolation for mip levels
	sampler_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ID3D11SamplerState* p_sampler_state;
	p_device_->CreateSamplerState(&sampler_description, &p_sampler_state);
	p_device_context_->PSSetSamplers(0u, 1u, &p_sampler_state);
	p_sampler_state->Release();
}

void Graphics::EndFrame()
{
	p_swap_chain_->Present(1u, 0u);
}

ID3D11Device& Graphics::GetDevice()
{
	return *p_device_;
}

ID3D11DeviceContext& Graphics::GetContext()
{
	return *p_device_context_;
}

Graphics::VertexBuffer Graphics::GetDefaultVB()
{
	return {
		{-1.0f, 1.0f, 0.0f, 0.0f},	   // top-left vertex {vertex.x, vertex.y, tex-coord.x, tex-coord.y}
		{1.0f, 1.0f, 1.0f, 0.0f},	   // top-right vertex
		{-1.0f, -1.0f, 0.0f, 1.0f},	   // bottom-left vertex
		{1.0f, -1.0f, 1.0f, 1.0f},	   // bottom-right vertex
	};
}

Graphics::CubeVertexBuffer Graphics::GetDefaultCubeVB()
{
	return {
		{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f},
		{1.0f, -1.0f, -1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, -1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, -1.0f, 1.0f, 1.0f},
		{-1.0f, -1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}
	};
}

bool Graphics::Initialized()
{
	return initialized_;
}

bool Graphics::BindShaderResourceView(ID3D11ShaderResourceView*& srv)
{
	p_device_context_->PSSetShaderResources(0u, 1u, &srv);
	return true;
}

void Graphics::UpdateVBVertexSubresource(const VertexBuffer & vb)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	p_device_context_->Map(p_vertex_buffer_, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_subresource);
	memcpy(mapped_subresource.pData, &vb, sizeof(vb));
	p_device_context_->Unmap(p_vertex_buffer_, 0u);
}

void Graphics::UpdateCubeVBVertexSubresource(const CubeVertexBuffer& cvb)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	p_device_context_->Map(p_vertex_buffer_, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_subresource);
	memcpy(mapped_subresource.pData, &cvb, sizeof(cvb));
	p_device_context_->Unmap(p_vertex_buffer_, 0u);
}

void Graphics::UpdateCBTransformSubresource(const ConstantBuffer & cb)
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	p_device_context_->Map(p_cb_transform_, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_subresource);
	memcpy(mapped_subresource.pData, &cb, sizeof(cb));
	p_device_context_->Unmap(p_cb_transform_, 0u);
}

void Graphics::ClearBuffer()
{
	const float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	p_device_context_->ClearRenderTargetView(p_rtv_back_buffer_, color);
}

void Graphics::Draw()
{
	p_device_context_->Draw(6u, 0u);
}

void Graphics::DrawIndexed() {
	p_device_context_->DrawIndexed(36u, 0u, 0);
}
