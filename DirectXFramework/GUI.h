#pragma once

#include "ResourceLibrary.h"
#include "Vec3.h"

#include <string>
#include <queue>
#include <memory>

class GUI {
private:
	struct ResourceQueue {
		float time_delay_;
		int type_; // 0 for rock, 1 for tree
	};
	struct LevelData {
		float time_passed_;
		int wood_collected_;
		int stone_collected_;
		int rails_collected_;
		int train_distance_;
	};
	std::queue<ResourceQueue> resource_queue_;
	std::queue<ResourceQueue> resource_queue_copy_;
	std::shared_ptr<ResourceLibrary> rl_;
	float steam_train_counter_ = 0.0f;
	float steam_train_anim_rate_ = 0.1f;
	int steam_train_index_ = 1;
	int steam_train_total_index_ = 3;
	Vecf3 steam_train_position_ = { 0.0f, -0.9f, 0.0f };
	float current_steam_train_position_ = -0.8f;
	int train_x = 0;
	int max_distance_ = 10;
public:
	LevelData level_data_;
public:
	GUI(std::shared_ptr<ResourceLibrary> rl);
	DirectX::XMMATRIX GetTransform(const Vecf3& position, const Vecf3& scale);
	void DrawString(const Vecf3& position, const float& scale, const std::string& text);
	void Update(const float& dt);
	void DrawLevelHUD(const float& dt);
	void DrawSteamtrainSprite(const float& dt);
	void SetTrainX(const int& i);
	void SetMaximumX(const int& i);
	void ProcessResourceQueue(const float& dt);
	void AddResource(const ResourceQueue& rq);
	void DrawSprite(const std::string& key, const Vecf3& position, const Vecf3& scale);
	void DrawOverlay(const std::string& key);
	int GetTrainX();
	void Draw();
};