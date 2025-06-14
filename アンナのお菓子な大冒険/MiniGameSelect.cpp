#include"Spotlight.hpp"
#include"MiniGameSelect.h"
#include"BGMManager.hpp"

MiniGameSelect::MiniGameSelect(const InitData& init)
	: IScene{ init }
{
	JSON json = JSON::Load(U"map.json");

	for (const auto& object : json[U"MiniGameStage"].arrayView())
	{
		const int32 stageNum = object.get<int32>();

		MiniGameData data;

		JSON miniGame = json[U"StageData"][U"Stage{}"_fmt(stageNum)];

		data.pictures = Texture{ miniGame[U"Picture"].getString() };
		data.sentence = miniGame[U"Sentence"].getString();
		data.stageNum = stageNum;
		data.sceneName = miniGame[U"SceneName"].getString();
		miniGameList << data;
	}

	AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
	BGMManager::get().play(U"WorldBGM");
}

void MiniGameSelect::update()
{
	if (backButton.leftClicked() || getData().menuBackKey.down())
	{
		AudioAsset{ U"決定ボタン" }.playOneShot();
		changeScene(U"TitleScene");
	}

	if (backButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
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
		getData().mini_mode = mode::Easy_Mode;
		startMiniGame = true;
	}

	if (normalButton.update() || (modeIndex == 1 && getData().menuDecisionKey.down()))
	{
		getData().mini_mode = mode::Normal_Mode;
		startMiniGame = true;
	}

	if (hardButton.update() || (modeIndex == 2 && getData().menuDecisionKey.down()))
	{
		getData().mini_mode = mode::Hard_Mode;
		startMiniGame = true;
	}

	if (startMiniGame)
	{
		getData().sceneName = miniGameList[getData().miniGameIndex].sceneName;
		getData().description = miniGameList[getData().miniGameIndex].sentence;
		changeScene(miniGameList[getData().miniGameIndex].sceneName);
		BGMManager::get().stop();
		AudioAsset{ U"決定ボタン" }.playOneShot();
	}

	leftFire.update();
	rightFire.update();
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

		int32 x = Scene::Width() / 7 * (gameIndex + 1);
		int32 y = 150 + gameIndex % 2 * 200;
		Circle{ x,y,200 }.draw(ColorF{ 2.0 }, ColorF{ 0.0 });

		Circle{ {75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
		Circle{ {Scene::Width() - 75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
	}
	light.draw();

	leftFire.draw();
	rightFire.draw();


	font(miniGameList[gameIndex].sentence).draw(150, Scene::Center().y + 100);

	easyButton.draw(chocolateBeltConveyor, font, U"イージー", Palette::Greenyellow, getData().miniGameList[gameIndex].easyClear, modeIndex == 0);
	normalButton.draw(chocolateBeltConveyor, font, U"ノーマル", Palette::Orange, getData().miniGameList[gameIndex].normalClear, modeIndex == 1);
	hardButton.draw(chocolateBeltConveyor, font, U"ハード", Palette::Red, getData().miniGameList[gameIndex].hardClear, modeIndex == 2);

	homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);

	FontAsset{ U"NormalFont" }(U"[Q]タイトルに戻る").draw(Arg::leftCenter = backButton.center + Vec2{ 30,0 });
}
