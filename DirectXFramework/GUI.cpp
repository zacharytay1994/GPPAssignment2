#include "GUI.h"
#include <assert.h>

GUI::GUI(std::shared_ptr<ResourceLibrary> rl)
	:
	rl_(rl)
{
}

DirectX::XMMATRIX GUI::GetTransform(const Vecf3& position, const Vecf3& scale)
{
	// no scaling by 0
	assert(scale.x != 0.0f || scale.y != 0.0f || scale.z != 0.0f);
	return	DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
	);
}

void GUI::Draw()
{
	//rl_->DrawTexturedPlane("grassblock", GetTransform({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }));
	rl_->DrawTexturedPlaneSlice("grassblock", GetTransform({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }), 1, 1, 1);
}
