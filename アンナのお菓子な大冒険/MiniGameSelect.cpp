#include"Spotlight.hpp"
#include"MiniGameSelect.h"
#include"BGMManager.hpp"
#include"StarDustEffect.h"
#include"KeyInfo.h"

MiniGameSelect::MiniGameSelect(const InitData& init)
	: IScene{ init }
{
	if(getData().notifyMiniGameSelect)
	{
		getData().notifyMiniGameSelect = false;
		getData().save();
	}

	if (getData().firstClearMinigame)
	{
		if (getData().miniGameModeIndex == 0)
		{
			easyButton.firstClearMode = true;
		}
		else if (getData().miniGameModeIndex == 1)
		{
			normalButton.firstClearMode = true;
		}
		else
		{
			hardButton.firstClearMode = true;
		}
	}


	JSON json = JSON::Load(U"map.json");

	for (const auto& object : json[U"MiniGameStage"].arrayView())
	{
		const int32 stageNum = object.get<int32>();

		if (getData().clearStage < stageNum)
		{
			continue;
		}

		MiniGameData data;

		JSON miniGame = json[U"StageData"][U"Stage{}"_fmt(stageNum)];

		data.pictures = Texture{ miniGame[U"Picture"].getString() };
		data.sentence = miniGame[U"Sentence"].getString();
		data.stageNum = stageNum;
		data.sceneName = miniGame[U"SceneName"].getString();
		miniGameList << data;
	}

	//AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
	BGMManager::get().play(U"BGM/WorldMap2.mp3");
}

void MiniGameSelect::update()
{
	leftFire.update();
	rightFire.update();

	if(getData().firstClearMinigame)
	{
		if (getData().miniGameModeIndex == 0)
		{
			if(not easyButton.updateFirstClearMode())
			{
				effect.add<StarDustEffect>(easyButton.getStarPos());
				getData().firstClearMinigame = false;
			}
		}
		else if (getData().miniGameModeIndex == 1)
		{
			if (not normalButton.updateFirstClearMode())
			{
				effect.add<StarDustEffect>(normalButton.getStarPos());
				getData().firstClearMinigame = false;
			}
		}
		else
		{
			if (not hardButton.updateFirstClearMode())
			{
				effect.add<StarDustEffect>(hardButton.getStarPos());
				getData().firstClearMinigame = false;
			}
		}

		return;
	}

	if (backButton.leftClicked() || getData().menuBackKey.down())
	{
		AudioAsset{ U"決定ボタン" }.playOneShot();
		changeScene(U"TitleScene");
	}

	if (backButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (miniGameList.isEmpty())
	{
		return;
	}

	if(rightInput.down())
	{
		if(getData().miniGameIndex< miniGameList.size() - 1)
		{
			++getData().miniGameIndex;
			AudioAsset{ U"カーソル移動" }.playOneShot();
		}
		else
		{
			AudioAsset{ U"ビープ音" }.playOneShot();
		}
	}

	if (leftInput.down())
	{
		if (0<getData().miniGameIndex)
		{
			--getData().miniGameIndex;
			AudioAsset{ U"カーソル移動" }.playOneShot();
		}
		else
		{
			AudioAsset{ U"ビープ音" }.playOneShot();
		}
	}

	if (downInput.down())
	{
		if (getData().miniGameModeIndex < 2)
		{
			++getData().miniGameModeIndex;
			AudioAsset{ U"カーソル移動" }.playOneShot();
		}
		else
		{
			AudioAsset{ U"ビープ音" }.playOneShot();
		}
	}

	if (upInput.down())
	{
		if (0 < getData().miniGameModeIndex)
		{
			--getData().miniGameModeIndex;
			AudioAsset{ U"カーソル移動" }.playOneShot();
		}
		else
		{
			AudioAsset{ U"ビープ音" }.playOneShot();
		}
	}

	for (int32 i = 0; i < miniGameList.size(); ++i)
	{
		int32 x = Scene::Width() / 7 * (i + 1);
		int32 y = 150 + i % 2 * 200;
		RectF rect = signboardHead.resized(220 * 1.2).regionAt(x, y);

		if (rect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);

			if (MouseL.down())
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();
				getData().miniGameIndex = i;
			}
		}

	}

	const int32 modeIndex = getData().miniGameModeIndex;

	bool startMiniGame = false;

	if (easyButton.update() || (modeIndex == 0 && getData().menuDecisionKey.down()))
	{
		getData().miniGameModeIndex = 0;
		getData().mini_mode = mode::Easy_Mode;
		startMiniGame = true;
	}

	if (normalButton.update() || (modeIndex == 1 && getData().menuDecisionKey.down()))
	{
		getData().miniGameModeIndex = 1;
		getData().mini_mode = mode::Normal_Mode;
		startMiniGame = true;
	}

	if (hardButton.update() || (modeIndex == 2 && getData().menuDecisionKey.down()))
	{
		getData().miniGameModeIndex = 2;
		getData().mini_mode = mode::Hard_Mode;
		startMiniGame = true;
	}

	if (startMiniGame)
	{
		getData().initGame();
		getData().sceneName = miniGameList[getData().miniGameIndex].sceneName;
		getData().description = miniGameList[getData().miniGameIndex].sentence;
		changeScene(miniGameList[getData().miniGameIndex].sceneName);
		BGMManager::get().stop();
		AudioAsset{ U"決定ボタン" }.playOneShot();
	}
}

