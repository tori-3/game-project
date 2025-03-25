#pragma once
#include"Common.h"
#include"setting.h"
#include"EnemyAdder.h"
#include"BGMManager.hpp"
#include"IrisOut.h"
#include"TalkManager.h"
#include"GrapesHPBar.h"
#include"Spotlight.hpp"
#include"MiniGameSceneBase.h"
#include"SmoothCamera.h"
#include"Player.h"

class MainGameScene : public MiniGameSceneBase
{
public:

	DataManagerStart start;

	JSON json = JSON::Load(getData().stageFile);
	Stage stage{ json };
	StageBackGroundManager backgroundManager{ json };

	MSRenderTexture rTexture{ Scene::Size(),ColorF{0,0} };

	Texture background{ getData().backgroundTexture };

	EntityManager manager;

	EnemyAdder adder;

	ColorF skyColor{Palette::Skyblue};

	Player* player=nullptr;

	SmoothCamera camera;

	IrisOut irisOut;

	GrapesHPBar hpBar{ Rect{Arg::center(Scene::Center().x,50),300,40} };

	Spotlight* light = nullptr;

	bool bgmStart = false;

	MainGameScene(const InitData& init);

	void gameUpdate() override;

	void gameDraw() const override;

	void drawFadeIn(double t) const override;

	static void CookieDisplay(int32 count, double tame);

	static void HpDisplay(int32 count, int32 maxHP);

	void EndGame(bool clear)
	{
		getData().mini_clear = clear;
		changeScene(U"Map");
		getData().backFromMainGameScene = true;
	}
};
