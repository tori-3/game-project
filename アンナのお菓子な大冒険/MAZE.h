#pragma once
#include"Common.h"

namespace Maze1 {

	//迷路のサイズ設定
	int mazeWidth;
	int mazeHeight;
	Vec2 ballPos{ 33, 33 }; //ボールの初期位置
	Vec2 cookiePos{ 10,10 };
	Rect goalPos;
	Vec2 gPos;
	Stopwatch stopwatch;
	bool clear = false;

	using namespace std;

	inline Array<Array<int>> generateMaze(int width, int height)
	{
		Array<Array<int>> maze(height, Array<int>(width, 1));
		maze[1][1] = 0;
		stack<pair<int, int>> stack;
		stack.push({ 1, 1 });
		const array<pair<int, int>, 4> directions{ { { -2, 0 },{ 2, 0 },{ 0, -2 },{ 0, 2 } } };

		while (!stack.empty())
		{
			const auto [currentX, currentY] = stack.top();
			stack.pop();
			array<pair<int, int>, 4> shuffledDirections = directions;
			shuffle(shuffledDirections.begin(), shuffledDirections.end(), GetDefaultRNG());

			for (const auto [dx, dy] : shuffledDirections)
			{
				const int nextX = currentX + dx;
				const int nextY = currentY + dy;

				if (nextX > 0 && nextX < width - 1 && nextY > 0 && nextY < height - 1 && maze[nextY][nextX] == 1)
				{
					maze[nextY][nextX] = 0;
					maze[currentY + dy / 2][currentX + dx / 2] = 0;
					stack.push({ nextX, nextY });
				}
			}
		}
		return maze;
	}

	//名前を変更してください
	//class MAZE : public App::Scene
	//{
	//public:
	//	const Font fontMSDF{ FontMethod::MSDF,32 };
	//	const Font fontA{ 48, U"MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
	//	const Font fontB{ 37, U"MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
	//	const Texture game{ U"MAZE/btn053/btn053/btn053_04.png" };
	//	const Texture control{ U"MAZE/btn053/btn053/btn053_02.png" };
	//	const Texture back{ U"MAZE/game0314.png" };
	//	const Audio BGM{ Audio::Stream, U"MAZE/audio/tie no wa.mp3", Loop::Yes };
	//	const Audio click{ U"MAZE/audio/color.mp3" };
	//	Rect start{ 441, 311, 318, 78 };
	//	Rect cont{ 441, 531, 318, 78 };

	//	MAZE(const InitData& init)//名前を変更してください
	//		: IScene{ init }
	//	{
	//		stopwatch.reset();
	//		switch (getData().mini_mode)
	//		{
	//		case Stage_Mode:
	//			mazeWidth = 45;
	//			mazeHeight = 30;
	//			ballPos = { 182, 132 };
	//			goalPos.set(1007, 637, 25, 25);
	//			gPos = { 1007, 637 };
	//			break;
	//		case Easy_Mode:
	//			mazeWidth = 33;
	//			mazeHeight = 23;
	//			ballPos = { 302, 202 };
	//			goalPos.set(887, 587, 25, 25);
	//			gPos = { 887, 587 };
	//			break;
	//		case Normal_Mode:
	//			mazeWidth = 45;
	//			mazeHeight = 30;
	//			ballPos = { 182, 132 };
	//			goalPos.set(1007, 637, 25, 25);
	//			gPos = { 1007, 637 };
	//			break;
	//		case Hard_Mode:
	//			mazeWidth = 60;
	//			mazeHeight = 40;
	//			ballPos = { 32, 32 };
	//			goalPos.set(1137, 737, 25, 25);
	//			gPos = { 1137, 737 };
	//			break;
	//		default:
	//			mazeWidth = 60;
	//			mazeHeight = 40;
	//			ballPos = { 32, 32 };
	//			goalPos.set(1137, 737, 25, 25);
	//			gPos = { 1137, 737 };
	//			break;
	//		}
	//		BGM.setVolume(getData().BGM_volume);
	//		BGM.play();
	//		//変数などの初期化
	//	}

	//	void update() override
	//	{
	//		if (start.leftClicked()) {
	//			click.setVolume(getData().Effect_volume);
	//			click.play();
	//			changeScene(U"Game");
	//		}
	//		if (cont.leftClicked()) {
	//			click.setVolume(getData().Effect_volume);
	//			click.play();
	//			if (clear) EndGame(true);
	//			else EndGame(false);
	//		}							//変数などの更新
	//	}

	//	void draw() const override
	//	{
	//		back.scaled(1.0).draw(0, 0);
	//		FontAsset(U"TitleFont")(U"迷路").drawAt(600, 80);
	//		game.scaled(1.6).draw(440, 310);
	//		control.scaled(1.6).draw(440, 530);
	//		//start.draw();
	//		//cont.draw();
	//		fontA(U"スタート").draw(508, 311, Palette::White);
	//		fontB(U"ゲームを終了する").draw(453, 540, Palette::White);
	//		if (start.mouseOver())Cursor::RequestStyle(CursorStyle::Hand);	//描画(draw関数内ではifの使用、変数の代入などができない)
	//		if (cont.mouseOver())Cursor::RequestStyle(CursorStyle::Hand);
	//	}

