#include"Common.h"
#include"KeyConfigUtility.h"
#include"KeyInfo.h"
#include"ControllerManager.h"
#include"ControllerInput.h"

void GameData::saveMiniGameClear()
{
	switch (mini_mode)
	{
	case Stage_Mode:
		if (clearStage < stage)
		{
			firstClearStage = true;
			notifyMiniGameSelect = true;
			NotificationAddon::Show(U"新しいミニゲームが追加！", U"[ミニゲーム]から様々な難易度で遊べます", NotificationAddon::Type::Minigame);
			clearStage = stage;
			save();
		}
		break;
	case Easy_Mode:
		if (not miniGameList[miniGameIndex].easyClear)
		{
			firstClearMinigame = true;
			miniGameList[miniGameIndex].easyClear = true;
			save();
		}
		break;
	case Normal_Mode:
		if (not miniGameList[miniGameIndex].normalClear)
		{
			firstClearMinigame = true;
			miniGameList[miniGameIndex].normalClear = true;
			save();
		}
		break;
	case Hard_Mode:
		if (not miniGameList[miniGameIndex].hardClear)
		{
			firstClearMinigame = true;
			miniGameList[miniGameIndex].hardClear = true;
			save();
		}
		break;
	default:
		break;
	}
}

void GameData::saveMainGame(bool clear)
{
	if (clear)
	{
		if (clearHPList[stage - 1]==0)
		{
			clearHPList[stage - 1] = maxHPList[stage - 1];
		}
		else
		{
			clearHPList[stage - 1] = Min(maxHPList[stage - 1], clearHPList[stage - 1]);
		}

		if (clearStage < stage)
		{
			clearStage = stage;

			firstClearStage = true;

			if (stage == LastBossStage)
			{
				notifyGallery = true;
				garellyNotificationAddonFlg = true;
			}
		}

		maxHPList[stage - 1] = 5;
	}
	else
	{
		if(increaseHPMode)
		{
			maxHPList[stage - 1] += 2;
			NotificationAddon::Show(U"HPが増加しました", U"失敗するとHPが2つ増加します\n[設定]からこの機能をOFFにできます", NotificationAddon::Type::Heart);
		}
	}

	save();
}

void GameData::update()
{
	if (not XInput(0).isConnected())
	{
		gamepadMode = false;
	}
	else if (Scene::Rect().leftClicked() || Scene::Rect().rightClicked()||not Cursor::DeltaRaw().isZero())
	{
		gamepadMode = false;
	}
	else if (AnyXInputPressed())
	{
		gamepadMode = true;
	}
	else
	{
		for (const auto& key : Keyboard::GetAllInputs())
		{
			if (key.down())
			{
				gamepadMode = false;
				break;
			}
		}
	}

	if(gamepadMode)
	{
		Cursor::RequestStyle(CursorStyle::Hidden);
	}

	//スクリーンショット
	//screenshotNotifyを使って通知は1フレームずらす
	if(screenshotNotify)
	{
		NotificationAddon::Show(U"スクリーンショットを保存", U"Screenshotフォルダに保存されています", NotificationAddon::Type::Screenshot);
		screenshotNotify = false;
	}

	if(screenshotKey.down())
	{
		screenshotNotify = true;
		ScreenCapture::SaveCurrentFrame();
	}
}

