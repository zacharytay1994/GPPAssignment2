#include "Level.h"
#include "Block.h"
#include "CollisionComponent.h"

#include <sstream>
#include <string>

#include "ChooChoo.h"
#include "InputComponent.h"


#include <cmath>
#include <sstream>

Level::Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game)
	:
	Scene(gfx, input, rl, game),
	mapGen_(std::make_unique<MapGenerator>(graphics_, input_, rl_, this)),
	pathfinder_(std::make_shared<AStarPathfinding>(mapGen_.get()))
{
	// Generate initial chunk
	mapGen_->GenerateMap();
	mapGen_->GenerateMap();

	enemy1_ = AddEnemy({ 5.0f, 0.0f, 0.0f }, { 0.5f, 0.5f , 0.5f }, pathfinder_, mapGen_.get());

	// Create player
	player_ = AddPlayer({ 0.0f, 1.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });
	player_->AddComponent(std::make_shared<InputComponent>(InputComponent(*player_, *input_, 'W', 'S', 'A', 'D', VK_LSHIFT)));
	player_->SetDrawTarget(true);
	player2_ = AddPlayer({ 0.0f, 1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f });
	player2_->AddComponent(std::make_shared<InputComponent>(InputComponent(*player2_, *input_, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_RSHIFT)));
	player2_->active_ = false;
	player2_->SetDrawTarget(true);
	player2_->SetPlayer2();
	//player_->AddComponent(std::make_shared<InputComponent>(InputComponent(*player_, *input_)));

	//target_block_ = AddBlock("highlight", player_->GetPosition(), {  });
	//TODO: add player2 target_block
}

