#pragma once
#include"LongPressInput.hpp"
#include"BunchoUI.hpp"
#include"Lisence.h"
#include"SettingWindow.h"
#include"KeyConfigUtility.h"
#include"NotificationAddon.h"

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

	//ステージタグ
	String tag;

	//ステージの最初のテキスト
	String text;

	static constexpr int32 ChocoMountain = 8;
	static constexpr int32 CandyCloud = 17;
	static constexpr int32 LastBossStage = 30;// 26

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


	bool pressAnyKeyFlg = true;


	bool gamepadMode = false;

	bool screenshotNotify = false;
	bool garellyNotificationAddonFlg = false;


	//カーソルを隠すか
	//Titleにいくかカーソルを動かしたらfalseにする
	bool hideCursor = true;


	//ボタンで終了したか
	bool exitButton = false;


	//タイトルシーンから移動したか(Map画面への遷移に使用)
	bool fadeWipe = false;

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
		initKeyConfig();

		GlobalAudio::BusSetVolume(MixBus0, 0.5);
		GlobalAudio::BusSetVolume(MixBus1, 0.5);

		load();
	}

	void update();

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

	void initKeyConfig()
	{
		minigameUpKey = KeyW | KeyUp | XInput(0).buttonUp;
		minigameLeftKey = KeyA | KeyLeft | XInput(0).buttonLeft;
		minigameDownKey = KeyS | KeyDown | XInput(0).buttonDown;
		minigameRightKey = KeyD | KeyRight | XInput(0).buttonRight;
		menuDecisionKey = KeyEnter | XInput(0).buttonA;
		menuBackKey = KeyQ | XInput(0).buttonB;
		attackKey = KeyEnter | XInput(0).buttonX;
		jumpKey = KeySpace | KeyW | XInput(0).buttonA;
		downKey = KeyS | KeyDown | XInput(0).buttonDown;
		pauseKey = KeyEscape | XInput(0).buttonStart;
		minigameLB = KeyA | KeyLeft | XInput(0).buttonLB;
		minigameRB = KeyD | KeyRight | XInput(0).buttonRB;
		screenshotKey = KeyF12 | XInput(0).buttonBack;
	}

	//キー
	InputGroup minigameUpKey;
	InputGroup minigameLeftKey;
	InputGroup minigameDownKey;
	InputGroup minigameRightKey;
	InputGroup menuDecisionKey;
	InputGroup menuBackKey;
	InputGroup attackKey;
	InputGroup jumpKey;
	InputGroup downKey;
	InputGroup pauseKey;
	InputGroup minigameLB;
	InputGroup minigameRB;
	InputGroup screenshotKey;


	//ステージ
	String backgroundTexture;
	String BGMPath;
	FilePath stageFile;
	//int32 maxHP = 5;

private:
	bool increaseHPMode = true;
};

using App = SceneManager<String, GameData>;