	//private:
	//	void EndGame(bool clear) {
	//		getData().mini_clear = clear;//クリア状況保存
	//		if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
	//		else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
	//	}
	//};

	class MAZE : public App::Scene
	{
	public:
		const Font fontMSDF{ FontMethod::MSDF,32 };
		const Font text{ 100 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font watch{ FontMethod::MSDF,22 };
		const Audio easy{ Audio::Stream, U"MiniGameAsset/MAZE/audio/bo-tto hidamari.mp3", Loop::Yes };
		const Audio normal{ Audio::Stream, U"MiniGameAsset/MAZE/audio/Candy Crush.mp3", Loop::Yes };
		const Audio hard{ Audio::Stream, U"MiniGameAsset/MAZE/audio/Mystic Edge.mp3", Loop::Yes };
		//const Texture choco{ U"MAZE/IMG_3688.jpg" };
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
		//const Font watch{ 22 , U"example/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		Vec2 ballVel{ 0, 0 }; //ボールの初期速度
		Vec2 cookieVel{ 0, 0 }; //ボールの初期速度
		Array < Array<int>> maze;	//迷路生成

		double angle = 0;

		Spotlight light;

		MSRenderTexture lightTexture{ Scene::Size(),ColorF{0} };

		MAZE(const InitData& init)
			: IScene{ init }
		{
			stopwatch.reset();
			switch (getData().mini_mode)
			{
			case Stage_Mode:
				mazeWidth = 45;
				mazeHeight = 30;
				ballPos = { 182, 132 };
				goalPos.set(1007, 637, 25, 25);
				gPos = { 1007, 637 };
				break;
			case Easy_Mode:
				mazeWidth = 33;
				mazeHeight = 23;
				ballPos = { 302, 202 };
				goalPos.set(887, 587, 25, 25);
				gPos = { 887, 587 };
				break;
			case Normal_Mode:
				mazeWidth = 45;
				mazeHeight = 30;
				ballPos = { 182, 132 };
				goalPos.set(1007, 637, 25, 25);
				gPos = { 1007, 637 };
				break;
			case Hard_Mode:
				mazeWidth = 60;
				mazeHeight = 40;
				ballPos = { 32, 32 };
				goalPos.set(1137, 737, 25, 25);
				gPos = { 1137, 737 };
				break;
			default:
				mazeWidth = 60;
				mazeHeight = 40;
				ballPos = { 32, 32 };
				goalPos.set(1137, 737, 25, 25);
				gPos = { 1137, 737 };
				break;
			}

			maze = generateMaze(mazeWidth, mazeHeight);
			offsetX = (Scene::Width() - mazeWidth * cellSize) / 2;
			offsetY = (Scene::Height() - mazeHeight * cellSize) / 2;

			stopwatch.start();
			switch (getData().mini_mode)
			{
			case Stage_Mode:
				normal.play(BGMMixBus);
				break;
			case Easy_Mode:
				easy.play(BGMMixBus);
				break;
			case Normal_Mode:
				normal.play(BGMMixBus);
				break;
			case Hard_Mode:
				hard.play(BGMMixBus);
				break;
			default:
				normal.play(BGMMixBus);
				break;
			}


		}


		void update() override
		{

			//ボール操作
			ballVel.x = ((getData().KeyRight.pressed()) - (getData().KeyLeft.pressed()));
			ballVel.y = ((getData().KeyDown.pressed()) - (getData().KeyUp.pressed()));

			if (ballVel != Vec2{ 0,0 })
			{
				angle = ballVel.getAngle();
			}

			ballVel.setLength(Scene::DeltaTime() * 100);

			//Vec2 newPos = ballPos + ballVel;
			//Circle newBall(newPos, 7.3);
			bool collisionX = false;
			bool collisionY = false;

			for (y = 0; y < mazeHeight; ++y)
			{
				for (x = 0; x < mazeWidth; ++x)
				{
					if (maze[y][x] == 1)
					{
						Rect wallRect(x * cellSize + offsetX, y * cellSize + offsetY, cellSize);

						if (Circle{ ballPos + Vec2{ballVel.x,0} ,7.3 }.intersects(wallRect))
						{
							collisionX = true;
						}

						if (Circle{ ballPos + Vec2{0,ballVel.y} ,7.3 }.intersects(wallRect))
						{
							collisionY = true;
						}

						if(collisionX && collisionY)
						{
							break;
						}
					}
				}
				if (collisionX && collisionY) break;
			} //衝突するか

			if (collisionX)
			{
				ballVel.x = 0;
			}

			if (collisionY)
			{
				ballVel.y = 0;
			}

			ballPos += ballVel;

			if (goalPos.intersects(Circle(ballPos, 8))) //重なったら
			{
				goal.play();
				changeScene(U"Clear");
				stopwatch.pause();
			}
			if (KeyEscape.up()) {
				click.play();
				key = true;
			}
			if (key) {
				stopwatch.pause();
				if (restart.leftClicked()) {
					click.play();
					key = false;
					stopwatch.resume();
				}
				if (replay.leftClicked()) {
					click.play();
					changeScene(U"Maze1");
					stopwatch.reset();
					switch (getData().mini_mode)
					{
					case Stage_Mode:
						ballPos = { 182, 132 };
						break;
					case Easy_Mode:
						ballPos = { 302, 202 };
						break;
					case Normal_Mode:
						ballPos = { 182, 132 };
						break;
					case Hard_Mode:
						ballPos = { 32, 32 };
						break;
					default:
						ballPos = { 32, 32 };
						break;
					}
				}
				if (title.leftClicked()) {
					click.play();
					stopwatch.reset();
					EndGame(false);
				}
			}

		}

		void draw() const override
		{
			int x = 0, y = 0;
			Scene::SetBackground(ColorF{0.7});
			//迷路描画
			for (y = 0; y < mazeHeight; ++y)
			{
				for (x = 0; x < mazeWidth; ++x)
				{
					if (maze[y][x] == 1) //ここが壁か
					{
						//Rect(x * cellSize + offsetX, y * cellSize + offsetY, cellSize).draw(Palette::White);
						chocoWall.resized(cellSize).draw(x * cellSize + offsetX, y * cellSize + offsetY);
					}

				}
			}

			//ボール描画
			//Circle(ballPos, 9).draw(Palette::Red);
			//cookie.scaled(0.055).draw(ballPos - cookiePos);

			annna.scaled(0.15).rotated(angle).drawAt(ballPos);

			//ゴール描画
			//goalPos.draw(Palette::Yellow);
			//choco.scaled(0.15).draw(gPos);
			cookie.resized(cellSize).drawAt(goalPos.center());

			{
				const ScopedRenderTarget2D target{ lightTexture };
				const ScopedRenderStates2D blend{ BlendState::Additive };

				Circle{ ballPos,30 }.draw();
			}
			Graphics2D::Flush();
			lightTexture.resolve();

			{
				ScopedSpotlight spot{ light };

				lightTexture.draw(ColorF{ 0.3 });

				Circle{ ballPos,100 }.draw(AlphaF(2), AlphaF(0));
				Circle{ ballPos, 400 }.drawPie(angle - 20_deg, 20_deg * 2,AlphaF(1.0), AlphaF(0.0));

				Circle{ goalPos.center(),50}.draw(AlphaF(1), AlphaF(0));
			}
			light.draw();

			watch(stopwatch).draw(1070, -5, Palette::White);
			if (key) {
				if (SimpleGUI::Button(U"ゲームに戻る", Vec2{ 520, 450 })) {}
				if (SimpleGUI::Button(U"やり直す", Vec2{ 540, 530 })) {}
				if (SimpleGUI::Button(U"タイトルに戻る", Vec2{ 512, 610 })) {}
				//restart.draw(Palette::Black);
				//replay.draw(Palette::Black);
				//title.draw(Palette::Black);
			}
		}

		void EndGame(bool clear) {
			getData().mini_clear = clear;//クリア状況保存
			if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
			else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
		}
	};

