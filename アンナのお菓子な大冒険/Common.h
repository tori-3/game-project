#pragma once
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

	void saveMiniGameClear()
	{
		switch (mini_mode)
		{
		case Stage_Mode:
			if(clearStage < stage)
			{
				clearStage = stage;
				save();
			}
			break;
		case Easy_Mode:
			if(not miniGameList[miniGameIndex].easyClear)
			{
				miniGameList[miniGameIndex].easyClear = true;
				save();
			}
			break;
		case Normal_Mode:
			if (not miniGameList[miniGameIndex].normalClear)
			{
				miniGameList[miniGameIndex].normalClear = true;
				save();
			}
			break;
		case Hard_Mode:
			if (not miniGameList[miniGameIndex].hardClear)
			{
				miniGameList[miniGameIndex].hardClear = true;
				save();
			}
			break;
		default:
			break;
		}
	}

	void save()
	{
		JSON saveDatajson;
		saveDatajson[U"SelectedStage"] = stage;
		saveDatajson[U"ClearStage"] = clearStage;
		saveDatajson[U"BGMVolume"] = GlobalAudio::BusGetVolume(BGMMixBus);
		saveDatajson[U"EffectVolume"] = GlobalAudio::BusGetVolume(EffectMixBus);

		for(int32 maxHP: maxHPList)
		{
			saveDatajson[U"MaxHP"].push_back(maxHP);
		}

		for (bool clearComplete : clearCompleteList)
		{
			saveDatajson[U"ClearComplete"].push_back(clearComplete);
		}

		for (const auto& miniGame : miniGameList)
		{
			JSON json;
			json[U"Easy"]= miniGame.easyClear;
			json[U"Normal"] = miniGame.normalClear;
			json[U"Hard"] = miniGame.hardClear;
			saveDatajson[U"MiniGame"].push_back(json);
		}

		saveDatajson.save(U"saveData.json");
	}

	void load()
	{
		JSON saveDatajson = JSON::Load(U"saveData.json");

		stage = saveDatajson[U"SelectedStage"].get<int32>();
		clearStage = saveDatajson[U"ClearStage"].get<int32>();
		GlobalAudio::BusSetVolume(BGMMixBus, saveDatajson[U"BGMVolume"].get<double>());
		GlobalAudio::BusSetVolume(EffectMixBus, saveDatajson[U"EffectVolume"].get<double>());

		maxHPList.clear();
		for(const auto& obj:saveDatajson[U"MaxHP"].arrayView())
		{
			maxHPList << obj.get<int32>();
		}

		clearCompleteList.clear();
		for (const auto& obj : saveDatajson[U"ClearComplete"].arrayView())
		{
			clearCompleteList << obj.get<bool>();
		}

		miniGameList.clear();
		for (const auto& obj : saveDatajson[U"MiniGame"].arrayView())
		{
			MiniGameState miniGame
			{
				.easyClear = obj[U"Easy"].get<bool>(),
				.normalClear = obj[U"Normal"].get<bool>(),
				.hardClear = obj[U"Hard"].get<bool>(),
			};

			miniGameList << miniGame;
		}
	}




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