void Level::Update(const float& dt)
{
	Scene::Update(dt);
	ps_.Update(dt);
	gui_.Update(dt);
	ps_.SetCameraSuckPosition(!game_over_ ? (input_->GetCameraPosition() + Vecf3(0.0f, 1.2f, 0.0f)) : (mapGen_->train_->GetPosition() + Vecf3(0.0f, 10.0f, 0.0f)));
	gui_.SetTrainX(std::dynamic_pointer_cast<ChooChoo>(mapGen_->train_)->GetPosition().x);


	//enemy1_->SetDestination(player_->GetPosition());

	// <!-- crafting cooldown
	if (crafting_cooldown_timer > 0) {
		crafting_cooldown_timer -= dt;
	} // -->

	// <!-- highlight player target block

	// -->

	wl_.SetPoint1(player_->GetPosition() + RotateVectorY(Vecf3(0.0f, 0.0f, 1.0f), -player_->GetOrientation().y) * 2.0f + Vecf3(0.0f, 1.0f, 0.0f));
	wl_.SetPoint2(mapGen_->train_->GetPosition() + RotateVectorY(Vecf3(0.0f, 0.0f, 1.0f), -(mapGen_->train_->GetCube().GetAngleY() + 1.57079f)) * 2.0f + Vecf3(0.0f, 1.0f, 0.0f));
	if (multiplayer_) {
		wl_.SetPoint3(player2_->GetPosition() + RotateVectorY(Vecf3(0.0f, 0.0f, 1.0f), -player2_->GetOrientation().y) * 2.0f + Vecf3(0.0f, 1.0f, 0.0f));
	}
	else {
		wl_.SetPoint3({0.0f, -1000.0f, 0.0f});
	}

	// SET MULTIPLAYER MODE
	if (input_->KeyWasPressed('M')) {
		multiplayer_ = !multiplayer_;
		if (multiplayer_) {
			player2_->active_ = true;
			player2_->SetPosition(player_->GetPosition());
		}
		else {
			player2_->active_ = false;
		}
	}
	/*__________________________________*/
	// CAMERA TRACKING MODE
	/*__________________________________*/
	if (multiplayer_) { camera_mode_ = 2; }
	Vecf3 cam_to_target;
	if (camera_mode_ != 0) {
		input_->EngageCamera(false);
	}
	else {
		input_->EngageCamera(true);
	}
	switch (camera_mode_) {
	case 0: // free roam
		break;
	case 1: // third person
		cam_to_target = ((player_->GetPosition() + Vecf3(0.0f, 3.0f, -4.0f)) - input_->GetCameraPosition());
		if (!(cam_to_target.LenSq() < 0.1f)) {
			input_->TranslateCamera({ cam_to_target.x, cam_to_target.y, cam_to_target.z }, dt);
		}
		if (abs(0.60f - input_->GetCamPitch()) > 0.05f) {
			input_->SetCamPitch(input_->GetCamPitch() + (0.60f - input_->GetCamPitch()) * dt * 2.0f);
		}
		break;
	case 2: // top down
		if (multiplayer_) {
			Vecf3 center = player2_->GetPosition() - player_->GetPosition();
			float distance = center.Len();
			center = player_->GetPosition() + (center / 2.0f);
			cam_to_target = ((center + Vecf3(0.0f, 5.0f + distance * 0.5f, -2.0f - distance * 0.1f)) - input_->GetCameraPosition());
		}
		else {
			cam_to_target = ((player_->GetPosition() + Vecf3(0.0f, 8.0f, -2.0f)) - input_->GetCameraPosition());
		}
		if (!(cam_to_target.LenSq() < 0.1f)) {
			input_->TranslateCamera({ cam_to_target.x, cam_to_target.y, cam_to_target.z }, dt);
		}
		if (abs(1.20f - input_->GetCamPitch()) > 0.05f) {
			input_->SetCamPitch(input_->GetCamPitch() + (1.20f - input_->GetCamPitch()) * dt * 2.0f);
		}
		break;
	case 3: // follow train
		Vecf3 tempvec = (mapGen_->train_->GetPosition() + Vecf3(0.0f, 10.0f, -5.0f));
		tempvec.z = 4.0f;
		cam_to_target = (tempvec - input_->GetCameraPosition());
		if (!(cam_to_target.LenSq() < 0.1f)) {
			input_->TranslateCamera({ cam_to_target.x, cam_to_target.y, cam_to_target.z }, dt);
		}
		if (abs(1.40f - input_->GetCamPitch()) > 0.05f) {
			input_->SetCamPitch(input_->GetCamPitch() + (1.40f - input_->GetCamPitch()) * dt * 2.0f);
		}
		break;
	}
	/*__________________________________*/
	// CAMERA SWITCH MODE
	/*__________________________________*/
	if (input_->KeyWasPressed('P')) {
		camera_mode_ = (camera_mode_ + 1) > 3 ? 0 : (camera_mode_ + 1);
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

	PlayerLogic(VK_SPACE, 'R', player_);
	
	if (multiplayer_) { PlayerLogic(VK_CONTROL, 'L', player2_); } // VK_RCONTROL doesn't work for me...

	//TODO: compare with playerlogic()
	//// Collect resource if facing block & within 1 block
	//if (input_->KeyWasPressed('C')) {

	//	// Get normalized player pos
	//	Vecf3 norm_player_pos = player_->GetPosition();
	//	norm_player_pos.x = (int)round(player_->GetPosition().x) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);
	//	norm_player_pos.z = (int)round(player_->GetPosition().z) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);

	//	// Get player heading & check if there is a block in front of the player
	//	float y_rot = fmod(player_->GetOrientation().y > 0 ? player_->GetOrientation().y : player_->GetOrientation().y + 2*PI, 2*PI);
	//	if ((y_rot >= 7*PI/4) || (y_rot <= PI/4)) {

	//		// Facing forward
	//		if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
	//			EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_,
	//				mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_->GetPosition());
	//			mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z + 1) * mapGen_->GetMapSize().x + norm_player_pos.x)]));
	//			player_->Punch();
	//		}

	//	} else if (y_rot <= 3*PI/4) {

	//		// Facing right
	//		if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].breakable_) {
	//			EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].block_type_,
	//				mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)].ent_->GetPosition());
	//			mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x + 1)]));
	//			player_->Punch();
	//		}
	//	}
	//	else if (y_rot <= 5 * PI / 4) {

	//		// Facing downward
	//		if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].breakable_) {
	//			EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_,
	//				mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)].ent_->GetPosition());
	//			mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z - 1) * mapGen_->GetMapSize().x + norm_player_pos.x)]));
	//			player_->Punch();
	//		}
	//	}
	//	else {

	//		// Facing left
	//		if (mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].breakable_) {
	//			EmitDestructionParticles(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].block_type_,
	//				mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)].ent_->GetPosition());
	//			mapGen_->RemoveResource(&(mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x - 1)]));
	//			player_->Punch();
	//		}
	//	}
	//}
	//// Place or craft rail
	//if (input_->KeyWasPressed('R')) {
	//	
	//	// Get normalized player pos
	//	//Vecf3 norm_player_pos = player_->GetPosition();
	//	Vecf3 norm_player_pos = target_block_->GetPosition(); // replaced player with target_block to place rail in front of player.
	//	norm_player_pos.x = (int)round(player_->GetPosition().x) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);
	//	norm_player_pos.z = (int)round(player_->GetPosition().z) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);

	//	// Check if rail can be placed on player pos
	//	if ((mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x)].walkable_ &&
	//		mapGen_->GetResourceTileData()[(int)(round(norm_player_pos.z) * mapGen_->GetMapSize().x + norm_player_pos.x)].block_type_ != ResourceBlockType::Rail) &&
	//		rail_count_ > 0) {

	//		// Spawn rail
	//		std::shared_ptr<Block> r = std::make_shared<Rail>("rail", graphics_, input_, rl_);
	//		r->SetScale({ 0.5f, 0.03125f, 0.5f });
	//		r->SetPosition(Vecf3((int)round(norm_player_pos.x), -0.5f, (int)round(norm_player_pos.z)));
	//		if (mapGen_->AddResource({ ResourceBlockType::Rail, 0, 1, r })) {
	//			rail_count_ -= 1;
	//			gui_.AddResource({ 1.0f, 2, -1 });
	//		}
	//		else {
	//			CraftRails();
	//		}
	//	}
	//	else {
	//		CraftRails();
	//	}


	//}

	//enemy behavior
	if (enemy1_ != nullptr)
	{
		enemy1_->SetDestination(mapGen_->GetLastRail()->GetPosition());
		if (enemy1_->AABB2dCollision(mapGen_->GetLastRail(), 1.0f, 1.0f))
		{
			RemoveEntity(mapGen_->GetLastRail());
		}
		if (enemy1_->isDead())
		{
			RemoveEntity(enemy1_);
			enemy1_ = nullptr;
		}
	}

	//collision	
	entityCollideWithPlayer(mapGen_->train_, 0.8f, 0.8f, player_);
	entityCollideWithPlayer(mapGen_->crafter_, 0.8f, 0.8f, player_);
	if (entityCollideWithPlayer(std::dynamic_pointer_cast<Entity>(enemy1_), 0.5f, 0.5f, player_))
	{

	}
	resourceCollideWithPlayer(dt, player_);

	if (multiplayer_)
	{
		resourceCollideWithPlayer(dt, player2_);
		entityCollideWithPlayer(std::dynamic_pointer_cast<Entity>(enemy1_), 0.5f, 0.5f, player2_);
		entityCollideWithPlayer(mapGen_->train_, 0.8f, 0.8f, player2_);
		entityCollideWithPlayer(mapGen_->crafter_, 0.8f, 0.8f, player2_);
	}



}


void Level::Render(const float& dt)
{
	Scene::Render(dt);
	ps_.Render();
	gui_.DrawLevelHUD(dt);
	// is game over
	if (std::dynamic_pointer_cast<ChooChoo>(mapGen_->train_)->GameOver()) {
		graphics_->EnableTransparency(true);
		gui_.DrawSprite("blackoverlay", { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		graphics_->EnableTransparency(false);
		game_over_ = true;
		if (wl_.GetWorldLightScale() > 0.2f) {
			wl_.SetWorldLightScale(wl_.GetWorldLightScale() - dt / 3.0f);
		}
		else {
			wl_.SetWorldLightScale(0.1f);
		}
		gui_.DrawString({ -0.75f, 0.0f, 0.0f }, 0.1f, "GAME OVER");
		std::stringstream ss;
		ss << "Score:" << gui_.GetTrainX();
		gui_.DrawString({ -0.55f, -0.2f, 0.0f }, 0.08f, ss.str());
		gui_.DrawString({ -0.94f, -0.4f, 0.0f }, 0.035f, "- Press Enter To Main Menu -");

		if (input_->KeyWasPressed(VK_RETURN)) {
			ChangeScene("mainmenu");
		}
	}
	/*_____________________________________*/
	// RENDER ALL TRANSPARENT STUFF HERE
	// Transparent objects have to be rendered over everything else
	// in order to blend the transparent colour's object over the existing one
	/*_____________________________________*/
	graphics_->EnableTransparency(true);
	rl_->DrawTexturedPlane("blueoverlay", rl_->GetTransform({0.0f, -0.5f, 0.0f}, {2000.0f, 2000.0f, 1.0f}, { 1.57079632f, 0.0f, 0.0f}, input_));
	/*rl_->DrawTexturedCubeNorm("robin", rl_->GetTransform({ 0.0f, -0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, input_),
		rl_->GetModelTransform({ 0.0f, -0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }));*/
	//rl_->DrawTexturedCube("robin", rl_->GetTransform({ 0.0f, 0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, input_));
	graphics_->EnableTransparency(false);
}

void Level::SpawnRandomBlocks(const int& val)
{
	// get random value between -5 and 5
	float x_rand = float(-val + (rand() % val + 1));
	float z_rand = float(-val + (rand() % val + 1));
	float rand_size = float(rand() % 4 + 1);
	gravity_blocks_.push_back(AddSolidBlock("grassblock", { x_rand, 8.0f, z_rand }, { rand_size, 1.0f, rand_size }, 5.0f * rand_size));
}


void Level::resourceCollideWithPlayer(const float& dt, std::shared_ptr<Player> player)
{
	// Resource collision
	float player_x = player->GetPosition().x;
	float player_z = player->GetPosition().z;


	MapGenerator::ResourceTileData& blockRight_ = mapGen_->GetCurrentTile(Vecf3(player_x + 1.0f, -0.5f, player_z));
	MapGenerator::ResourceTileData& blockLeft_ = mapGen_->GetCurrentTile(Vecf3(player_x - 1.0f, -0.5f, player_z));
	MapGenerator::ResourceTileData& blockFront_ = mapGen_->GetCurrentTile(Vecf3(player_x, -0.5f, player_z + 1.0f));
	MapGenerator::ResourceTileData& blockBack_ = mapGen_->GetCurrentTile(Vecf3(player_x, -0.5f, player_z - 1.0f));

	// collide with world boundaries
	// right bound 0
	if ((player_x - 0.4f) < -0.5f) {
		float penetration = (player_x - 0.4f) - (-0.5f);
		player->SetPosition(player->GetPosition() - Vecf3(penetration, 0.0f, 0.0f));
	}
	// bottom bound
	if ((player_z - 0.4f) < -0.5f) {
		float penetration = (player_z - 0.4f) - (-0.5f);
		player->SetPosition(player->GetPosition() - Vecf3(0.0f, 0.0f, penetration));
	}
	// top bound
	if ((player_z + 0.4f) > 15.5f) {
		float penetration = (player_z + 0.4f) - (15.5f);
		player->SetPosition(player->GetPosition() - Vecf3(0.0f, 0.0f, penetration));
	}

	if (blockRight_.ent_ != nullptr && blockRight_.walkable_ == false && player->AABB2dCollision(blockRight_.ent_, 1.0f, 1.0f))
	{
		float penetration = (player->GetPosition().x + 0.4f) - (blockRight_.ent_->GetPosition().x - 0.5f);
		player->SetPosition(player->GetPosition() - Vecf3(penetration, 0.0f, 0.0f));
	}

	if (blockLeft_.ent_ != nullptr && blockLeft_.walkable_ == false && player->AABB2dCollision(blockLeft_.ent_, 1.0f, 1.0f))
	{
		float penetration = (blockLeft_.ent_->GetPosition().x + 0.5f) - (player->GetPosition().x - 0.4f);
		player->SetPosition(player->GetPosition() + Vecf3(penetration, 0.0f, 0.0f));
	}

	if (blockFront_.ent_ != nullptr && blockFront_.walkable_ == false && player->AABB2dCollision(blockFront_.ent_, 1.0f, 1.0f))
	{
		float penetration = (player->GetPosition().z + 0.4f) - (blockFront_.ent_->GetPosition().z - 0.5f);
		player->SetPosition(player->GetPosition() - Vecf3(0.0f, 0.0f, penetration));
	}

	if (blockBack_.ent_ != nullptr && blockBack_.walkable_ == false && player->AABB2dCollision(blockBack_.ent_, 1.0f, 1.0f))
	{
		float penetration = (blockBack_.ent_->GetPosition().z + 0.5f) - (player->GetPosition().z - 0.4f);
		player->SetPosition(player->GetPosition() + Vecf3(0.0f, 0.0f, penetration));
	}
}

bool Level::entityCollideWithPlayer(std::shared_ptr<Entity> ent_, float length, float width, std::shared_ptr<Player> player_)
{
	if (ent_ != nullptr)
		if (player_->AABB2dCollision(ent_, length, width))
		{
			if (player_->GetPosition().x +0.4f  < ent_->GetPosition().x  && (player_->GetPosition().z + 0.4f > ent_->GetPosition().z - width / 2.0f) && (player_->GetPosition().z - 0.4f < ent_->GetPosition().z + width / 2.0f))
			{
				float penetration = (player_->GetPosition().x + 0.4f) - (ent_->GetPosition().x - length / 2.0f);
				player_->SetPosition(player_->GetPosition() - Vecf3(penetration, 0.0f, 0.0f));
			}
			else if (player_->GetPosition().x -0.4f> ent_->GetPosition().x && (player_->GetPosition().z + 0.4f > ent_->GetPosition().z - width / 2.0f) && (player_->GetPosition().z - 0.4f < ent_->GetPosition().z + width / 2.0f))
			{
				float penetration = (ent_->GetPosition().x + length / 2.0f) - (player_->GetPosition().x - 0.4f);
				player_->SetPosition(player_->GetPosition() - Vecf3(-penetration, 0.0f, 0.0f));
			}
			else if (player_->GetPosition().z +0.4f < ent_->GetPosition().z && (player_->GetPosition().x + 0.4f > ent_->GetPosition().x - length / 2.0f) && (player_->GetPosition().x - 0.4f < ent_->GetPosition().x + length / 2.0f))
			{
				float penetration = (player_->GetPosition().z + 0.4f) - (ent_->GetPosition().z - width / 2.0f);
				player_->SetPosition(player_->GetPosition() - Vecf3(0.0f, 0.0f, penetration));
			}
			else if (player_->GetPosition().z - 0.4f > ent_->GetPosition().z && (player_->GetPosition().x + 0.4f > ent_->GetPosition().x - length / 2.0f) && (player_->GetPosition().x - 0.4f < ent_->GetPosition().x + length / 2.0f))
			{
				float penetration = (ent_->GetPosition().z + width / 2.0f) - (player_->GetPosition().z - 0.4f);
				player_->SetPosition(player_->GetPosition() - Vecf3(0.0f, 0.0f, -penetration));
			}

			return true;
		}
		else
		{
			return false;
		}
}




void Level::EmitDestructionParticles(const ResourceBlockType& type, const Vecf3& pos)
{
	switch (type)
	{
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

void Level::CraftRails(std::shared_ptr<Player> player)
{
	// TODO: check crafter nearby

	Vecf3 distance = player->GetPosition() - mapGen_->crafter_->GetPosition();

	distance.x = distance.x * distance.x;
	distance.z = distance.z * distance.z;

	// crafter in range
	// cooldown
	// wood & rock is enuf
	if (distance.x + distance.z < crafting_radius_ * crafting_radius_ && 
		crafting_cooldown_timer <= 0 && 
		wood_count_ >= wood_per_rail_ && rock_count_ >= rock_per_rail_) {


		rail_count_ += 1;
		gui_.AddResource({ 0.1f, 2, 1 });
		wood_count_ -= wood_per_rail_;
		gui_.AddResource({ 0.1f, 1, -wood_per_rail_ });
		rock_count_ -= rock_per_rail_;
		gui_.AddResource({ 0.1f, 0, -rock_per_rail_ });

		crafting_cooldown_timer = crafting_cooldown;
	}

}

void Level::PlayerLogic(const char& k1, const char& k2, std::shared_ptr<Player> player)
{
	// Get normalized player pos
	Vecf3 norm_player_pos = player->GetPosition();
	norm_player_pos.x = (int)round(player->GetPosition().x) - max(0, (mapGen_->GetTotalChunkNo() - 3) * mapGen_->GetChunkSize().x);
	norm_player_pos.z = (int)round(player->GetPosition().z);

	// Collect resource if facing block & within 1 block
	if (input_->KeyWasPressed(k1))
	{
		// Placeholder to ResourceTileData, to be used in rotation check
		//MapGenerator::ResourceTileData* tile;

		// Get player heading & check if there is a block in front of the player
		//float y_rot = fmod(player->GetOrientation().y > 0 ? player->GetOrientation().y : player->GetOrientation().y + 2 * PI, 2 * PI);
		//if ((y_rot >= 7 * PI / 4) || (y_rot <= PI / 4))
		//{
		//	// Facing forward
		//	tile = mapGen_->GetCurrentTilePtr(Vecf3(norm_player_pos.x, norm_player_pos.y, norm_player_pos.z + 1));
		//	
		//}
		//else if (y_rot <= 3 * PI / 4)
		//{
		//	// Facing right
		//	tile = mapGen_->GetCurrentTilePtr(Vecf3(norm_player_pos.x + 1, norm_player_pos.y, norm_player_pos.z));
		//}
		//else if (y_rot <= 5 * PI / 4)
		//{
		//	// Facing downward
		//	tile = mapGen_->GetCurrentTilePtr(Vecf3(norm_player_pos.x, norm_player_pos.y, norm_player_pos.z - 1));
		//}
		//else
		//{
		//	// Facing left
		//	tile = mapGen_->GetCurrentTilePtr(Vecf3(norm_player_pos.x - 1, norm_player_pos.y, norm_player_pos.z));
		//}

		Vecf3 player_target = { player->target_->GetX() , player->target_->GetY() ,player->target_->GetZ() };
		MapGenerator::ResourceTileData* tile = mapGen_->GetCurrentTilePtr(player_target);
		if (tile == nullptr) {
			return;
		}

		// break block
		if (tile->breakable_)
		{
			EmitDestructionParticles(tile->block_type_, tile->ent_->GetPosition());
			mapGen_->RemoveResource(tile);
			pathfinder_->GetGrid().SetGrid();
			player->Punch();
		}
		else if (tile->walkable_ && tile->block_type_ != ResourceBlockType::Rail && rail_count_ > 0) {
			// Spawn rail
			std::shared_ptr<Rail> r = ObjectPool<Rail, 1152>::getInstance(graphics_, input_, rl_)->Acquire("rail");
			r->SetScale(Vecf3(0.5f, 0.03125f, 0.5f));
			r->SetPosition(Vecf3((int)round(player_target.x), -0.5f, (int)round(player_target.z)));
			// attempt to place rail
			if (mapGen_->AddResource({ ResourceBlockType::Rail, 0, 1, r })) {
				rail_count_ -= 1;
				gui_.AddResource({ 1.0f, 2, -1 });
			} 
			else { // fail to place rail
				CraftRails(player);
			}
		} // no action available, attempt to craft rails
		else if (enemy1_ != nullptr &&
			!enemy1_->isDead() && 
			abs(enemy1_->GetPosition().x - player->GetPosition().x) < 1 && abs(enemy1_->GetPosition().z - player->GetPosition().z) < 1)
		{
			if (enemy1_->AABB2dCollision(player, 0.6f, 0.6f))
			{
				enemy1_->minusHealth();
			}
		}
		else {
			CraftRails(player);
		}

	}

	//// Place rail
	//if (input_->KeyWasPressed(k2))
	//{
	//	// Check if rail can be placed on player pos
	//	Vecf3 targetTile = { player->target_->GetX(), player->target_->GetY() ,player->target_->GetZ() };
	//	MapGenerator::ResourceTileData tile = mapGen_->GetCurrentTile(targetTile);
	//	if (tile.walkable_ && tile.block_type_ != ResourceBlockType::Rail && rail_count_ > 0)
	//	{
	//		// Spawn rail
	//		std::shared_ptr<Rail> r = std::make_shared<Rail>("rail", graphics_, input_, rl_);
	//		r->SetScale(Vecf3(0.5f, 0.03125f, 0.5f));
	//		r->SetPosition(Vecf3((int)round(targetTile.x), -0.5f, (int)round(targetTile.z)));
	//		if (mapGen_->AddResource({ ResourceBlockType::Rail, 0, 1, r })) {
	//			rail_count_ -= 1;
	//			gui_.AddResource({ 1.0f, 2, -1 });
	//		}
	//		else {
	//			CraftRails(player);
	//		}
	//		
	//	}
	//	else {
	//		CraftRails(player);
	//	}
	//}
}
