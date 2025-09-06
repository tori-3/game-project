#pragma once

#include"Common.h"
#include"LoadAsset.h"
#include"SweetsPanel.hpp"
#include"CharacterSystem.h"

class TitleScene : public App::Scene
{
public:

	Texture stickTexture{ U"SignboardStick.png" };

	bool updateStick(const Vec2& pos);

	void drawStick(const Vec2& pos, StringView text)const;

	const Array<String>pathList{ U"BackGroundTexture/雪原背景.png",U"BackGroundTexture/洞窟背景.png",U"BackGroundTexture/雲背景.png" ,U"BackGroundTexture/ラスボス背景.png",U"BackGroundTexture/宇宙背景.png"};

	size_t textureIndex = 0;

	Timer timer{ 10s,StartImmediately::Yes };

	Array<String>menuList
	{
		U"マップ",
		U"ミニゲーム",
		U"思い出",
		U"設定",
		U"ライセンス",
		U"終了",
	};

	size_t selectedIndex = 0;

	LongPressInput upInput{ getData().minigameUpKey };
	LongPressInput downInput{ getData().minigameDownKey };

	static constexpr Vec2 startPlace{ -100,630+15 };
	static constexpr Vec2 targetPlace{ 400,startPlace.y };
	Vec2 playerPos = startPlace;

	CharacterSystem character{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.5,startPlace,false};

	Timer changeMapTimer{ 1s };
	Timer changeMiniGameTimer{ 1s };

	static constexpr Vec2 menuPos{ 800,450 + 30-35 };
	static constexpr double menuHeight = 60;

	double time = 0;

	bool playerWalkStop = false;

	bool menuClicked = false;


	UIManager uiManager;

	double translate()const
	{
		return (1 - Min(time / 2.0, 1.0)) * 500;
	}

	TitleScene(const InitData& init);

	void update() override;

	void draw() const override;

	std::shared_ptr<UIElement> licenseDialog();

};
