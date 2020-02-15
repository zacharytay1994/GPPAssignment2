#include "GUI.h"
#include <assert.h>
#include <sstream>

GUI::GUI(std::shared_ptr<ResourceLibrary> rl)
	:
	rl_(rl)
{
	// initialize level data
	level_data_.time_passed_ = 0.0f;
	level_data_.wood_collected_ = 0;
	level_data_.stone_collected_ = 0;
	level_data_.rails_collected_ = 0;
	level_data_.train_distance_ = 0;
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

void GUI::DrawString(const Vecf3& position, const float& scale, const std::string& text)
{
	float spacing = 2.0f * scale;
	Vecf3 initial_position = position;
	for (int i = 0; i < text.size(); i++) {
		initial_position = position + Vecf3(i * spacing, 0.0f, 0.0f);
		int index = 0;
		int ascii = (int)text[i];
		// if is caps alphabet
		if (((ascii >= 65 && ascii <= 90) || ascii >= 97 && ascii <= 122)) {
			// if small
			if (ascii >= 97 && ascii <= 122) {
				// make it caps
				index = ascii - 96 + 65;
			}
			else {
				index = ascii - 64 + 33;
			}
		}
		// if is a symbol
		else if (ascii >= 32 && ascii <= 64) {
			index = ascii - 31;
		}
		else if (ascii >= 91 && ascii <= 96) {
			index = ascii - 90 + 59;
		}
		else  if (ascii >= 123 && ascii <= 126) {
			index = ascii - 122 + 91;
		}
		rl_->DrawTexturedPlaneSlice("mariofont", GetTransform(initial_position, { scale, scale, scale }), 32, 3, index);
	}
}

void GUI::Update(const float& dt)
{
	// update time passed
	level_data_.time_passed_ += dt;
	ProcessResourceQueue(dt);
}

void GUI::DrawLevelHUD(const float& dt)
{
	// draw time passed gui
	std::stringstream ss;
	//float round = ((int)(level_data_.time_passed_ * 100 + .5f) / 100.0f);
	rl_->DrawTexturedPlane("stopwatchlogo", GetTransform({ -0.90f, 0.90f, 0.0f }, { 0.04f, 0.04f, 0.04f }));
	ss << " x" << (int)level_data_.time_passed_ << "";
	DrawString({ -0.90f, 0.90f, 0.0f }, 0.04f, ss.str());
	ss.str("");
	rl_->DrawTexturedPlane("woodlogo", GetTransform({ -0.40f, 0.90f, 0.0f }, { 0.04f, 0.04f, 0.04f }));
	ss << " x" << level_data_.wood_collected_;
	DrawString({ -0.40f, 0.90f, 0.0f }, 0.04f, ss.str());
	ss.str("");
	rl_->DrawTexturedPlane("stonelogo", GetTransform({ 0.10f, 0.90f, 0.0f }, { 0.04f, 0.04f, 0.04f }));
	ss << " x" << level_data_.stone_collected_;
	DrawString({ 0.10f, 0.90f, 0.0f }, 0.04f, ss.str());
	ss.str("");
	rl_->DrawTexturedPlane("tracklogo", GetTransform({ 0.60f, 0.90f, 0.0f }, { 0.04f, 0.04f, 0.04f }));
	ss << " x" << level_data_.rails_collected_;
	DrawString({ 0.60f, 0.90f, 0.0f }, 0.04f, ss.str());
	rl_->DrawTexturedPlane("bottombar", GetTransform({ 0.0f, -0.9f, 0.0f }, { 0.95f, 0.05f, 0.1f }));
	DrawSteamtrainSprite(dt);
	// draw string ontop of train
	ss.str("");
	ss << train_x << "m";
	DrawString({ current_steam_train_position_, -0.60f, 0.0f }, 0.04f, ss.str());
	// draw string at end max distance
	ss.str("");
	ss << max_distance_ << "m";
	DrawString({ 0.75f, -0.60f, 0.0f }, 0.04f, ss.str());
}

void GUI::DrawSteamtrainSprite(const float& dt)
{
	// move train to current position
	if (abs(current_steam_train_position_ - steam_train_position_.x) > 0.01f) {
		int i = steam_train_position_.x - current_steam_train_position_ > 0 ? 1 : -1;
		current_steam_train_position_ += i * 0.2f * dt;
	}
	steam_train_position_.x = -0.9f + 1.8f * ((float)train_x / (float)max_distance_);
	if (steam_train_counter_ < steam_train_anim_rate_) {
		steam_train_counter_ += dt;
	}
	else {
		steam_train_counter_ = 0.0f;
		steam_train_index_ = ((steam_train_index_ + 1) % steam_train_total_index_) + 1;
	}
	rl_->DrawTexturedPlaneSlice("steamtrainsprite", GetTransform({current_steam_train_position_, -0.85f, -0.1f}, 
		{ 0.2f, 0.2f, 0.2f }), 3, 1, steam_train_index_);
}

void GUI::SetTrainX(const int& i)
{
	train_x = i;
	if (train_x > max_distance_) {
		max_distance_ += 10;
	}
}

void GUI::SetMaximumX(const int& i)
{
}

void GUI::ProcessResourceQueue(const float& dt)
{
	ResourceQueue rq;
	while (resource_queue_.size() > 0) {
		rq = resource_queue_.front();
		resource_queue_.pop();
		// check timer
		if (rq.time_delay_ > 0.0f) {
			rq.time_delay_ -= dt;
			// if not 0 add back to queue
			if (rq.time_delay_ > 0.0f) {
				resource_queue_copy_.push(rq);
			}
			else {
				// increment type resource by 1
				switch (rq.type_) {
				case 0: // rock
					level_data_.stone_collected_++;
					break;
				case 1: // tree
					level_data_.wood_collected_++;
					break;
				}
			}
		}
	}
	// swap queues
	std::swap(resource_queue_copy_, resource_queue_);
}

void GUI::AddResource(const ResourceQueue& rq)
{
	resource_queue_.push(rq);
}

void GUI::Draw()
{
	//rl_->DrawTexturedPlaneSlice("halogencaps", GetTransform({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }), 26, 1, 1);
	DrawString({ -1.5f, 0.0f, 0.0f }, 0.1f, "test{&@']28;Ab");
}
