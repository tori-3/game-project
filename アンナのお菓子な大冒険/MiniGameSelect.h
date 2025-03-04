#pragma once
#include"Common.h"
#include"Fire.hpp"
//#include"Spotlight.hpp"

class MiniGameSelect : public App::Scene
{
public:

	struct MiniGameData
	{
		Texture pictures;
		String sentence;
		int32 stageNum;
		String sceneName;
	};

	static constexpr Circle backButton{ {35,35},30 };

	Texture homeIcon{ 0xf015_icon,40 };

	Texture background{ U"BackGroundTexture/洞窟背景.png" };

	Texture signboardHead{U"SignboardHead.png"};
	Texture signboardStick{ U"SignboardStick.png" };

	Texture candle{ U"StageTexture/Candle.png" };

	Array<MiniGameData>miniGameList;

	Spotlight light{ Scene::Size() };

	Fire leftFire{ {75,620},HSV{ 20,0.8,1 },40,{ 0, -30 } };
	Fire rightFire{ {Scene::Width() - 75,620},HSV{ 20,0.8,1 },40,{ 0, -30 } };

	const Font font{ 30 };

	Texture chocolateBeltConveyor{ U"StageTexture/BeltConveyor.png" };


	LongPressInput leftInput{ getData().KeyLeft };
	LongPressInput rightInput{ getData().KeyRight };
	LongPressInput upInput{ getData().KeyUp };
	LongPressInput downInput{ getData().KeyDown };

	static constexpr int32 ChocolateButtonBlockCount = 3;
	static constexpr double ChocolateButtonBlockSize = 80;
	static constexpr Vec2 EasyButtonPos{ 800,500 };
	static constexpr Vec2 NormalButtonPos{ 800,600 };
	static constexpr Vec2 HardButtonPos{ 800,700 };

	bool chocolateButtonUpdate(const Vec2& pos, int32 num, double blockSize)const
	{
		return RectF{ pos,num * blockSize,blockSize }.leftClicked();
	}

	void chocolateButtonDraw(const Vec2&pos,int32 num,double blockSize,StringView text,const ColorF&color,bool star,bool selected)const
	{
		RectF rect{ pos,num * blockSize,blockSize };
		const bool mouseOver = rect.mouseOver();
		for(int32 i=0;i<num;++i)
		{
			beltConveyorLeft(pos + Vec2{ i * blockSize,0 },blockSize, mouseOver|| selected);
		}

		rect.drawFrame(selected?8:2, color);

		if(star)
		{
			Shape2D::Star(blockSize * 0.3, { rect.x + blockSize * 0.5,rect.center().y }).draw(Palette::Yellow);
		}
		else
		{
			Shape2D::Star(blockSize * 0.3, { rect.x + blockSize * 0.5,rect.center().y }).drawFrame(3, Palette::White);
		}

		font(text).drawAt(RectF{ pos + Vec2{blockSize*0.8,0},(num-1) * blockSize,blockSize }.center());
	}

	void beltConveyorLeft(const Vec2& pos,double blockSize,bool move)const
	{
		const double size = chocolateBeltConveyor.size().x;
		double d = 1;
		if(move)
		{
			d = 1 - Periodic::Sawtooth0_1(2s);
		}
		chocolateBeltConveyor(size - size * d, 0, size * d, size).resized(blockSize * d, blockSize).draw(pos);
		chocolateBeltConveyor(0, 0, size - size * d, size).resized(blockSize * (1 - d), blockSize).draw((pos + Vec2{ d* blockSize,0 }));
	}





	MiniGameSelect(const InitData& init)
		: IScene{ init }
	{
		JSON json = JSON::Load(U"map.json");

		for(const auto& object: json[U"MiniGameStage"].arrayView())
		{
			const int32 stageNum = object.get<int32>();

			MiniGameData data;

			JSON miniGame=json[U"StageData"][U"Stage{}"_fmt(stageNum)];

			data.pictures = Texture{ miniGame[U"Picture"].getString() };
			data.sentence = miniGame[U"Sentence"].getString();
			data.stageNum = stageNum;
			data.sceneName = miniGame[U"SceneName"].getString();
			miniGameList << data;
		}

		AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
		BGMManager::get().play(U"WorldBGM");
	}

