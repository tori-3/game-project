#pragma once
#include"Common.h"
#include"Fire.hpp"
#include"BeltConveyorButton.h"

class MiniGameSelect : public App::Scene
{
public:

	struct MiniGameData
	{
		Texture pictures;
		String sentence;
		int32 stageNum;
		String sceneName;
	};

	static constexpr Circle backButton{ {35,35},30 };

	Texture homeIcon{ 0xf015_icon,40 };

	Texture background{ U"BackGroundTexture/洞窟背景.png" };

	Texture signboardHead{U"SignboardHead.png"};
	Texture signboardStick{ U"SignboardStick.png" };

	Texture candle{ U"StageTexture/Candle.png" };

	Array<MiniGameData>miniGameList;

	Spotlight light{ Scene::Size() };

	Fire leftFire{ {75,620},HSV{ 20,0.8,1 },40,{ 0, -30 } };
	Fire rightFire{ {Scene::Width() - 75,620},HSV{ 20,0.8,1 },40,{ 0, -30 } };

	//const Font font{ 30 };

	LongPressInput leftInput{ getData().minigameLeftKey };
	LongPressInput rightInput{ getData().minigameRightKey };
	LongPressInput upInput{ getData().minigameUpKey };
	LongPressInput downInput{ getData().minigameDownKey };

	BeltConveyorButton easyButton{ {800,500},3,80 };
	BeltConveyorButton normalButton{ {800,600},3,80 };
	BeltConveyorButton hardButton{ {800,700},3,80 };

	Texture chocolateBeltConveyor{ U"StageTexture/BeltConveyor.png" };

	Effect effect;

	MiniGameSelect(const InitData& init);

	void update()override;

	void draw()const override;
};
