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

	String description;

	//ゲームをやめるでMapに戻ってきたか
	bool quitStage = false;

	int32 clearStage = 0;;

	int32 miniGameIndex = 0;

	//ミニゲーム選択画面用
	//前回選択したミニゲームのモード
	int32 miniGameModeIndex = 0;

	//今いるシーンの名前
	String sceneName;

	static constexpr int32 ChocoMountain = 8;
	static constexpr int32 CandyCloud = 15;
	static constexpr int32 LastBossStage = 23;// 26

	struct MiniGameState
	{
		bool easyClear = false;
		bool normalClear = false;
		bool hardClear = false;
	};

	Array<int32>maxHPList;

	Array<int32>clearHPList;



	Array<MiniGameState>miniGameList;

	bool notifyMiniGameSelect = false;
	bool notifyGallery = false;

	//そのステージ(ミニゲームを始めてクリアしたか) 演出を完了するときにfalseにする
	bool firstClearMinigame = false;
	bool firstClearStage = false;

	double vibration = 1.0;

	//ゲームを始める前に呼ぶ
	void initGame()
	{
		//firstClearMinigame = false;
		//firstClearStage = false;
		mini_clear = false;//一応
		quitStage = false;//一応
	}

	GameData()
	{
		load();
	}

	void saveMiniGameClear();

	void saveMainGame(bool clear);

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

	String fmt(StringView text)const;

	//キー
	InputGroup minigameUpKey = s3d::KeyW | s3d::KeyUp;
	InputGroup minigameLeftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup minigameDownKey = s3d::KeyS | s3d::KeyDown;
	InputGroup minigameRightKey = s3d::KeyD | s3d::KeyRight;
	InputGroup menuDecisionKey = s3d::KeyEnter;
	InputGroup menuBackKey = s3d::KeyQ;

	InputGroup attackKey = s3d::KeyEnter;
	InputGroup jumpKey = s3d::KeyW | s3d::KeySpace | s3d::KeyUp;
	//InputGroup leftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup downKey = s3d::KeyS | s3d::KeyDown;
	//InputGroup rightKey = s3d::KeyD | s3d::KeyRight;
	InputGroup pauseKey = KeyEscape;

	//ステージ
	String backgroundTexture;
	String BGMPath;
	FilePath stageFile;
	//int32 maxHP = 5;

private:
	bool increaseHPMode = true;
};

using App = SceneManager<String, GameData>;