	class Clear : public App::Scene
	{
	public:
		const Font text{ 100 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font text2{ 85 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font text3{ 60 , U"MiniGameAsset/MAZE/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bitmap };
		const Font otologic{ FontMethod::MSDF,18 };
		const Audio Bgm{ Audio::Stream, U"MiniGameAsset/MAZE/audio/Loop03-long.mp3", Loop::Yes };
		const Audio click{ U"MiniGameAsset/MAZE/audio/color.mp3" };
		Clear(const InitData& init)
			: IScene{ init } {
			Bgm.play(BGMMixBus);
			clear = true;
		}
		void update() override
		{
			if (KeyEnter.down()) {
				click.play();
				//changeScene(U"Maze1");
				EndGame(clear);
			}
		}
		void draw() const override
		{
			Scene::SetBackground(ColorF(0.2, 0.8, 0.6));

			otologic(U"BGM by OtoLogic").draw(2, 772, Palette::Black);
			text(U"クリア！").draw(415, 100, Palette::Yellow);
			text2(U"タイム: ", stopwatch).draw(215, 300, Palette::Black);
			text3(U"エンターキーを押してマップに戻る").draw(95, 510, Palette::Blue);
		}

		void EndGame(bool clear) {
			getData().mini_clear = clear;//クリア状況保存
			if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
			else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
		}
	};

}
