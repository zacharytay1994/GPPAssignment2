#include "Level.h"
#include "Block.h"
#include "CollisionComponent.h"
#include <sstream>
#include <string>

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

	resourceTileData_ = mapGen_->GetResourceTileData();

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
		//float y_rot = fmod(player_->GetOrientation().y > 0 ? player_->GetOrientation().y : player_->GetOrientation().y + 2*PI, 2*PI);
		//if ((y_rot >= 7*PI/4) || (y_rot <= PI/4)) {


		//	// Facing forward
		//	RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_);

			// Facing forward
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
				RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_);
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)]));
			}


		//} else if (y_rot <= 3*PI/4) {


		//	// Facing right
		//	RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].ent_);

			// Facing right
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
				RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].ent_);
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)]));
			}


		//} else if (y_rot <= 5*PI/4) {


		//	// Facing downward
		//	RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_);

			// Facing downward
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
				RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_);
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)]));
			}

		//} else {


		//	// Facing left
		//	RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].ent_);

		//}

			// Facing left
			if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
				RemoveEntity(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].ent_);
				mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)]));
			}

		
	}

	// Place rail
	if (input_->KeyWasPressed('R')) {
		
		// Get normalized player pos
		Vecf3 norm_player_pos = player_->GetPosition();
		norm_player_pos.x = (int)round(player_->GetPosition().x) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);
		
		// Check if rail can be placed on player pos
		if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x)].walkable_ && 
			mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_ != ResourceBlockType::Rail) {
			
			// Spawn rail
			std::shared_ptr<Block> r = std::make_shared<Rail>("rail", graphics_, input_, rl_);
			r->SetScale({ 0.5, 0.03125, 0.5 });
			r->SetPosition(Vecf3((int)round(norm_player_pos.x), -0.5f, (int)round(norm_player_pos.z)));
			
			AddEntity(r);
			mapGen_->AddResource({ ResourceBlockType::Rail, 0, 1, r });

		}


	}

	isCollideWithPlayer();
}

void Level::Render(const float& dt)
{
	Scene::Render(dt);
}

void Level::SpawnRandomBlocks(const int& val)
{
	// get random value between -5 and 5
	float x_rand = float(-val + (rand() % val + 1));
	float z_rand = float(-val + (rand() % val + 1));
	float rand_size = float(rand() % 4 + 1);
	gravity_blocks_.push_back(AddSolidBlock("grassblock", { x_rand, 8.0f, z_rand }, { rand_size, 1.0f, rand_size }, 5.0f * rand_size));
}

