#pragma once
#include "Base/Graphics.h"
#include <memory>

class Bindable
{
public:
	virtual void Bind( std::shared_ptr<Graphics> gfx ) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept;
	static ID3D11Device* GetDevice( Graphics& gfx ) noexcept;
};