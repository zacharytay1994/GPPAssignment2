#include "Level.h"
#include "Block.h"
#include "CollisionComponent.h"

#include <cmath>

Level::Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	Scene(gfx, input, rl)
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
}

void Level::Update(const float& dt)
{
	Scene::Update(dt);
	ps_.Update(dt);
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

		// Get player heading & check if there is a block in front of the player
		float y_rot = fmod(player_->GetOrientation().y > 0 ? player_->GetOrientation().y : player_->GetOrientation().y + 2*PI, 2*PI);
		if ((y_rot >= 7*PI/4) || (y_rot <= PI/4)) {

			// Facing forward
			RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_);
			// testng particle system
			Vecf3 pos = mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_->GetPosition();
			pos.z += 0.5f;
			ps_.EmitSphere(10, pos, 1, 1.0f, 0.1f, 2.0f, { 0.0f, 1.0f, 0.0f, 1.0f });

		} else if (y_rot <= 3*PI/4) {
			// Facing right
			RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].ent_);

		}
		else if (y_rot <= 5 * PI / 4) {

			// Facing downward
			RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_);

		}
		else {

			// Facing left
			RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].ent_);

		}
	}

	if (input_->KeyWasPressed('E')) {
		//ps_.EmitSphere(15, {6.0f, 5.0f, 6.0f}, 3.0f);
	}
	//test_object_->Draw();
	//test_object_2_->Draw();
	// --->
}

void Level::Render(const float& dt)
{
	Scene::Render(dt);
	ps_.Render();
}

void Level::SpawnRandomBlocks(const int& val)
{
	// get random value between -5 and 5
	float x_rand = float(-val + (rand() % val + 1));
	float z_rand = float(-val + (rand() % val + 1));
	float rand_size = float(rand() % 4 + 1);
	gravity_blocks_.push_back(AddSolidBlock("grassblock", { x_rand, 8.0f, z_rand }, { rand_size, 1.0f, rand_size }, 5.0f * rand_size));
}
