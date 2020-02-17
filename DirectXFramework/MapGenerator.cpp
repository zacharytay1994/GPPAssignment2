#include "MapGenerator.h"

#include "Block.h"
#include "Rail.h"
#include "ChooChoo.h"

#include <time.h>

MapGenerator::MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Scene* scene)
	:
	pn_(new PerlinNoise(rand()%100)),
	fn_(new FastNoise(rand() % 100)),
	graphics_(graphics),
	input_(input),
	rng_(rd_()),
	dist(1, chunk_height_-2),
	rl_(rl),
	scene_(scene)
{
	fn_->SetFrequency(.3);
	fn_->SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Natural);

	block_pool_ = ObjectPool<Block, 1152>::getInstance(graphics_, input_, rl_);
	rail_pool_ = ObjectPool<Rail, 1152>::getInstance(graphics_, input_, rl_);
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
				ResourceTileData tile = resource_data_[3 * chunk_width_ * z + x];
				if (tile.ent_) 
				{
					switch (tile.block_type_)
					{
						case ResourceBlockType::Rail:
							rail_pool_->Release(std::dynamic_pointer_cast<Rail>(tile.ent_));
						default:
							block_pool_->Release(std::dynamic_pointer_cast<Block>(tile.ent_));
					}
				}
				if (ground_data_[3 * chunk_width_ * z + x].ent_)
				{
					std::shared_ptr<Block> b = std::dynamic_pointer_cast<Block>(ground_data_[3 * chunk_width_ * z + x].ent_);
					if (b != nullptr) block_pool_->Release(b);
				}
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

	float n;
	float vn;
	for (int z = 0; z < chunk_height_; z++)
	{
		for (int x = total_map_size_ * chunk_width_; x < (total_map_size_ * chunk_width_) + chunk_width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) {

				// Spawn block & set ground_data_
				b = block_pool_->Acquire("startblock");
				if (total_map_size_ == 0) { b->SetPosition(Vecf3(x, -1.0, z)); }
				else
				{
					b->SetPosition(Vecf3(x, -(x % chunk_width_), z));
					b->MoveTo(Vecf3(x, -1.0, z));
				}
				scene_->AddEntity(b);
				ground_data_[3*chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { GroundBlockType::Checkpoint, true, std::dynamic_pointer_cast<Entity>(b) };

			}
			else {

				// Spawn block & set ground_data_
				dim = rl_->GetDimensions("gndblk");
				b = block_pool_->Acquire("gndblk");
				b->SetDrawMode(3);
				if (total_map_size_ == 0) { b->SetPosition(Vecf3(x, -1.5f, z)); }
				else
				{
					b->SetPosition(Vecf3(x, -(x % chunk_width_), z));
					b->MoveTo(Vecf3(x, -1.5f, z));
				}
				b->SetScale(Vecf3(1 / dim.x, 1 / dim.y, 1 / dim.z));
				scene_->AddEntity(b);
				ground_data_[3 * chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { GroundBlockType::Ground, true, std::dynamic_pointer_cast<Entity>(b) };

			}

			// Generate perlin noise value for current block
			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			if ((z - checkpoint.z) == 0 && abs(x - checkpoint.x) <= 1) {

				// temp rails
				std::shared_ptr<Rail> r = rail_pool_->Acquire("rail");
				r->SetScale(Vecf3(0.5, 0.03125, 0.5));
				r->SetPosition(Vecf3(x, -0.5f, z));
				AddResource({ ResourceBlockType::Rail, 0, 1, r });

			}
			else if (abs(z - checkpoint.z) <= 1 && abs(x - checkpoint.x) <= 1 || n > .4 && n < .6) {

				// Set resource_data_
				resource_data_[3 * chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Air, 0, 1, nullptr };

			}
			else {

				// Generate voronoi noise value for current block
				vn = fn_->GetCellular(x, z);

				if (vn < -.3) 
				{
					// Spawn tree & set resource_data_
					dim = rl_->GetDimensions("tree");
					b = block_pool_->Acquire("tree");
					b->SetDrawMode(3);
					if (total_map_size_ == 0) { b->SetPosition(Vecf3(x, -0.5f, z)); }
					else
					{
						b->SetPosition(Vecf3(x, (x % chunk_width_), z));
						b->MoveTo(Vecf3(x, -0.5f, z));
					}
					b->SetScale(Vecf3(1 / dim.x, .9 / dim.y, 1 / dim.z));
					scene_->AddEntity(b);
					resource_data_[3 * chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Tree, 1, 0, b };
				}
				else if (vn < .1) 
				{
					// Spawn unbreakable rock & set resource_data_
					dim = rl_->GetDimensions("unbreakablerock");
					b = block_pool_->Acquire("unbreakablerock");
					b->SetDrawMode(3);
					if (total_map_size_ == 0) { b->SetPosition(Vecf3(x, -0.5f, z)); }
					else
					{
						b->SetPosition(Vecf3(x, (x % chunk_width_), z));
						b->MoveTo(Vecf3(x, -0.5f, z));
					}
					b->SetScale(Vecf3(1 / dim.x, .9 / dim.y, 1 / dim.z));
					scene_->AddEntity(b);
					resource_data_[3 * chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Unbreakable, 0, 0, b };
				}
				else 
				{
					// Spawn rock & set resource_data_
					dim = rl_->GetDimensions("rock");
					b = block_pool_->Acquire("rock");
					b->SetDrawMode(3);
					if (total_map_size_ == 0) { b->SetPosition(Vecf3(x, -0.5f, z)); }
					else 
					{
						b->SetPosition(Vecf3(x, (x % chunk_width_), z));
						b->MoveTo(Vecf3(x, -0.5f, z));
					}
					b->SetScale(Vecf3(1 / dim.x, .8 / dim.y, 1 / dim.z));
					scene_->AddEntity(b);
					resource_data_[3 * chunk_width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * chunk_width_) : x)] = { ResourceBlockType::Rock, 1, 0, b };
				}

			}
		}
	}

	// Spawn train in first chunk
	if (total_map_size_ == 0) {
		/*std::shared_ptr<Entity> train = std::dynamic_pointer_cast<Entity>(std::make_shared<ChooChoo>("train", graphics_, input_, rl_, this));
		train->SetDrawMode(2);
		train->GetCube().SetScaleX(0.0005f);
		train->GetCube().SetScaleY(0.0005f);
		train->GetCube().SetScaleZ(0.0005f);
		train->GetCube().SetAngleXDeg(-90);
		train->GetCube().SetAngleYDeg(90);
		train->SetPosition({ checkpoint.x, -0.5, checkpoint.z });
		scene_->AddEntity(train);
		train_ = train;*/
		std::shared_ptr<ChooChoo> train = std::make_shared<ChooChoo>("enginecart", graphics_, input_, rl_, this, scene_->ps_);
		train->SetDrawMode(3);
		train->GetCube().SetScaleX(0.05f);
		train->GetCube().SetScaleY(0.05f);
		train->GetCube().SetScaleZ(0.05f);
		//train->GetCube().SetAngleXDeg(-90);
		//train->GetCube().SetAngleYDeg(90);
		train->SetPosition({ checkpoint.x, -0.5 + 0.03125, checkpoint.z });
		train_ = std::dynamic_pointer_cast<Entity>(train);
		scene_->AddEntity(train_);
		

		
		train->SetChildChoo(std::make_shared<ChooChoo>("craftingcart", graphics_, input_, rl_, this, scene_->ps_),"crafter");
		train = train->GetChildChoo("crafter");
		train->SetDrawMode(3);
		train->GetCube().SetScaleX(0.05f);
		train->GetCube().SetScaleY(0.05f);
		train->GetCube().SetScaleZ(0.05f);
		train->SetPosition({ checkpoint.x - 1.25f, -0.5 + 0.03125, checkpoint.z });
		crafter_ = std::dynamic_pointer_cast<Entity>(train);
		scene_->AddEntity(crafter_);
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
			else 
			{ 
				rail_pool_->Release(r);
				return false; 
			}

 			rails_.push_back(r);
			return true;

		}
	}
	rail_pool_->Release(r);
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

