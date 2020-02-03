#include "Level.h"
#include "Block.h"

Level::Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	Scene(gfx, input)
{
	test_block_ = std::make_shared<Block>(Block(L"Images/grassblock.png", graphics_, input_));
	test_block_->SetPosition({ 0.0f, -60.0f, 5.0f });
	test_block_->BindOBBToCM(cm_);
	test_block_->SetScale({ 30.0f, 30.0f, 30.0f });
	test_block_->UpdateExtents({ 30.0f, 30.0f, 30.0f });
	test_block_->UpdateMass(500000.0f);
	AddEntity(std::dynamic_pointer_cast<Entity>(test_block_));
	test_block_2_ = std::make_shared<Block>(Block(L"Images/grassblock.png", graphics_, input_));
	test_block_2_->SetPosition({ 0.7f, 5.0f, 5.0f });
	test_block_2_->SetAngularVelocity({ 1.0f, 1.0f, 0.0f });
	test_block_2_->BindOBBToCM(cm_);
	AddEntity(std::dynamic_pointer_cast<Entity>(test_block_2_));
}

void Level::Update(const float& dt)
{
	// gravity update
	test_block_2_->ApplyLinearForce(Vecf3(0.0f, -981.0f, 0.0f) * dt);
	for (int i = 0; i < blocks_.size(); i++) {
		blocks_[i]->ApplyLinearForce(Vecf3(0.0f, -981.0f, 0.0f) * dt);
	}

	if (input_->KeyWasPressed('B')) {
		//AddEntity(std::dynamic_pointer_cast<Entity>(std::make_shared<Block>((Block(L"Images/grassblock.png", graphics_, input_)))));
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
	if (input_->KeyIsDown('P')) {
		Vecf3 v = test_block_->GetPosition();
		v.z += 10.0f * dt;
		test_block_->SetPosition(v);
	}
	// Physics tests
	cm_.IntegrateVelocities(dt);
	cm_.TestAllOBBS();
	int iterations = 20;
	for (int i = 0; i < iterations; i++) {
		cm_.PreSolveSolve(dt);
	}
	cm_.CleanUp();
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

	std::shared_ptr<Block> block = std::make_shared<Block>(Block(L"Images/grassblock.png", graphics_, input_));
	// set block attributes
	float rand_size = float(rand() % 4 + 1);
	block->SetPosition({ x_rand, 8.0f, z_rand });
	block->BindOBBToCM(cm_);
	block->SetScale({ rand_size, rand_size, rand_size });
	block->UpdateExtents({ rand_size, rand_size, rand_size });
	block->UpdateMass(5.0f * rand_size);
	blocks_.push_back(block);
	AddEntity(std::dynamic_pointer_cast<Entity>(block));
}
