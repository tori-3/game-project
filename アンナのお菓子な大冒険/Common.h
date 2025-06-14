#pragma once
#include"LongPressInput.hpp"
#include"BunchoUI.hpp"
#include"Lisence.h"
#include"SettingWindow.h"
#include"KeyConfigUtility.h"

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

	void setIncreaseHPMode(bool onOff)
	{
		if (increaseHPMode && not onOff)
		{
			constexpr int32 defaultHP = 5;

			for (int32& maxHP : maxHPList)
			{
				maxHP = defaultHP;
			}
		}

		increaseHPMode = onOff;
	}

	bool getIncreaseHPMode()const noexcept
	{
		return increaseHPMode;
	}

	//キー
	InputGroup minigameUpKey = s3d::KeyW | s3d::KeyUp;
	InputGroup minigameLeftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup minigameDownKey = s3d::KeyS | s3d::KeyDown;
	InputGroup minigameRightKey = s3d::KeyD | s3d::KeyRight;

	InputGroup menuUpKey = s3d::KeyW | s3d::KeyUp;
	InputGroup menuLeftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup menuDownKey = s3d::KeyS | s3d::KeyDown;
	InputGroup menuRightKey = s3d::KeyD | s3d::KeyRight;
	InputGroup menuDecisionKey = s3d::KeyEnter;
	InputGroup menuBackKey = s3d::KeyQ;

	InputGroup jumpKey = s3d::KeyW | s3d::KeySpace | s3d::KeyUp;
	InputGroup attackKey = s3d::KeyEnter;
	InputGroup leftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup downKey = s3d::KeyS | s3d::KeyDown;
	InputGroup rightKey = s3d::KeyD | s3d::KeyRight;
	InputGroup pauseKey = KeyEscape;

	//ステージ
	String backgroundTexture;
	String BGMPath;
	FilePath stageFile;
	int32 maxHP = 5;

private:
	bool increaseHPMode = true;
};

using App = SceneManager<String, GameData>;
