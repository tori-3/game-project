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
	static constexpr int32 CandyCloud = 17;
	static constexpr int32 LastBossStage = 25;// 26

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

	//ゲームを始める前に呼ぶ
	void initGame()
	{
		//firstClearMinigame = false;
		//firstClearStage = false;
		mini_clear = false;//一応
		quitStage = false;//一応
	}

	GameData()
		:maxHPList(LastBossStage, 5)
		,clearHPList(LastBossStage, 0)
		, miniGameList(6,MiniGameState{})
	{
		GlobalAudio::BusSetVolume(MixBus0, 0.5);
		GlobalAudio::BusSetVolume(MixBus1, 0.5);

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
	InputGroup minigameUpKey = KeyW | KeyUp | XInput(0).buttonUp;
	InputGroup minigameLeftKey = KeyA | KeyLeft | XInput(0).buttonLeft;
	InputGroup minigameDownKey = KeyS | KeyDown | XInput(0).buttonDown;
	InputGroup minigameRightKey = KeyD | KeyRight | XInput(0).buttonRight;
	InputGroup menuDecisionKey = KeyEnter | XInput(0).buttonA;
	InputGroup menuBackKey = KeyQ | XInput(0).buttonB;

	InputGroup attackKey = KeyEnter | XInput(0).buttonA;
	InputGroup jumpKey = KeySpace | XInput(0).buttonB;
	InputGroup downKey = KeyS | KeyDown | XInput(0).buttonDown;
	InputGroup pauseKey = KeyEscape | XInput(0).buttonStart;

	//ステージ
	String backgroundTexture;
	String BGMPath;
	FilePath stageFile;
	//int32 maxHP = 5;

private:
	bool increaseHPMode = true;
};

using App = SceneManager<String, GameData>;
