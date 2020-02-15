#include "Level.h"
#include "Block.h"
#include "CollisionComponent.h"
#include "ChooChoo.h"
#include "InputComponent.h"

#include <cmath>

Level::Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game)
	:
	Scene(gfx, input, rl, game)
{
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
	const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };

	// Initialize map generator
	mapGen_ = std::make_unique<MapGenerator>(graphics_, input_, rl_, this);

	// Generate map
	mapGen_->GenerateMap();

	//AddSolidBlock("grassblock", { 0.0f, -60.0f, 5.0f }, { 30.0f, 30.0f, 30.0f }, 500000.0f);
	//std::shared_ptr<Block> giraffe = AddModel("nsur", { 1.0f, 0.0f, 10.0f }, { 0.10f, 0.10f, 0.10f }, true);

	//giraffe->GetCube().SetAngleZDeg(180);
	//giraffe->GetCube().SetAngleYDeg(180);
	//giraffe->GetCube().SetAngleXDeg(-90);
	//AddSolidBlock("grassblock", { 0.0f, -60.0f, 5.0f }, { 30.0f, 30.0f, 30.0f }, 500000.0f);
	//gravity_blocks_.push_back(AddSolidBlock("grassblock", { 0.0f, 5.0f, 5.0f }, { 1.0f, 1.0f, 1.0f }, 5.0f));
	//gravity_blocks_[0]->GetComponentOfType<CollisionComponent>("Collision")->SetAngularVelocity({ 1.0f, 1.0f, 1.0f }); // only solid blocks can be added to gravity blocks
	//AddBlock("grassblock", { 0.0f, -10.0f, 5.0f }, { 5.0f, 1.0f, 5.0f });
	player_ = AddPlayer({ 0.0f, 1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f });
	player_->AddComponent(std::make_shared<InputComponent>(InputComponent(*player_, *input_)));

	highlight_block_ = AddBlock("highlight", player_->GetPosition(), { 0.5f,0.03125,0.5f });
}

void Level::Update(const float& dt)
{
	Scene::Update(dt);
	ps_.Update(dt);
	gui_.Update(dt);
	ps_.SetCameraSuckPosition(input_->GetCameraPosition() + Vecf3(0.0f, 1.2f, 0.0f));
	gui_.SetTrainX(std::dynamic_pointer_cast<ChooChoo>(mapGen_->train_)->GetPosition().x);

	if (crafting_cooldown_timer > 0) {
		crafting_cooldown_timer -= dt;
	}

	if (input_->KeyWasPressed('P')) {
		gui_.AddResource({ 1.0f, 0 });
	}
	// <--- test code can remove if need be
	if (input_->KeyWasPressed('B')) {
		start_spawning_ = true;
	}
	if (start_spawning_) {
		if (spawn_iterations_ > 0) {
			if (interval_tracker_ > spawn_interval_) {
				SpawnRandomBlocks(5);
				SpawnRandomBlocks(10);
				SpawnRandomBlocks(15);
				interval_tracker_ = 0.0f;
				spawn_iterations_ -= 1;
			}
			else {
				interval_tracker_ += dt;
			}
		}
	}

	// Generate new chunk
	if (input_->KeyWasPressed('G')) mapGen_->GenerateMap();

	// Collect resource if facing block & within 1 block
	if (input_->KeyWasPressed('C')) {

		// Get normalized player pos
		Vecf3 norm_player_pos = player_->GetPosition();
		norm_player_pos.x = (int)round(player_->GetPosition().x) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);
		norm_player_pos.z = (int)round(player_->GetPosition().z) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);

		// Get player heading & check if there is a block in front of the player
		float y_rot = fmod(player_->GetOrientation().y > 0 ? player_->GetOrientation().y : player_->GetOrientation().y + 2*PI, 2*PI);
		if ((y_rot >= 7*PI/4) || (y_rot <= PI/4)) {

			// Facing forward
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
				EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_,
					mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_->GetPosition());
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)]));
			}

		} else if (y_rot <= 3*PI/4) {

			// Facing right
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].breakable_) {
				EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].block_type_,
					mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].ent_->GetPosition());
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)]));
			}
		}
		else if (y_rot <= 5 * PI / 4) {

			// Facing downward
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
				EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_,
					mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_->GetPosition());
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)]));
			}
		}
		else {

			// Facing left
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].breakable_) {
				EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].block_type_,
					mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].ent_->GetPosition());
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)]));
			}
		}
	}
	// Place or craft rail
	if (input_->KeyWasPressed('R')) {
		
		// Get normalized player pos
		Vecf3 norm_player_pos = player_->GetPosition();
		norm_player_pos.x = (int)round(player_->GetPosition().x) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);
		norm_player_pos.z = (int)round(player_->GetPosition().z) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);

		// Check if rail can be placed on player pos
		if ((mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x)].walkable_ &&
			mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_ != ResourceBlockType::Rail) &&
			rail_count_ > 0) {

			// Spawn rail
			std::shared_ptr<Block> r = std::make_shared<Rail>("rail", graphics_, input_, rl_);
			r->SetScale({ 0.5f, 0.03125f, 0.5f });
			r->SetPosition(Vecf3((int)round(norm_player_pos.x), -0.5f, (int)round(norm_player_pos.z)));
			if (mapGen_->AddResource({ ResourceBlockType::Rail, 0, 1, r })) {
				rail_count_ -= 1;
				gui_.AddResource({ 1.0f, 2, -1 });
			}
			else {
				CraftRails();
			}
			

		} // TODO: check crafter nearby
		else {
			CraftRails();
		}


	}




	Vecf3 player_pos = player_->GetPosition();
	Vecf3 player_orientation = player_->GetOrientation();
	float y_rot = fmod(player_->GetOrientation().y > 0 ? player_->GetOrientation().y : player_->GetOrientation().y + 2 * PI, 2 * PI);
	Vecf3 highlight_block_pos = {round(player_pos.x), round(player_pos.y-2)+0.5f, round(player_pos.z)};
	if ((y_rot >= 7 * PI / 4) || (y_rot <= PI / 4)) {
		highlight_block_pos.z += 1;
	}
	else if (y_rot <= 3 * PI / 4) {
		highlight_block_pos.x += 1;
	}
	else if (y_rot <= 5 * PI / 4) {
		highlight_block_pos.z -= 1;
	}
	else {
		highlight_block_pos.x -= 1;
	}



	highlight_block_->SetPosition(highlight_block_pos);
}

