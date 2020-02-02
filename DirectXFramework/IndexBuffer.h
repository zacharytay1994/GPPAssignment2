#pragma once
#include "Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( std::shared_ptr<Graphics> gfx,const std::vector<unsigned short>& indices );
	void Bind(std::shared_ptr<Graphics> gfx) noexcept override;
	UINT GetCount() const noexcept;
protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};