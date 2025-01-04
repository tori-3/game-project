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
	};

	static constexpr Circle backButton{ {35,35},30 };

	Texture homeIcon{ 0xf015_icon,40 };

	Texture background{ U"BackGroundTexture/洞窟背景.png" };

	Texture signboardHead{U"SignboardHead.png"};
	Texture signboardStick{ U"SignboardStick.png" };

	Texture candle{ U"StageTexture/Candle.png" };

	Array<MiniGameData>miniGameList;

	Spotlight light{ Scene::Size() };

	size_t index = 0;

	Fire leftFire{ {75,620},HSV{ 20,0.8,1 },40,{ 0, -30 } };
	Fire rightFire{ {Scene::Width() - 75,620},HSV{ 20,0.8,1 },40,{ 0, -30 } };

	const Font font{ 30 };

	Texture chocolateBeltConveyor{ U"StageTexture/BeltConveyor.png" };

	void chocolateButton(const Vec2&pos,int32 num,double blockSize,StringView text,const ColorF&color,bool star)const
	{
		RectF rect{ pos,num * blockSize,blockSize };
		const bool mouseOver = rect.mouseOver();
		for(int32 i=0;i<num;++i)
		{
			beltConveyorLeft(pos + Vec2{ i * blockSize,0 },blockSize, mouseOver);
		}

		rect.drawFrame(2, color);

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

			miniGameList << data;
		}

		AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
		BGMManager::get().play(U"WorldBGM");
	}

	void update()override
	{
		if (backButton.leftClicked())
		{
			changeScene(U"TitleScene");
		}

		if (backButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		index = Clamp<int32>(index + KeyD.down() - KeyA.down(), 0, miniGameList.size() - 1);

		leftFire.update();
		rightFire.update();

	}

	void draw()const override
	{
		background.resized(Scene::Width()).draw();


		for (int32 i = 0; i < miniGameList.size(); ++i)
		{
			int32 x = Scene::Width() / 7 * (i + 1);
			int32 y = 150 + i % 2 * 200;

			signboardStick.resized(30,y).drawAt(x, y/2.0);
			signboardHead.resized(220*1.2).drawAt(x, y);
			miniGameList[i].pictures.resized(190).drawAt(x, y);

			candle.drawAt(75, 700);
			candle.drawAt(Scene::Width() - 75, 700);
		}

		{
			ScopedSpotlight target{ light,ColorF{0.4} };

			int32 x = Scene::Width() / 7 * (index + 1);
			int32 y = 150 + index % 2 * 200;
			Circle{ x,y,200 }.draw(ColorF{ 2.0 }, ColorF{ 0.0 });

			Circle{ {75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
			Circle{ {Scene::Width()- 75,600},100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
		}
		light.draw();

		leftFire.draw();
		rightFire.draw();


		font(miniGameList[index].sentence).draw(150,Scene::Center().y+100);

		chocolateButton({ 800,500 }, 3, 80,U"イージー",Palette::Greenyellow,true);
		chocolateButton({ 800,600 }, 3, 80,U"ノーマル",Palette::Orange,true);
		chocolateButton({ 800,700 }, 3, 80,U"ハード",Palette::Red,false);


		homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);

	}


};