	void update()override
	{
		if (backButton.leftClicked() || KeyQ.down())
		{
			changeScene(U"TitleScene");
		}

		if (backButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		getData().miniGameIndex = Clamp<int32>(getData().miniGameIndex + rightInput.down() - leftInput.down(), 0, miniGameList.size() - 1);
		getData().miniGameModeIndex = Clamp<int32>(getData().miniGameModeIndex + downInput.down() - upInput.down(), 0, 2);
		
		for (int32 i = 0; i < miniGameList.size(); ++i)
		{
			int32 x = Scene::Width() / 7 * (i + 1);
			int32 y = 150 + i % 2 * 200;
			RectF rect = signboardHead.resized(220 * 1.2).regionAt(x, y);

			if(rect.mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);

				if(MouseL.down())
				{
					getData().miniGameIndex = i;
				}
			}

		}

		const int32 modeIndex = getData().miniGameModeIndex;

		if (chocolateButtonUpdate(EasyButtonPos, ChocolateButtonBlockCount, ChocolateButtonBlockSize) || (modeIndex == 0 && KeyEnter.down()))
		{
			getData().mini_mode = mode::Easy_Mode;
			getData().sceneName = miniGameList[getData().miniGameIndex].sceneName;
			changeScene(miniGameList[getData().miniGameIndex].sceneName);
			BGMManager::get().stop();
		}

		if (chocolateButtonUpdate(NormalButtonPos, ChocolateButtonBlockCount, ChocolateButtonBlockSize) || (modeIndex == 1 && KeyEnter.down()))
		{
			getData().mini_mode = mode::Normal_Mode;
			getData().sceneName = miniGameList[getData().miniGameIndex].sceneName;
			changeScene(miniGameList[getData().miniGameIndex].sceneName);
			BGMManager::get().stop();
		}

		if (chocolateButtonUpdate(HardButtonPos, ChocolateButtonBlockCount, ChocolateButtonBlockSize) || (modeIndex == 2 && KeyEnter.down()))
		{
			getData().mini_mode = mode::Hard_Mode;
			getData().sceneName = miniGameList[getData().miniGameIndex].sceneName;
			changeScene(miniGameList[getData().miniGameIndex].sceneName);
			BGMManager::get().stop();
		}

		leftFire.update();
		rightFire.update();
	}

	void draw()const override
	{
		background.resized(Scene::Width()).draw();

		const int32 gameIndex = getData().miniGameIndex;
		const int32 modeIndex = getData().miniGameModeIndex;

		for (int32 i = 0; i < miniGameList.size(); ++i)
		{
			int32 x = Scene::Width() / 7 * (i + 1);
			int32 y = 150 + i % 2 * 200;

			signboardStick.resized(30,y).drawAt(x, y/2.0);
			signboardHead.resized(220*1.2).drawAt(x, y);
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
			Circle{ {Scene::Width()- 75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
		}
		light.draw();

		leftFire.draw();
		rightFire.draw();


		font(miniGameList[gameIndex].sentence).draw(150,Scene::Center().y+100);

		chocolateButtonDraw(EasyButtonPos, ChocolateButtonBlockCount, ChocolateButtonBlockSize,U"イージー",Palette::Greenyellow,getData().miniGameList[gameIndex].easyClear, modeIndex == 0);
		chocolateButtonDraw(NormalButtonPos, ChocolateButtonBlockCount, ChocolateButtonBlockSize,U"ノーマル",Palette::Orange, getData().miniGameList[gameIndex].normalClear, modeIndex == 1);
		chocolateButtonDraw(HardButtonPos, ChocolateButtonBlockCount, ChocolateButtonBlockSize, U"ハード", Palette::Red, getData().miniGameList[gameIndex].hardClear, modeIndex == 2);

		homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);

		FontAsset{ U"NormalFont" }(U"[Q]タイトルに戻る").draw(Arg::leftCenter=backButton.center+Vec2{30,0});
	}
};
