#include "MapGenerator.h"

#include "Block.h"
#include "Rail.h"
#include "ChooChoo.h"

MapGenerator::MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Scene* scene)
	:
	pn_(new PerlinNoise()),
	graphics_(graphics),
	input_(input),
	rng_(rd_()),
	dist(1, chunk_height_-2),
	rl_(rl),
	scene_(scene)
{
}

void MapGenerator::GenerateMap()
{
	// Update resource & ground data
	if (total_map_size_ >= 3)
	{
		// Remove old entities
		for (int z = 0; z < chunk_height_; z++)
		{
			for (int x = 0; x < chunk_width_; x++)
			{
				if (resource_data_[3*chunk_width_ * z + x].ent_) { scene_->RemoveEntity(resource_data_[3*chunk_width_ * z + x].ent_); }
				if (ground_data_[3*chunk_width_ * z + x].ent_) { scene_->RemoveEntity(ground_data_[3*chunk_width_ * z + x].ent_); }
			}
		}

		// Pull forward resource & ground data 
		for (int z = 0; z < chunk_height_; z++)
		{
			for (int x = 0; x < 2*chunk_width_; x++)
			{
				resource_data_[3*chunk_width_ * z + x] = resource_data_[3*chunk_width_ * z + (x + chunk_width_)];
				ground_data_[3*chunk_width_ * z + x] = ground_data_[3*chunk_width_ * z + (x + chunk_width_)];
			}
		}
	}

	// Generate checkpoint
	Vecf3 checkpoint = Vecf3((total_map_size_ * chunk_width_) + 2, 0, dist(rng_));

	Vecf3 dim;
	std::shared_ptr<Block> b;

	double n;
	for (int z = 0; z < chunk_height_; z++)
	{
		for (int x = total_map_size_ * chunk_width_; x < (total_map_size_ * chunk_width_) + chunk_width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) {

				// Spawn block & set ground_data_
				b = scene_->AddBlock("startblock", Vecf3(x, -1.0, z), Vecf3(0.5, 0.5, 0.5));
				ground_data_[3*chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { GroundBlockType::Checkpoint, true, b };

			}
			else {

				// Spawn block & set ground_data_
				b = scene_->AddBlock("grassblock", Vecf3(x, -1.0, z), Vecf3(0.5, 0.5, 0.5));
				ground_data_[3*chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { GroundBlockType::Grass, true, b };

			}

			// Generate noise value for current block
			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			if ((z - checkpoint.z) == 0 && abs(x - checkpoint.x) <= 1) {

				// temp rails
				std::shared_ptr<Rail> rail = std::make_shared<Rail>("rail", graphics_, input_, rl_);
				rail->SetScale({ 0.5, 0.03125, 0.5 });
				rail->SetPosition(Vecf3(x, -0.5f, z));
				AddResource({ ResourceBlockType::Rail, 0, 1, rail });

			} else if (abs(z - checkpoint.z) <= 1 && abs(x - checkpoint.x) <= 1|| n > .4 && n < .6) {

				// Set resource_data_
				resource_data_[3*chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Air, 0, 1, nullptr };

			}
			else if (n < .4) {

				// Spawn tree & set resource_data_
				dim = rl_->GetDimensions("tree");
				b = scene_->AddModel("tree", Vecf3(x, -0.5, z), Vecf3(1 / dim.x, 1.3 / dim.y, 1 / dim.z), 1);
				resource_data_[3*chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Tree, 1, 0, b };

			}
			else {

				// Spawn rock & set resource_data_
				dim = rl_->GetDimensions("rock");
				b = scene_->AddModel("rock", Vecf3(x, -0.5, z), Vecf3(1 / dim.x, .8 / dim.y, 1 / dim.z), 1);
				resource_data_[3*chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Rock, 1, 0, b };

			}
		}
	}

	// Spawn train in first chunk
	if (total_map_size_ == 0) {
		std::shared_ptr<Entity> train = std::dynamic_pointer_cast<Entity>(std::make_shared<ChooChoo>("train", graphics_, input_, rl_, this));
		train->SetDrawMode(2);
		train->GetCube().SetScaleX(0.0005f);
		train->GetCube().SetScaleY(0.0005f);
		train->GetCube().SetScaleZ(0.0005f);
		train->GetCube().SetAngleXDeg(-90);
		train->GetCube().SetAngleYDeg(90);
		train->SetPosition({ checkpoint.x - 1.0f, -0.5, checkpoint.z });
		scene_->AddEntity(train);
	}

	// Increment total_map_size_
 	total_map_size_++;
}

bool MapGenerator::CanAddRail(std::shared_ptr<Rail> r)
{
	// Check if rail can be placed on rail pos
	auto tile = GetResourceTileData()[(int)(round(r->GetPosition().z) * GetMapSize().x + r->GetPosition().x)];
	if (tile.block_type_ == ResourceBlockType::Air) {

		if (GetLastRail() == nullptr) {
			rails_.push_back(r);
			return true;
		}
		else if (abs(GetLastRail()->GetPosition().x - r->GetPosition().x) + abs(GetLastRail()->GetPosition().z - r->GetPosition().z) <= 1) {

			if ((int)round(GetLastRail()->GetPosition().z - r->GetPosition().z) == 1) {
				// Last rail is above 
				// Place vertical rail
				r->SetDirection(Rail::Direction::Vertical);

				// Make correction to second last rail (if needed)
				if (GetLastRail()->GetDirection() == Rail::Direction::Horizontal) {
					if (GetSecondLastRail() == nullptr) {
						GetLastRail()->SetDirection(Rail::Direction::Vertical);
					}
					else {
						if ((GetSecondLastRail()->GetPosition().x - GetLastRail()->GetPosition().x) == -1) {
							GetLastRail()->SetDirection(Rail::Direction::LBottomCurved);
						}
						else {
							GetLastRail()->SetDirection(Rail::Direction::RBottomCurved);
						}
					}
				}
			}
			else if ((int)round(GetLastRail()->GetPosition().z - r->GetPosition().z) == -1) {
				// Last rail is below
				// Place vertical rail
				r->SetDirection(Rail::Direction::Vertical);

				// Make correction to second last rail (if needed)
				if (GetLastRail()->GetDirection() == Rail::Direction::Horizontal) {
					if (GetSecondLastRail() == nullptr) {
						GetLastRail()->SetDirection(Rail::Direction::Vertical);
					}
					else {
						if ((GetSecondLastRail()->GetPosition().x - GetLastRail()->GetPosition().x) == -1) {
							GetLastRail()->SetDirection(Rail::Direction::LTopCurved);
						}
						else {
							GetLastRail()->SetDirection(Rail::Direction::RTopCurved);
						}
					}
				}
			}
			else if ((int)round(GetLastRail()->GetPosition().x - r->GetPosition().x) == 1) {
				// Last rail is on the right
				// Place horizontal rail
				r->SetDirection(Rail::Direction::Horizontal);

				// Make correction to second last rail (if needed)
				if (GetLastRail()->GetDirection() == Rail::Direction::Vertical) {
					if (GetSecondLastRail() == nullptr) {
						GetLastRail()->SetDirection(Rail::Direction::Horizontal);
					}
					else {
						if ((GetSecondLastRail()->GetPosition().z - GetLastRail()->GetPosition().z) == -1) {
							GetLastRail()->SetDirection(Rail::Direction::LBottomCurved);
						}
						else {
							GetLastRail()->SetDirection(Rail::Direction::LTopCurved);
						}
					}
				}
			}
			else if ((int)round(GetLastRail()->GetPosition().x - r->GetPosition().x) == -1) {
				// Last rail is on the left
				// Place horizontal rail
				r->SetDirection(Rail::Direction::Horizontal);

				// Make correction to second last rail (if needed)
				if (GetLastRail()->GetDirection() == Rail::Direction::Vertical) {
					if (GetSecondLastRail() == nullptr) {
						GetLastRail()->SetDirection(Rail::Direction::Horizontal);
					}
					else {
						if ((GetSecondLastRail()->GetPosition().z - GetLastRail()->GetPosition().z) == -1) {
							GetLastRail()->SetDirection(Rail::Direction::RBottomCurved);
						}
						else {
							GetLastRail()->SetDirection(Rail::Direction::RTopCurved);
						}
					}
				}
			}
			else { return false; }

 			rails_.push_back(r);
			return true;

		}
	}
	return false;
}

MapGenerator::ResourceTileData** MapGenerator::GetTilesAround(ResourceTileData* starting_tile) {

	// Create return value & fill with nullptrs
	ResourceTileData* retval[8] = { nullptr };

	// Get (normalized) starting_tile location
	int starting_tile_index = std::distance(resource_data_, starting_tile);
	int starting_tile_x = starting_tile_index % chunk_width_;
	int starting_tile_y = (starting_tile_index - starting_tile_x) / (chunk_width_ * 3);

	// Get tiles surrounding starting_tile
	if (starting_tile_y <= chunk_height_) { 
		// North
		retval[0] = starting_tile + (chunk_width_ * 3); 

		if (starting_tile_x <= chunk_width_ * 3) {
			// North-East
			retval[1] = starting_tile + chunk_width_ * 3 + 1;
		}
	}

	if (starting_tile_x <= chunk_width_ * 3) {
		// East
		retval[2] = starting_tile + 1;

		if (starting_tile_y > 0) {
			// South-East
			retval[3] = starting_tile - chunk_width_ * 3 + 1;
		}
	}

	if (starting_tile_y > 0) {
		// South
		retval[4] = starting_tile - chunk_width_ * 3;

		if (starting_tile_x > 0) {
			// South-West
			retval[5] = starting_tile - chunk_width_ * 3 - 1;
		}
	}
	
	if (starting_tile_x > 0) {
		// West
		retval[6] = starting_tile - 1;

		if (starting_tile_y <= chunk_height_) {
			// North-West
			retval[7] = starting_tile + chunk_width_ * 3 - 1;
		}
	}

	return retval;
}

MapGenerator::ResourceTileData& MapGenerator::GetCurrentTile(const Vecf3& pos)
{
	int index = (int)(round(pos.z) * chunk_width_ * 3 + round(pos.x));
	return resource_data_[index];
}

void MapGenerator::AddResource(ResourceTileData tile)
{
	if (tile.block_type_ == ResourceBlockType::Rail && !CanAddRail(std::dynamic_pointer_cast<Rail>(tile.ent_))) { return; }

	Vecf3 tile_pos = tile.ent_->GetPosition();
	int index = (int)(round(tile_pos.z) * chunk_width_ * 3 + round(tile_pos.x));
	resource_data_[index] = tile;

	scene_->AddEntity(tile.ent_);
}

void MapGenerator::RemoveResource(ResourceTileData* tile)
{
	*(tile) = { ResourceBlockType::Air, 0, 1, nullptr };;
}