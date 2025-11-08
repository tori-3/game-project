#include"Spotlight.hpp"
#include"Maze2.h"
#include"ControllerManager.h"
#include"KeyInfo.h"

namespace Maze2
{
	mazeGame::mazeGame(const InitData& init)//名前を変更してください
		: MiniGameSceneBase{ init,true }
	{
		BGMManager::get().play(U"MiniGameBGM");


		//変数などの初期化

		if (getData().mini_mode == Stage_Mode) {
			maze.make(21);
		}
		else if (getData().mini_mode == Easy_Mode) {
			maze.make(19);
		}
		else if (getData().mini_mode == Normal_Mode) {
			maze.make(23);
		}
		else if (getData().mini_mode == Hard_Mode) {
			maze.make(27);
		}
		player.mazeUpdate(maze.data, maze);
		rotate.cpos = 20 * Vec2{ maze.data[0].size(),maze.data.size() } / 2.0;
		rotate.pos = Vec2{ 1200,800 } / 2.0;
	}

	void mazeGame::gameUpdate()
	{
		if (clear)
		{
			if (getData().menuDecisionKey.down())
			{
				EndGame(true);
			}
			return;
		}

		if (judge.reacts(player.pos, player.size, maze.gpos, Vec2(1, 1))) {
			AudioAsset{ U"ミニゲームクリア" }.playOneShot();
			clear = true;
			getData().saveMiniGameClear();
			//maze.make(23);
			//rotate.cpos = 20*Vec2{ maze.data[0].size(),maze.data.size() }/2.0;
			//player.mazeUpdate(maze.data,maze);
		}
		//player.vel = Vec2(KeyRight.pressed()-KeyLeft.pressed(), -KeyUp.pressed()+ KeyDown.pressed())/10;
		rotate.angle_vel = 0.02 * ((getData().minigameRightKey.pressed()|| ControllerManager::get().RightPressed()) - (getData().minigameLeftKey.pressed()|| ControllerManager::get().LeftPressed()));
		//if (MouseL.pressed()) {
		//	rotate.angle_vel = Cursor::DeltaF().x / 100.0;
		//}
		rotate.angle += rotate.angle_vel;
		//rotate.angle = Cursor::Pos().x/100.0;
		player.update(rotate, rotate.angle_vel * 0.1);
		//maze.draw(rotate);
		//player.draw(rotate);
		//変数などの更新
	}

	void mazeGame::gameDraw() const
	{
		TextureAsset{ U"BackGroundTexture/洞窟背景.png" }.draw();

		//描画(draw関数内ではifの使用、変数の代入などができない)
		maze.draw(rotate);
		player.draw(rotate);

		{
			ScopedSpotlight spot{ light,ColorF{0.2} };
			Circle{ Scene::Center(),500 }.draw(ColorF{ 1.0 }, ColorF{ 0 });
		}
		light.draw();


		FontAsset{ U"NormalFont" }(U"{}-\U000F099B左回転          {}-\U000F0453右回転"_fmt(ToKeyName(getData().minigameLeftKey, getData().gamepadMode), ToKeyName(getData().minigameRightKey, getData().gamepadMode))).drawAt(35,Scene::Center().x,Scene::Height()-70);

		if (clear)
		{
			Scene::Rect().draw(ColorF{ 0,0.5 });
			FontAsset{ U"NormalFont" }(U"Clear!!").drawAt(150, Scene::Center());
			FontAsset{ U"NormalFont" }(U"{}で戻る"_fmt(ToKeyName(getData().menuDecisionKey, getData().gamepadMode))).drawAt(45, Scene::Center() + Vec2{ 0,150 });
		}

		FontAsset{ U"NormalFont" }(U"{} ポーズ"_fmt(ToKeyName(getData().pauseKey, getData().gamepadMode))).draw(30,Arg::topRight = Vec2{ Scene::Width() - 10,5 });
	}
}
