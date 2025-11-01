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
#include"Shadow.h"

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

	GrapesHPBar hpBar{ Rect{Arg::center(Scene::Center().x,50+20),300,40} };

	Spotlight* light = nullptr;
	
	RenderTexture backgroundRenderTexture{ Scene::Size() };
	//const RenderTexture internalTexture{ Scene::Size() };
	//const RenderTexture downsample{ Scene::Size() / 2 };

	bool bgmStart = false;

	double kaisouAlpha = 0;

	bool saveFlg = false;

	double damageTimer = 0;

	Entity* fairy = nullptr;

	bool setBossTalkFlg = false;

	//お試し
	Shadow shadow;

	MainGameScene(const InitData& init);

	~MainGameScene();

	void gameUpdate() override;

	void gameDraw() const override;

	void drawFadeIn(double t) const override;

	static void CookieDisplay(int32 count, double tame);

	static void HpDisplay(int32 count, int32 maxHP);

	bool isShowKaiso()const
	{
		return getData().stage == 1 && InRange(TalkManager::get().talkWindow.index, 3ull , 6ull) && TalkManager::get().talkWindow.talks.size() == 9;
	}

	void EndGame(bool clear)
	{
		getData().mini_clear = clear;

		changeScene(U"Map");

		//getData().backFromMainGameScene = true;
	}
};
