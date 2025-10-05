#include"Spotlight.hpp"
#include"MAZE.h"
#include"ControllerManager.h"

namespace Maze1
{
	int mazeWidth;
	int mazeHeight;
	Vec2 ballPos{ 33, 33 }; //ボールの初期位置
	Vec2 cookiePos{ 10,10 };
	Rect goalPos;
	Vec2 gPos;
	Stopwatch stopwatch;
	bool clear = false;

	Array<Array<int>> generateMaze(int width, int height)
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


	MAZE::MAZE(const InitData& init)
		: MiniGameSceneBase{ init }
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

	void MAZE::onPauseStart()
	{
		stopwatch.pause();
	}

	void MAZE::onGameStart()
	{
		stopwatch.resume();
	}

	void MAZE::gameUpdate()
	{

		//ボール操作
		ballVel.x = ((getData().minigameRightKey.pressed() || ControllerManager::get().RightPressed()) - (getData().minigameLeftKey.pressed() || ControllerManager::get().LeftPressed()));
		ballVel.y = ((getData().minigameDownKey.pressed()|| ControllerManager::get().DownPressed()) - (getData().minigameUpKey.pressed() || ControllerManager::get().UpPressed()));

		if (ballVel != Vec2{ 0,0 })
		{
			angle = ballVel.getAngle();
		}

		ballVel.setLength(Scene::DeltaTime() * 100);

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

					if (collisionX && collisionY)
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
			getData().saveMiniGameClear();
			goal.play();
			changeScene(U"Clear");
			stopwatch.pause();
		}

	}

	void MAZE::gameDraw() const
	{
		int x = 0, y = 0;
		Scene::SetBackground(ColorF{ 0.7 });
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

		annna.scaled(0.15).rotated(angle).drawAt(ballPos);

		//ゴール描画
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
			Circle{ ballPos, 400 }.drawPie(angle - 20_deg, 20_deg * 2, AlphaF(1.0), AlphaF(0.0));

			Circle{ goalPos.center(),50 }.draw(AlphaF(1), AlphaF(0));
		}
		light.draw();

		watch(stopwatch).draw(1070, -5, Palette::White);

		FontAsset{ U"NormalFont" }(U"[ESC]ポーズ").draw(Vec2{ 10,5 });
	}

	Clear::Clear(const InitData& init)
		: MiniGameSceneBase{ init }
	{
		Bgm.play(BGMMixBus);
		clear = true;
	}

	void Clear::gameUpdate()
	{
		if (getData().menuDecisionKey.down())
		{
			click.play();
			//changeScene(U"Maze1");
			EndGame(clear);
		}
	}

	void Clear::gameDraw() const
	{
		Scene::SetBackground(ColorF(0.2, 0.8, 0.6));

		otologic(U"BGM by OtoLogic").draw(2, 772, Palette::Black);
		text(U"クリア！").draw(415, 100, Palette::Yellow);
		text2(U"タイム: ", stopwatch).draw(215, 300, Palette::Black);
		text3(U"エンターキーを押してマップに戻る").draw(95, 510, Palette::Blue);
	}

}
