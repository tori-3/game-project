﻿#pragma once
#include"LongPressInput.hpp"
#include"BunchoUI.hpp"
#include"Lisence.h"
#include"SettingWindow.h"

using namespace BunchoUI;

enum mode { Stage_Mode, Easy_Mode, Normal_Mode, Hard_Mode };

constexpr MixBus EffectMixBus = MixBus0;
constexpr MixBus BGMMixBus = MixBus1;

struct GameData
{
	int32 stage = 1;
	mode mini_mode = Easy_Mode;
	bool mini_clear = false;
	bool boss_clear = false;

	String description;

	bool backFromMainGameScene = false;

	//ゲームをやめるでMapに戻ってきたか
	bool quitStage = false;

	int32 clearStage = 0;;

	int32 miniGameIndex = 0;

	//ミニゲーム選択画面用
	//前回選択したミニゲームのモード
	int32 miniGameModeIndex = 0;

	//今いるシーンの名前
	String sceneName;

	struct MiniGameState
	{
		bool easyClear = false;
		bool normalClear = false;
		bool hardClear = false;
	};

	Array<int32>maxHPList;
	Array<bool>clearCompleteList;
	Array<MiniGameState>miniGameList;

	GameData()
	{
		load();
	}

	void saveMiniGameClear();

	void save();

	void load();

	//キー
	InputGroup KeyUp = s3d::KeyW | s3d::KeyUp;
	InputGroup KeyLeft = s3d::KeyA | s3d::KeyLeft;
	InputGroup KeyDown = s3d::KeyS | s3d::KeyDown;
	InputGroup KeyRight = s3d::KeyD | s3d::KeyRight;

	InputGroup jumpKey = s3d::KeyW | s3d::KeySpace | s3d::KeyUp;
	Input attackKey = s3d::KeyEnter;
	InputGroup leftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup downKey = s3d::KeyS | s3d::KeyDown;
	InputGroup rightKey = s3d::KeyD | s3d::KeyRight;

	//ステージ
	String backgroundTexture;
	String BGMPath;
	FilePath stageFile;
	int32 maxHP = 5;
};

using App = SceneManager<String, GameData>;
