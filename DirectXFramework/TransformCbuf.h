#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
public:
	TransformCbuf( std::shared_ptr<Graphics> gfx,const Drawable& parent,UINT slot = 0u );
	void Bind( std::shared_ptr<Graphics> gfx ) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};