bool Level::isCollideWithPlayer()
{
	//int chunkSize = mapGen_->GetChunkSize().x * mapGen_->GetChunkSize().y;
	//for (int i = 1; i < chunkSize; i++)
	//{
	//	//switch (static_cast<int>((resourceTileData_ + i)->block_type_))
	//	//{
	//	//case 0:
	//	//	OutputDebugString("Rock");
	//	//case 1:
	//	//	OutputDebugString("Tree");
	//	//case 2:
	//	//	OutputDebugString("Rail");
	//	//case 3:
	//	//	OutputDebugString("Air");
	//	//}

	//			
	//	if ((resourceTileData_ + i)->ent_ != nullptr)
	//	{
	//		if ((resourceTileData_ + i)->ent_->AABB2dCollision(std::dynamic_pointer_cast<Entity>(player_)))
	//		{
	//			//std::stringstream ss;
	//			//ss << (resourceTileData_ + i)->ent_->GetPosition().x << ", " << (resourceTileData_ + i)->ent_->GetPosition().y << ", " << (resourceTileData_ + i)->ent_->GetPosition().z << "\n";
	//			//OutputDebugString(ss.str().c_str());
	//			//

	//				
	//			return(true);
	//		}
	//		else
	//		{
	//			//OutputDebugString("Not Colliding\n");
	//		}
	//	}
	//}
	//return (false);
	
	player_->player_touch_left = false;
	player_->player_touch_right = false;
	player_->player_touch_front = false;
	player_->player_touch_back = false;

	int chunkSize = mapGen_->GetChunkSize().x * mapGen_->GetChunkSize().y;
	for (int i = 0; i < chunkSize; i++)
	{
		if ((resourceTileData_ + i)->ent_ != nullptr)
		{

			//bool blockRight = round((resourceTileData_ + i)->ent_->GetPosition().x) == round(player_->GetPosition().x) + 1 && round((resourceTileData_ + i)->ent_->GetPosition().z) == round(player_->GetPosition().z);
			//bool blockLeft = round((resourceTileData_ + i)->ent_->GetPosition().x) == round(player_->GetPosition().x) - 1 && round((resourceTileData_ + i)->ent_->GetPosition().z) == round(player_->GetPosition().z);
			//bool blockFront = round((resourceTileData_ + i)->ent_->GetPosition().x) == round(player_->GetPosition().x) && round((resourceTileData_ + i)->ent_->GetPosition().z) == round(player_->GetPosition().z) + 1;
			//bool blockBack = round((resourceTileData_ + i)->ent_->GetPosition().x) == round(player_->GetPosition().x) && round((resourceTileData_ + i)->ent_->GetPosition().z) == round(player_->GetPosition().z) - 1;
			//bool blockFrontLeft = blockFront & blockLeft;
			//bool blockFrontRight = blockFront & blockRight;
			//bool blockBackRight = blockBack & blockRight;
			//bool blockBackLeft = blockBack & blockRight;

			bool blockRight = (resourceTileData_ + i)->ent_->GetPosition().x <= player_->GetPosition().x + 1.5 && (resourceTileData_ + i)->ent_->GetPosition().x >= player_->GetPosition().x + 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z <= player_->GetPosition().z +0.5  && (resourceTileData_ + i)->ent_->GetPosition().z >= player_->GetPosition().z - 0.5;
			bool blockLeft = (resourceTileData_ + i)->ent_->GetPosition().x >= player_->GetPosition().x - 1.5 && (resourceTileData_ + i)->ent_->GetPosition().x <= player_->GetPosition().x - 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z <= player_->GetPosition().z + 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z >= player_->GetPosition().z - 0.5;
			bool blockFront = (resourceTileData_ + i)->ent_->GetPosition().x >= player_->GetPosition().x - 0.5 && (resourceTileData_ + i)->ent_->GetPosition().x <= player_->GetPosition().x + 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z >= player_->GetPosition().z + 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z <= player_->GetPosition().z + 1.5;
			bool blockBack = (resourceTileData_ + i)->ent_->GetPosition().x >= player_->GetPosition().x - 0.5 && (resourceTileData_ + i)->ent_->GetPosition().x <= player_->GetPosition().x + 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z <= player_->GetPosition().z - 0.5 && (resourceTileData_ + i)->ent_->GetPosition().z >= player_->GetPosition().z - 1.5;
			if (blockFront || blockBack || blockLeft || blockRight)
			{
				if ((player_->AABB2dCollision((resourceTileData_ + i)->ent_)))
				{ 
					
					std::stringstream ss;
					if (blockFront/* || blockFrontLeft || blockFrontRight*/)
					{
						
						player_->player_touch_front = true;
						ss << "Front\n";
					}
					if (blockBack /*|| blockBackRight || blockBackLeft*/)
					{
						player_->player_touch_back = true;
						ss << "Back\n";
					}
					if (blockRight/* || blockFrontRight || blockBackRight*/)
					{
						player_->player_touch_right = true;
						ss << "Right\n";
					}
					if (blockLeft /*|| blockFrontLeft || blockBackLeft*/)
					{
						player_->player_touch_left = true;
						ss << "Left\n";
					}
					OutputDebugString(ss.str().c_str());
					return true;
				}
			}
		}
	}
	return false;

	//int chunkSize = mapGen_->GetChunkSize().x * mapGen_->GetChunkSize().y;
	//for (int i = 0; i < chunkSize; i++)
	//{
	//	if ((resourceTileData_ + i)->ent_ != nullptr)
	//	{
	//		if ((round(player_->GetPosition().x)) == round((resourceTileData_ + i)->ent_->GetPosition().x) && (round(player_->GetPosition().z)) == round((resourceTileData_ + i)->ent_->GetPosition().z))
	//		{
	//			MapGenerator::ResourceTileData* current_rtd_ = (resourceTileData_ + i);
	//			MapGenerator::ResourceTileData** rtd_around = mapGen_->GetTilesAround(current_rtd_);
	//			//bool something = rtd_around[0]->ent_->AABB2dCollision(player_);
	//			for (i = 0; i < 8; i++)
	//			{
	//				if (rtd_around[i]->ent_ != nullptr)
	//				{
	//					if (rtd_around[i]->ent_->AABB2dCollision(player_))
	//					{
	//						OutputDebugString("Colliding");
	//						return true;

	//					}
	//				}
	//				else
	//				{
	//					continue;
	//				}

	//			}
	//		}
	//	}
	//}
	//return false;

}