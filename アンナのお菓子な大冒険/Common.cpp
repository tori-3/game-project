#include"Common.h"
#include"KeyConfigUtility.h"
#include"KeyInfo.h"

void GameData::saveMiniGameClear()
{
	switch (mini_mode)
	{
	case Stage_Mode:
		if (clearStage < stage)
		{
			firstClearStage = true;
			notifyMiniGameSelect = true;
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
			}
		}

		maxHPList[stage - 1] = 5;
	}
	else
	{
		if(increaseHPMode)
		{
			maxHPList[stage - 1] += 2;
		}
	}

	save();
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
		//json[U"leftKey"] = KeyConfigUtility::ToJSON(leftKey);
		json[U"downKey"] = KeyConfigUtility::ToJSON(downKey);
		//json[U"rightKey"] = KeyConfigUtility::ToJSON(rightKey);
		json[U"pauseKey"] = KeyConfigUtility::ToJSON(pauseKey);
		saveDatajson[U"Key"] = json;
	}

	saveDatajson.save(U"saveData.json");
}

void GameData::load()
{
	JSON saveDatajson = JSON::Load(U"saveData.json");

	stage = saveDatajson[U"SelectedStage"].get<int32>();
	clearStage = saveDatajson[U"ClearStage"].get<int32>();
	GlobalAudio::BusSetVolume(BGMMixBus, saveDatajson[U"BGMVolume"].get<double>());
	GlobalAudio::BusSetVolume(EffectMixBus, saveDatajson[U"EffectVolume"].get<double>());
	increaseHPMode = saveDatajson[U"IncreaseHPMode"].get<bool>();

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
		//leftKey = KeyConfigUtility::FromJSON(json[U"leftKey"]);
		downKey = KeyConfigUtility::FromJSON(json[U"downKey"]);
		//rightKey = KeyConfigUtility::FromJSON(json[U"rightKey"]);
		pauseKey = KeyConfigUtility::FromJSON(json[U"pauseKey"]);
	}

}

String GameData::fmt(StringView text)const
{
	return Fmt(text)(ToKeyName(minigameUpKey), ToKeyName(minigameLeftKey), ToKeyName(minigameDownKey), ToKeyName(minigameRightKey), ToKeyName(attackKey), ToKeyName(jumpKey), ToKeyName(downKey));
}