MapGenerator::ResourceTileData* MapGenerator::GetCurrentTilePtr(const Vecf3& pos)
{
	Vecf3 norm_pos = pos;
	norm_pos.x = (int)round(pos.x) - (std::max)(0, (total_map_size_ - 3) * chunk_width_);
	norm_pos.z = (int)round(pos.z);
	int index = (int)(round(norm_pos.z) * chunk_width_ * 3 + round(norm_pos.x));
	return &(resource_data_[index]);
}

MapGenerator::ResourceTileData& MapGenerator::GetCurrentTile(const Vecf3& pos)
{
	Vecf3 norm_pos = pos;
	norm_pos.x = (int)round(pos.x) - (std::max)(0, (total_map_size_ - 3) * chunk_width_);
	norm_pos.z = (int)round(pos.z);
	int index = (int)(round(norm_pos.z) * chunk_width_ * 3 + round(norm_pos.x));
	return resource_data_[index];
}

Veci2 MapGenerator::GetCurrentIndex(const Vecf3& pos)
{
	Vecf3 norm_pos = pos;
	norm_pos.x = (int)round(pos.x) - (std::max)(0, (total_map_size_ - 3) * chunk_width_);
	norm_pos.z = (int)round(pos.z);
	return { (int)norm_pos.x, (int)norm_pos.z };
}

Vecf3 MapGenerator::GetWorldPosOfIndex(const Veci2& index)
{
	return { (float)index.x + (total_map_size_ > 3 ? total_map_size_ - 3 : 0)* chunk_width_, 1.0f, (float)index.y };
}

bool MapGenerator::AddResource(ResourceTileData tile)
{
	if (tile.block_type_ == ResourceBlockType::Rail && !CanAddRail(std::dynamic_pointer_cast<Rail>(tile.ent_))) { return false; }

	Vecf3 tile_pos = tile.ent_->GetPosition();
	int index = (int)(round(tile_pos.z) * chunk_width_ * 3 + round(tile_pos.x));
	resource_data_[index] = tile;
	
	scene_->AddEntity(tile.ent_);

	return true;
}

void MapGenerator::RemoveResource(ResourceTileData* tile)
{
	block_pool_->Release(std::dynamic_pointer_cast<Block>(tile->ent_));
	*(tile) = { ResourceBlockType::Air, 0, 1, nullptr };;
}