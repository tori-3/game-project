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

	void drawStick(const Vec2& pos, StringView text, bool notify)const;

	Array<String>pathList{ U"BackGroundTexture/雪原背景.png" };

	size_t textureIndex = 0;

	Timer timer{ 10s };

	Array<String>menuList
	{
		U"はじめる",
		U"ミニゲーム",
		U"設定",
		U"詳細情報",
		U"終了",
	};

	Array<std::function<void()>>funcList
	{
		[&]{
			playerWalkStop = true;
			character.clearMotion();
			character.addMotion(U"Tosshin", true);
			changeMapTimer.restart();
		},
		[&] {
			playerWalkStop = true;
			character.clearMotion();
			character.addMotion(U"HeadDrop");
			changeMiniGameTimer.restart();
		},
		[&] {
			uiManager.addChild({ SettingWindow(getData().minigameUpKey,getData().minigameDownKey,getData().minigameLeftKey,getData().minigameRightKey,[=] {menuClicked = false; getData().save(); KeyConfigUtility::CleapInput(getData().menuDecisionKey); upInput.input = getData().minigameUpKey; downInput.input = getData().minigameDownKey; },getData(),uiManager)});
		},
		[&] {
			uiManager.addChild({ licenseDialog() });
		},
		[&]{
			System::Exit();
		}
	};

	size_t selectedIndex = 0;

	LongPressInput upInput{ getData().minigameUpKey,ControllerManager::Direction::Up };
	LongPressInput downInput{ getData().minigameDownKey,ControllerManager::Direction::Down };

	static constexpr Vec2 startPlace{ -100,630+15-10 };
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

	Texture notifyIcon{ 0xF06BD_icon,40 };

	UIManager uiManager;

	Timer omakeTimer{ 15s };

	RenderTexture downsample;

	Timer anyKeyTimer{ 1.0s };

	double rogoTimer = 0;

	static constexpr Vec2 anyKeyPos{ 600,610 };

	double translate()const
	{
		return (1 - Min(time / 2.0, 1.0)) * 500;
	}

	TitleScene(const InitData& init);

	void update() override;

	void draw() const override;

	std::shared_ptr<UIElement> licenseDialog();

};
