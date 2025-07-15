#include"Common.h"
#include"KeyConfigUtility.h"

void GameData::saveMiniGameClear()
{
	switch (mini_mode)
	{
	case Stage_Mode:
		if (clearStage < stage)
		{
			clearStage = stage;
			save();
		}
		break;
	case Easy_Mode:
		if (not miniGameList[miniGameIndex].easyClear)
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

void GameData::save()
{
	JSON saveDatajson;
	saveDatajson[U"SelectedStage"] = stage;
	saveDatajson[U"ClearStage"] = clearStage;
	saveDatajson[U"BGMVolume"] = GlobalAudio::BusGetVolume(BGMMixBus);
	saveDatajson[U"EffectVolume"] = GlobalAudio::BusGetVolume(EffectMixBus);
	saveDatajson[U"IncreaseHPMode"] = increaseHPMode;

	for (int32 maxHP : maxHPList)
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
		json[U"leftKey"] = KeyConfigUtility::ToJSON(leftKey);
		json[U"downKey"] = KeyConfigUtility::ToJSON(downKey);
		json[U"rightKey"] = KeyConfigUtility::ToJSON(rightKey);
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

	maxHPList.clear();
	for (const auto& obj : saveDatajson[U"MaxHP"].arrayView())
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
		leftKey = KeyConfigUtility::FromJSON(json[U"leftKey"]);
		downKey = KeyConfigUtility::FromJSON(json[U"downKey"]);
		rightKey = KeyConfigUtility::FromJSON(json[U"rightKey"]);
		pauseKey = KeyConfigUtility::FromJSON(json[U"pauseKey"]);
	}

}
