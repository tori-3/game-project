#pragma once
#include"Common.h"
#include"MiniGameSceneBase.h"

namespace Maze1
{
	//迷路のサイズ設定
	extern int mazeWidth;
	extern int mazeHeight;
	extern Vec2 ballPos; //ボールの初期位置
	extern Vec2 cookiePos;
	extern Rect goalPos;
	extern Vec2 gPos;
	extern Stopwatch stopwatch;
	extern bool clear;

	using namespace std;

	Array<Array<int>> generateMaze(int width, int height);

	class MAZE : public MiniGameSceneBase
	{
	public:
		const Font fontMSDF{ FontMethod::MSDF,32 };
		const Font text{ 100 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font watch{ FontMethod::MSDF,22 };
		const Audio easy{ Audio::Stream, U"MiniGameAsset/MAZE/audio/bo-tto hidamari.mp3", Loop::Yes };
		const Audio normal{ Audio::Stream, U"MiniGameAsset/MAZE/audio/Candy Crush.mp3", Loop::Yes };
		const Audio hard{ Audio::Stream, U"MiniGameAsset/MAZE/audio/Mystic Edge.mp3", Loop::Yes };
		const Texture cookie{ U"MiniGameAsset/MAZE/biscuit1.png" };
		const Audio click{ U"MiniGameAsset/MAZE/audio/color.mp3" };
		const Audio goal{ U"MiniGameAsset/MAZE/audio/bonus.mp3" };

		const Texture annna{ U"MiniGameAsset/MAZE/annna.png" };

		const Texture chocoWall{ U"StageTexture/ChocolateWall.png" };

		int x = 0, y = 0;
		bool key = false;
		Rect restart{ 521, 451, 158, 34 };
		Rect replay{ 540, 531, 120, 34 };
		Rect title{ 513, 611, 178, 34 };
		int cellSize = 20;
		int offsetX;
		int offsetY;
		Vec2 ballVel{ 0, 0 }; //ボールの初期速度
		Vec2 cookieVel{ 0, 0 }; //ボールの初期速度
		Array < Array<int>> maze;	//迷路生成

		double angle = 0;

		Spotlight light;

		MSRenderTexture lightTexture{ Scene::Size(),ColorF{0} };

		MAZE(const InitData& init);

		void onPauseStart()override;

		void onGameStart()override;

		void gameUpdate() override;

		void gameDraw() const override;

	};

	class Clear : public MiniGameSceneBase
	{
	public:
		const Font text{ 100 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font text2{ 85 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font text3{ 60 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font otologic{ FontMethod::MSDF,18 };
		const Audio Bgm{ Audio::Stream, U"MiniGameAsset/MAZE/audio/Loop03-long.mp3", Loop::Yes };
		const Audio click{ U"MiniGameAsset/MAZE/audio/color.mp3" };

		Clear(const InitData& init);

		void gameUpdate() override;

		void gameDraw() const override;
	};

}
