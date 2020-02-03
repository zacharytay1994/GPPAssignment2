#include "MainMenu.h"
#include "Block.h"
#include "Player.h"
#include "Rail.h"

MainMenu::MainMenu(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input):
	Scene(gfx, input)
{
	std::shared_ptr<Block> tempBlock;
	std::shared_ptr<Rail> tempRail;
	//for (float z = 3.0f; z < 15.0f; z++) {


	//	for (float x = -1.0f; x < 4.0f; x++)
	//	{
	//		//floor
	//		tempBlock = std::make_shared<Block>(Block(L"Images/stoneblock.png", graphics_, input_));
	//		tempBlock->SetPosition({ x,0,z });
	//		AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

			// ceiling
			//tempBlock = std::make_shared<Block>(Block(L"Images/stoneblock.png", graphics_, input_));
			//tempBlock->SetPosition({ x,6,z });
			//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
		//}


		//walls
		//for (float y = 1.0f; y < 6.0f; y++) {
		//	tempBlock = std::make_shared<Block>(Block(L"Images/stoneblock.png", graphics_, input_));
		//	tempBlock->SetPosition({ -2,y,z });
		//	AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		//	tempBlock = std::make_shared<Block>(Block(L"Images/stoneblock.png", graphics_, input_));
		//	tempBlock->SetPosition({ 4,y,z });
		//	AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
		//}

		//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
		//tempBlock->SetPosition({ -1,1,z });
		//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
		//tempBlock->SetPosition({ 3,1,z });
		//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
		//tempBlock->SetPosition({ -1,5,z });
		//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
		//tempBlock->SetPosition({ 3,5,z });
		//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		// rails
	//	tempRail = std::make_shared<Rail>(Rail(graphics_, input_));
	//	tempRail->SetPosition({ 1,0.5,z });
	//	AddEntity(std::dynamic_pointer_cast<Entity>(tempRail));
	//}

	
	std::shared_ptr<Player> player = std::make_shared<Player>(Player(graphics_, input_));
	player->SetPosition({ 0,0.75,0 });

	AddEntity(std::dynamic_pointer_cast<Entity>(player));


	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ 1,0,0 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ -1,0,0 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ 0,0,1 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ 0,0,-1 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

	//tempBlock = std::make_shared<Block>(Block(L"Images/alex.png", graphics_, input_));
	//tempBlock->SetPosition({ 0,0,0 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

	tempBlock = std::make_shared<Block>(Block(L"Images/menu.png", graphics_, input_));
	tempBlock->SetScale({ (18+6/16)*0.5, 0.5, (9+6/16)*0.5 });
	tempBlock->SetPosition({ (18+6/16)*0.5, 0, (9+6/16)*0.5 });
	AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));


	wb(0.387096774193548, 0.139784946236559, 0, 0);
	wb(0.155913978494624, 0.39247311827957, 0, 0.268817204301075);
	wb(0.155913978494624, 0.39247311827957, 0.462365591397849, 0.279569892473118);

	wb(0.123655913978495, 0.145161290322581, 0.903225806451613, 0);
	wb(0.333333333333333, 0.139784946236559, 0.903225806451613, 0.279569892473118);
	wb(0.198924731182796, 0.0591397849462366, 0.903225806451613, 0.548387096774194);
	wb(0.161290322580645, 0.043010752688172, 0.903225806451613, 0.655913978494624);
	wb(0.123655913978495, 0.0537634408602151, 0.903225806451613, 0.731182795698925);
	wb(0.123655913978495, 0.0591397849462366, 1.32258064516129, 0);
	wb(0.161290322580645, 0.043010752688172, 1.24731182795699, 0.10752688172043);
	wb(0.198924731182796, 0.0537634408602151, 1.17204301075269, 0.182795698924731);
	wb(0.123655913978495, 0.145161290322581, 1.32258064516129, 0.548387096774194);

	wb(0.123655913978495, 0.317204301075269, 1.68817204301075, 0);
	wb(0.166666666666667, 0.112903225806452, 1.68817204301075, 0.623655913978495);
	wb(0.129032258064516, 0.161290322580645, 2.01075268817204, 0.526881720430108);
	wb(0.129032258064516, 0.0860215053763441, 2.01075268817204, 0.365591397849462);
	wb(0.0483870967741935, 0.112903225806452, 1.9247311827957, 0.311827956989247);
	wb(0.0967741935483871, 0.188172043010753, 2.01075268817204, 0);
	wb(0.0376344086021505, 0.166666666666667, 2.19354838709677, 0);

	gb(0.0860215053763441, 0.32258064516129, 2.38709677419355, 0);
	gb(0.043010752688172, 0.198924731182796, 2.54838709677419, 0);
	gb(0.0591397849462366, 0.102150537634409, 2.62365591397849, 0.236559139784946);
	gb(0.043010752688172, 0.198924731182796, 2.73118279569892, 0);
	gb(0.0806451612903226, 0.32258064516129, 2.80645161290323, 0);
	gb(0.0268817204301075, 0.0645161290322581, 2.54838709677419, 0.387096774193548);
	gb(0.0268817204301075, 0.0645161290322581, 2.76344086021505, 0.387096774193548);
	gb(0.0483870967741935, 0.032258064516129, 2.54838709677419, 0.505376344086022);
	gb(0.0483870967741935, 0.032258064516129, 2.72043010752688, 0.505376344086022);
	gb(0.0591397849462366, 0.0483870967741935, 2.62365591397849, 0.548387096774194);
	gb(0.290322580645161, 0.10752688172043, 2.38709677419355, 0.634408602150538);

	wb(0.123655913978495, 0.424731182795699, 3.08602150537634, 0);

	wb(0.28494623655914, 0.112903225806452, 3.47311827956989, 0);
	wb(0.123655913978495, 0.317204301075269, 3.47311827956989, 0.21505376344086);

	wb(0.123655913978495, 0.419354838709677, 4.17204301075269, 0);
	wb(0.166666666666667, 0.112903225806452, 4.40860215053763, 0);
	wb(0.166666666666667, 0.10752688172043, 4.40860215053763, 0.311827956989247);
	wb(0.166666666666667, 0.112903225806452, 4.40860215053763, 0.612903225806452);

	wb(0.123655913978495, 0.311827956989247, 4.86021505376344, 0);
	wb(0.145161290322581, 0.112903225806452, 5.09677419354839, 0);
	wb(0.043010752688172, 0.349462365591398, 5.37634408602151, 0.0752688172043011);
	wb(0.0913978494623656, 0.317204301075269, 5.20430107526882, 0.21505376344086);
	wb(0.17741935483871, 0.118279569892473, 4.86021505376344, 0.612903225806452);

	wb(0.123655913978495, 0.123655913978495, 5.59139784946237, 0);
	wb(0.123655913978495, 0.381720430107527, 5.59139784946237, 0.290322580645161);

	wb(0.155913978494624, 0.166666666666667, 0.935483870967742, 0.946236559139785);
	wb(0.155913978494624, 0.0967741935483871, 1.43010752688172, 0.946236559139785);
	wb(0.21505376344086, 0.0752688172043011, 1.31182795698925, 1.12903225806452);
	wb(0.403225806451613, 0.204301075268817, 0.935483870967742, 1.26881720430108);
	wb(0.21505376344086, 0.102150537634409, 0.935483870967742, 1.66666666666667);
	wb(0.155913978494624, 0.166666666666667, 1.43010752688172, 1.66666666666667);
	wb(0.155913978494624, 0.0698924731182796, 0.935483870967742, 1.86021505376344);

	wb(0.0376344086021505, 0.349462365591398, 1.87096774193548, 1.02150537634409);
	wb(0.0913978494623656, 0.424731182795699, 1.93548387096774, 0.946236559139785);
	wb(0.0913978494623656, 0.112903225806452, 2.10752688172043, 0.946236559139785);
	wb(0.0913978494623656, 0.118279569892473, 2.10752688172043, 1.55913978494624);
	wb(0.0967741935483871, 0.424731182795699, 2.27956989247312, 0.946236559139785);
	wb(0.0376344086021505, 0.349462365591398, 2.46236559139785, 1.02150537634409);

	wb(0.290322580645161, 0.112903225806452, 2.65591397849462, 1.55913978494624);
	wb(0.129032258064516, 0.311827956989247, 2.81720430107527, 0.946236559139785);

	wb(0.220430107526882, 0.139784946236559, 3.36559139784946, 0.946236559139785);
	wb(0.155913978494624, 0.198924731182796, 3.79569892473118, 0.946236559139785);
	wb(0.370967741935484, 0.139784946236559, 3.36559139784946, 1.33333333333333);
	wb(0.150537634408602, 0.198924731182796, 3.36559139784946, 1.60215053763441);
	wb(0.225806451612903, 0.139784946236559, 3.65591397849462, 1.72043010752688);

	wb(0.0376344086021505, 0.344086021505376, 4.23655913978495, 1.02150537634409);
	wb(0.0913978494623656, 0.424731182795699, 4.3010752688172, 0.946236559139785);
	wb(0.0913978494623656, 0.112903225806452, 4.47311827956989, 0.946236559139785);
	wb(0.0913978494623656, 0.118279569892473, 4.47311827956989, 1.55913978494624);
	wb(0.0967741935483871, 0.424731182795699, 4.64516129032258, 0.946236559139785);
	wb(0.0376344086021505, 0.349462365591398, 4.82795698924731, 1.02150537634409);





}

void MainMenu::Update(const float& dt)
{
	if (input_->KeyWasPressed('B')) {
		AddEntity(std::dynamic_pointer_cast<Entity>(std::make_shared<Block>((Block(L"Images/grassblock.png", graphics_, input_)))));
	}
}

void MainMenu::Render(const float& dt)
{
	Scene::Render(dt);
}
