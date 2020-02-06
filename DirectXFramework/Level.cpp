#include "Level.h"
#include "Block.h"

Level::Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	Scene(gfx, input)
{
	//test_block_ = std::make_shared<Block>(Block(L"Images/grassblock.png", graphics_, input_));
	//AddEntity(std::dynamic_pointer_cast<Entity>(test_block_));

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
	const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
	test_object_ = std::make_shared<TestObject>(graphics_, input_, mat, "Models\\10021_Giraffe_v04.obj", L"Images/10021_Giraffe_v05.png");
	/*test_object_ = std::make_shared<TestObject>(graphics_, input_, "Models\\dog.obj");
>>>>>>> d6855af42c04a8ccdc680a2c4e571b1b68c4a4bb
	test_object_->temp_x = -3.14f/2.0f;
	test_object_->temp_scale_ = 1.0f;
	test_object_->temp_pos_x_ = -2.0f;
<<<<<<< HEAD
	test_object_->temp_pos_y_ = -1.0f;
	//test_object_2_ = std::make_shared<TestObject>(graphics_, input_, mat, "Models\\lowpolytree.obj");
	//AddEntity(std::dynamic_pointer_cast<Entity>(test_object_));
=======
	test_object_->temp_pos_y_ = -1.0f;*/
	//test_object_2_ = std::make_shared<TestObject>(graphics_, input_, "Models\\lowpolytree.obj");
	//AddEntity(std::dynamic_pointer_cast<Entity>(test_object_2_));

	// Initialize map generator
	mapGen_ = std::make_unique<MapGenerator>(graphics_, input_);

	// Generate map
	for (std::shared_ptr<Entity> e : mapGen_->GenerateMap())
	{ AddEntity(e); }
}

void Level::Update(const float& dt)
{
	if (input_->KeyWasPressed('B')) {
		AddEntity(std::dynamic_pointer_cast<Entity>(std::make_shared<Block>((Block(L"Images/grassblock.png", graphics_, input_)))));
	}
	if (input_->KeyIsDown('P')) {
		Vecf3 v = test_block_->GetPosition();
		v.z += 10.0f * dt;
		test_block_->SetPosition(v);
	}

	// Generate new chunk
	if (input_->KeyWasPressed('G')) 
	{
		for (std::shared_ptr<Entity> e : mapGen_->GenerateMap())
		{ AddEntity(e); }
	}

	test_object_->Draw();
	//test_object_2_->Draw();
}

void Level::Render(const float& dt)
{
	Scene::Render(dt);
}