void GameData::save()
{
	JSON saveDatajson;
	saveDatajson[U"SelectedStage"] = stage;
	saveDatajson[U"ClearStage"] = clearStage;
	saveDatajson[U"BGMVolume"] = GlobalAudio::BusGetVolume(BGMMixBus);
	saveDatajson[U"EffectVolume"] = GlobalAudio::BusGetVolume(EffectMixBus);
	saveDatajson[U"IncreaseHPMode"] = increaseHPMode;
	saveDatajson[U"NotifyMiniGameSelect"] = notifyMiniGameSelect;
	saveDatajson[U"NotifyGallery"] = notifyGallery;
	saveDatajson[U"IsFullscreen"] = Window::GetState().fullscreen;
	saveDatajson[U"Vibration"] = ControllerManager::get().vibration;

	for (int32 maxHP : maxHPList)
	{
		saveDatajson[U"MaxHP"].push_back(maxHP);
	}

	for (int32 clearComplete : clearHPList)
	{
		saveDatajson[U"ClearHP"].push_back(clearComplete);
	}

	for (const auto& miniGame : miniGameList)
	{
		JSON json;
		json[U"Easy"] = miniGame.easyClear;
		json[U"Normal"] = miniGame.normalClear;
		json[U"Hard"] = miniGame.hardClear;
		saveDatajson[U"MiniGame"].push_back(json);
	}

	{
		JSON json;
		json[U"minigameUpKey"] = KeyConfigUtility::ToJSON(minigameUpKey);
		json[U"minigameLeftKey"] = KeyConfigUtility::ToJSON(minigameLeftKey);
		json[U"minigameDownKey"] = KeyConfigUtility::ToJSON(minigameDownKey);
		json[U"minigameRightKey"] = KeyConfigUtility::ToJSON(minigameRightKey);
		json[U"menuDecisionKey"] = KeyConfigUtility::ToJSON(menuDecisionKey);
		json[U"menuBackKey"] = KeyConfigUtility::ToJSON(menuBackKey);
		json[U"jumpKey"] = KeyConfigUtility::ToJSON(jumpKey);
		json[U"attackKey"] = KeyConfigUtility::ToJSON(attackKey);
		json[U"downKey"] = KeyConfigUtility::ToJSON(downKey);
		json[U"pauseKey"] = KeyConfigUtility::ToJSON(pauseKey);
		json[U"screenshotKey"] = KeyConfigUtility::ToJSON(screenshotKey);
		saveDatajson[U"Key"] = json;
	}

	saveDatajson.save(U"saveData.json");
}

void GameData::load()
{
	JSON saveDatajson = JSON::Load(U"saveData.json");

	if(not saveDatajson)
	{
		return;
	}

	stage = saveDatajson[U"SelectedStage"].get<int32>();
	clearStage = saveDatajson[U"ClearStage"].get<int32>();
	GlobalAudio::BusSetVolume(BGMMixBus, saveDatajson[U"BGMVolume"].get<double>());
	GlobalAudio::BusSetVolume(EffectMixBus, saveDatajson[U"EffectVolume"].get<double>());
	increaseHPMode = saveDatajson[U"IncreaseHPMode"].get<bool>();

	ControllerManager::get().vibration = saveDatajson[U"Vibration"].get<double>();

	notifyMiniGameSelect = saveDatajson[U"NotifyMiniGameSelect"].get<bool>();
	notifyGallery = saveDatajson[U"NotifyGallery"].get<bool>();

	Window::SetFullscreen(saveDatajson[U"IsFullscreen"].get<bool>());

	maxHPList.clear();
	for (const auto& obj : saveDatajson[U"MaxHP"].arrayView())
	{
		maxHPList << obj.get<int32>();
	}

	clearHPList.clear();
	for (const auto& obj : saveDatajson[U"ClearHP"].arrayView())
	{
		clearHPList << obj.get<int32>();
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

	{
		JSON json = saveDatajson[U"Key"];
		minigameUpKey = KeyConfigUtility::FromJSON(json[U"minigameUpKey"]);
		minigameLeftKey = KeyConfigUtility::FromJSON(json[U"minigameLeftKey"]);
		minigameDownKey = KeyConfigUtility::FromJSON(json[U"minigameDownKey"]);
		minigameRightKey = KeyConfigUtility::FromJSON(json[U"minigameRightKey"]);
		menuDecisionKey = KeyConfigUtility::FromJSON(json[U"menuDecisionKey"]);
		menuBackKey = KeyConfigUtility::FromJSON(json[U"menuBackKey"]);
		jumpKey = KeyConfigUtility::FromJSON(json[U"jumpKey"]);
		attackKey = KeyConfigUtility::FromJSON(json[U"attackKey"]);
		downKey = KeyConfigUtility::FromJSON(json[U"downKey"]);
		pauseKey = KeyConfigUtility::FromJSON(json[U"pauseKey"]);
		screenshotKey = KeyConfigUtility::FromJSON(json[U"screenshotKey"]);
	}

}

String GameData::fmt(StringView text)const
{
	return Fmt(text)(ToKeyName(minigameUpKey,gamepadMode), ToKeyName(minigameLeftKey, gamepadMode), ToKeyName(minigameDownKey, gamepadMode), ToKeyName(minigameRightKey, gamepadMode), ToKeyName(attackKey, gamepadMode), ToKeyName(jumpKey, gamepadMode), ToKeyName(downKey, gamepadMode), ToKeyName(minigameLB, gamepadMode), ToKeyName(minigameRB, gamepadMode));
}