void Level::Render(const float& dt)
{
	Scene::Render(dt);
	ps_.Render();
	gui_.DrawLevelHUD(dt);
}

void Level::SpawnRandomBlocks(const int& val)
{
	// get random value between -5 and 5
	float x_rand = float(-val + (rand() % val + 1));
	float z_rand = float(-val + (rand() % val + 1));
	float rand_size = float(rand() % 4 + 1);
	gravity_blocks_.push_back(AddSolidBlock("grassblock", { x_rand, 8.0f, z_rand }, { rand_size, 1.0f, rand_size }, 5.0f * rand_size));
}

void Level::EmitDestructionParticles(const ResourceBlockType& type, const Vecf3& pos)
{
	switch (type) {
	case ResourceBlockType::Rock:
		// params are : (no. of particles, position, radius, mass, scale, force, colour)
		ps_.EmitSphere(5, pos + Vecf3(0.0f, 0.5f, 0.0f), 0.8f, 4.0f, 0.1f, 2.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); // larger rock chunks
		ps_.EmitSphere(10, pos + Vecf3(0.0f, 0.5f, 0.0f), 1.0f, 1.0f, 0.05f, 2.0f, { 1.0f, 0.0f, 0.0f, 1.0f }); // smaller rock chunks
		gui_.AddResource({ 1.0f, 0 , 1});
		rock_count_ += 1;
		break;
	case ResourceBlockType::Tree:
		ps_.EmitSphere(5, pos + Vecf3(0.0f, 0.5f, 0.0f), 0.8f, 0.8f, 0.1f, 1.0f, { 0.6f, 0.6f, 0.6f, 1.0f }); // trunk
		ps_.EmitSphere(10, pos + Vecf3(0.0f, 0.5f, 0.0f), 0.1, Randf(0.8f, 1.2f), 0.05f, 1.0f, { 0.3f, Randf(0.8f, 1.0f), 0.3f, 1.0f }); // lower leaves
		ps_.EmitSphere(15, pos + Vecf3(0.0f, 1.5f, 0.0f), 0.1, Randf(0.8f, 1.2f), 0.05f, 1.0f, { 0.3f, Randf(0.8f, 1.0f), 0.3f, 1.0f }); // upper leaves
		gui_.AddResource({ 1.0f, 1 , 1});
		wood_count_ += 1;
		break;
	}
}

void Level::CraftRails()
{
	// TODO: check crafter nearby
	if (crafting_cooldown_timer <= 0 && wood_count_ >= wood_per_rail_ && rock_count_ >= rock_per_rail_) {
		rail_count_ += 1;
		gui_.AddResource({ 0.1f, 2, 1 });
		wood_count_ -= wood_per_rail_;
		gui_.AddResource({ 0.1f, 1, -wood_per_rail_ });
		rock_count_ -= rock_per_rail_;
		gui_.AddResource({ 0.1f, 0, -rock_per_rail_ });

		crafting_cooldown_timer = crafting_cooldown;
	}

}
