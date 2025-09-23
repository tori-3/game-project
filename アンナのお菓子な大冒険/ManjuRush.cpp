#include"ManjuRush.h"

namespace ManjuRush
{
	void ManjuRush::onPauseStart()
	{
		stopwatch.pause();
		jumptime.pause();
	}

	void ManjuRush::onGameStart()
	{
		stopwatch.resume();
		jumptime.resume();
	}

	ManjuRush::ManjuRush(const InitData& init)
		: MiniGameSceneBase{ init }
	{

		enemys << RectF{ 1100,player.y + player.h - 16 - 50,80,60 };
		enemys << RectF{ 2000,player.y - 100,80,60 };
		enemys << RectF{ 1500,player.y - 350 + 5,80,60 };
		enemys << RectF{ 2500,370,80,60 };
	}

	void ManjuRush::gameUpdate()
	{
		if (flag == false) {
			if (stopwatch < 2s) {
				font(U"3").drawAt(600, 200+90);
			}
			else if (stopwatch < 3s) {
				font(U"2").drawAt(600, 200 + 90);
			}
			else if (stopwatch < 4s) {
				font(U"1").drawAt(600, 200 + 90);
			}
			else if (stopwatch < 5.5s) {
				font(U"スタート!").drawAt(600, 200 + 90);
			}
			else if (stopwatch > 5.5s) {
				flag = true;
			}
		}
		else {
			if (!defeat) {
				BGM.play(BGMMixBus);
			}

			if (score > 1300 || getData().mini_mode == Hard_Mode) {
				level = 3;
			}
			else if (score > 500 || getData().mini_mode == Normal_Mode) {
				level = 2;
			}


			//Clear
			if (score == 1300) {
				if (clear == false) {
					//stopwatch.restart();
					Clear.play();
					getData().saveMiniGameClear();
				}
				clear = true;
				//if (stopwatch > 3s) {
				//	EndGame(true);
				//}

				if (KeyEnter.down())
				{
					EndGame(true);
				}
			}
			//プレイヤー更新
			if (!defeat) {
				//重力
				if (player.y < 500) {
					player.y += g;
				}
				else if (player.y != 500 && !clear) {
					player.y = 500;
					score += 100;
					if (score != 1300) {
						Score.playOneShot();
					}
					effect.add<BlockEffect>(Vec2(player.x + player.w / 2, player.y + player.h));
					if (score < 1000) {
						speed += 0.5;
					}
				}
				//スコア
				font(U"スコア:", score).draw(600, 0);
				//ジャンプ

				if (getData().minigameUpKey.pressed() && jump == false)
				{
					jump = true;

					v = 22;

					jumptime.restart();

				}
				//上昇
				if (0s < jumptime && jumptime < 0.4s && 0 < v && getData().minigameUpKey.pressed()) {
					jump = true;
					player.y -= v;
				}
				else if (0 < v && player.y <= 500) {
					//下降
					if (getData().minigameDownKey.pressed()) {
						player.y += 5;
					}
					//減衰
					jump = true;
					v -= 0.2;
					player.y -= v;
				}
				else {
					v = 0;
					jump = false;
				}
			}


			//敵更新
			if (!clear && !defeat) {
				for (int i = 0; i < enemys.size(); i++) {
					enemys[i].x -= speed;
				}
				if (level == 1) {
					enemys[2].x += speed;
				}
				if (level == 1 || level == 2) {
					enemys[3].x += speed;
				}

				rad1 -= 0.1;

				if (level == 3) {
					if (enemys[3].y + (Periodic::Sine0_1(4s) - 0.5) * 300 != 380) {
						enemys[3].y = 380;
					}
					enemys[3].y -= (Periodic::Sine0_1(4s) - 0.5) * 300;
					rad3 -= 0.2;
					enemys[3].x += speed / 4;
				}
				for (int i = 0; i < enemys.size(); i++) {
					if (enemys[i].x < -200) {
						enemys[i].x = Random(1300, 2100);
					}
				}
			}

			//GameOver
			for (int i = 0; i < enemys.size(); i++) {
				if (player.intersects(enemys[i])) {
					if (defeat == false) {
						stopwatch.restart();
						Touch.play();
						BGM.stop();
					}
					defeat = true;
					if (stopwatch > 2s) {
						Defeat.play();
					}
					if (stopwatch > 4s) {
						EndGame(false);
					}
				}
			}
		}
	}

	void ManjuRush::gameDraw() const
	{
		player.draw(ColorF{ 1,1,1,0 });
		//*
		enemys[0].draw(ColorF{ 1,1,1,0 });
		enemys[1].rotated(rad1).draw(ColorF{ 1,1,1,0 });
		if (level == 2 || level == 3) {
			enemys[2].rotated(rad1).draw(ColorF{ 1,1,1,0 });
		}
		if (level == 3) {
			enemys[3].rotated(rad3).draw(ColorF{ 1,1,1,0 });
		}
		Scene::SetBackground(Palette::Lightpink);
		//*/

		kumo.scaled(0.45).draw(100, 50);
		kumo.scaled(0.35).draw(450, 125);
		kumokumo.scaled(0.65).draw(800, 150);
		yama_l.scaled(0.65).draw(1000, 300);
		manju_l.scaled(0.65).draw(550, 400);
		yama_m.scaled(0.45).draw(300, 415);
		manju_m.scaled(0.45).draw(900, 473);
		yama_s.scaled(0.3).draw(0, 515);
		yama_s.scaled(0.3).draw(825, 515);
		manju_s.scaled(0.3).draw(220, 530);

		for (int i = 0; i < 20; i++) {
			renga.scaled(0.25).draw(-100 + 100 * i, 600);
			renga.scaled(0.25).draw(-100 + 100 * i, 700);
		}

		effect.update();

		if (jump == true && !defeat) {
			if (jumptime < 0.15s)
			{
				anna2_j1.scaled(0.2).draw(player.x - 10, player.y);
			}
			else if (0 < v - g)
			{
				anna2_j2.scaled(0.2).draw(player.x - 10, player.y);
			}
			else
			{
				anna2_j3.scaled(0.2).draw(player.x - 10, player.y);
			}
		}
		else if (!defeat) {

			anna.scaled(0.2).draw(player.x - 10, player.y);
		}
		else {
			anna2_d.scaled(0.2).draw(player.x - 10, player.y);
		}
		manju.scaled(0.8).draw(enemys[0].x - 10, enemys[0].y);
		manju.scaled(0.8).rotatedAt(Vec2{ enemys[0].w / 2 + 10,enemys[0].h / 2 }, rad1).draw(enemys[1].x - 10, enemys[1].y);

		if (level == 2 || level == 3) {
			manju.scaled(0.8).rotatedAt(Vec2{ enemys[0].w / 2 + 10,enemys[0].h / 2 }, rad1).draw(enemys[2].x - 10, enemys[2].y);
		}
		if (level == 3) {
			manju.scaled(0.8).rotatedAt(Vec2{ enemys[0].w / 2 + 10,enemys[0].h / 2 }, rad3).draw(enemys[3].x - 10, enemys[3].y);
		}

		if (clear)
		{
			Scene::Rect().draw(ColorF{ 0,0.5 });
			FontAsset{ U"TitleFont" }(U"Clear!!").drawAt(150, Scene::Center());
			FontAsset{ U"TitleFont" }(U"Enterで戻る").drawAt(45, Scene::Center() + Vec2{ 0,150 });
		}

		FontAsset{ U"NormalFont" }(U"[ESC]ポーズ").draw(Vec2{ 10,5 });
	}

}
