#pragma once

#include "Entity.h"

class Player : public Entity {
private:
public:
	Player(std::shared_ptr<Graphics> graphics, const std::wstring& spritename);
	void CDraw() override;
	void CUpdate(const float& frametime) override;
};
