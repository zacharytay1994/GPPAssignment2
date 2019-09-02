#pragma once
#include "ZombsEntity.h"

class ZombsPlayer : public ZombsEntity {
private:
public:
	ZombsPlayer(std::shared_ptr<Graphics>& graphics, const std::wstring& spriteresource, const int& width, const int& height);
	~ZombsPlayer();
	virtual void Update(std::shared_ptr<Input>& input, const float& dt) override;
	virtual void Draw() override;
};