void MiniGameSelect::draw()const
{
	background.resized(Scene::Width()).draw();

	const int32 gameIndex = getData().miniGameIndex;
	const int32 modeIndex = getData().miniGameModeIndex;

	for (int32 i = 0; i < miniGameList.size(); ++i)
	{
		int32 x = Scene::Width() / 7 * (i + 1);
		int32 y = 150 + i % 2 * 200;

		signboardStick.resized(30, y).drawAt(x, y / 2.0);
		signboardHead.resized(220 * 1.2).drawAt(x, y);
		miniGameList[i].pictures.resized(190).drawAt(x, y);
	}

	candle.drawAt(75, 700);
	candle.drawAt(Scene::Width() - 75, 700);


	{
		ScopedSpotlight target{ light,ColorF{0.4} };

		if (not miniGameList.isEmpty())
		{

			int32 x = Scene::Width() / 7 * (gameIndex + 1);
			int32 y = 150 + gameIndex % 2 * 200;
			Circle{ x,y,200 }.draw(ColorF{ 2.0 }, ColorF{ 0.0 });
		}

		Circle{ {75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
		Circle{ {Scene::Width() - 75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
	}
	light.draw();


	if (miniGameList.isEmpty())
	{
		FontAsset{ U"NormalFont" }(U"まだ遊べるミニゲームはありません").drawAt(50, Scene::Center());
		FontAsset{ U"NormalFont" }(U"ミニゲームをクリアすると追加されます").drawAt(25, Scene::Center() + Vec2{ 0,50 });
	}

	leftFire.draw();
	rightFire.draw();

	if (not miniGameList.isEmpty())
	{
		FontAsset{ U"NormalFont" }(getData().fmt(miniGameList[gameIndex].sentence)).draw(30, 150, Scene::Center().y + 100 - 20);

		easyButton.draw(chocolateBeltConveyor, FontAsset{ U"NormalFont" }, U"イージー", Palette::Greenyellow, getData().miniGameList[gameIndex].easyClear, modeIndex == 0);
		normalButton.draw(chocolateBeltConveyor, FontAsset{ U"NormalFont" }, U"ノーマル", Palette::Orange, getData().miniGameList[gameIndex].normalClear, modeIndex == 1);
		hardButton.draw(chocolateBeltConveyor, FontAsset{ U"NormalFont" }, U"ハード", Palette::Red, getData().miniGameList[gameIndex].hardClear, modeIndex == 2);

		drawKey(Vec2{ 750,560 }, getData().minigameUpKey, U"難易度↑");
		drawKey(Vec2{ 750,640 }, getData().menuDecisionKey, U"はじめる");
		drawKey(Vec2{ 750,720 }, getData().minigameDownKey, U"難易度↓");
	}

	if (1 < miniGameList.size())
	{
		drawKey(Vec2{ 100,350 }, getData().minigameLeftKey, U"←");
		drawKey(Vec2{ Scene::Width() - 100,150 }, getData().minigameRightKey, U"→");
	}

	homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);

	FontAsset{ U"NormalFont" }(U"{}-タイトルに戻る"_fmt(ToKeyName(getData().menuBackKey, getData().gamepadMode))).draw(30, Arg::leftCenter = backButton.center + Vec2{ 30,0 });

	{
		const ScopedRenderStates2D blend{ BlendState::Additive };
		effect.update();
	}
}

void MiniGameSelect::drawKey(const Vec2& pos,const InputGroup& inputs, StringView text)const
{
	FontAsset{ U"NormalFont" }(ToKeyName(inputs, getData().gamepadMode)).drawAt(30, pos);
	FontAsset{ U"NormalFont" }(text).drawAt(20, pos + Vec2{ 0,30 });
}
