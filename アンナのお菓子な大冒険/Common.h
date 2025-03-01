#pragma once
#include"LongPressInput.hpp"
#include"BunchoUI.hpp"

using namespace BunchoUI;

enum mode { Stage_Mode, Easy_Mode, Normal_Mode, Hard_Mode };

struct GameData
{
	double BGMVolume = 1.0;
	double EffectVolume = 1.0;
	int32 stage = 1;
	mode mini_mode = Easy_Mode;
	bool mini_clear = false;
	bool boss_clear = false;

	bool backFromMainGameScene = false;

	//ゲームをやめるでMapに戻ってきたか
	bool quitStage = false;

	int32 clearStage = 0;;

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

	void save()
	{
		JSON saveDatajson;
		saveDatajson[U"SelectedStage"] = stage;
		saveDatajson[U"ClearStage"] = clearStage;
		saveDatajson[U"BGMVolume"] = BGMVolume;
		saveDatajson[U"EffectVolume"] = EffectVolume;

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
		BGMVolume = saveDatajson[U"BGMVolume"].get<double>();
		EffectVolume = saveDatajson[U"EffectVolume"].get<double>();

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
