#pragma once
#include"Common.h"
#include"MiniGameSceneBase.h"

namespace ManjuRush {

	struct BlockEffect : IEffect {

		Array<Vec2>m_pos;
		Array<Vec2>m_v;

		explicit BlockEffect(const Vec2& p_pos)
			:m_pos{ 5 }, m_v{ 5 }
		{
			for (auto& pos : m_pos) {
				pos = p_pos + RandomVec2(10.0);
			}
			for (auto& v : m_v) {
				v = RandomVec2(1.0) * Random(40) * Vec2(2.5, 1);
			}
		}

		bool update(double t) override
		{
			const double e = EaseOutExpo(t);

			for (int i = 0; i < m_pos.size(); i++) {

				const Vec2 pos = m_pos[i] + m_v[i] * Vec2(1, 1.1) * t;

				RectF{ pos,8 }.draw(Palette::Mediumvioletred);
			}
			return(t < 0.5);
		}
	};

	//名前を変更してください
	class ManjuRush : public MiniGameSceneBase
	{
	public:
		const Font font{ 100 };
		Texture anna{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃんver2右.png" };
		const Texture anna2_j1{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2_j1.png" };
		const Texture anna2_j2{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2_j2.png" };
		const Texture anna2_j3{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2_j3.png" };
		const Texture anna2_d{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2右_d.png" };
		Texture manju{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう3.png" };
		Texture renga{ U"MiniGameAsset/まんじゅうラッシュ素材/レンガピンク.png" };
		Texture renga80{ U"MiniGameAsset/まんじゅうラッシュ素材/レンガピンク80.png" };

		Texture yama_s{ U"MiniGameAsset/まんじゅうラッシュ素材/山小.png" };
		Texture manju_s{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう山小.png" };
		Texture manju_m{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう山中.png" };
		Texture manju_l{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう山大.png" };
		Texture yama_m{ U"MiniGameAsset/まんじゅうラッシュ素材/山中.png" };
		Texture yama_l{ U"MiniGameAsset/まんじゅうラッシュ素材/山大.png" };
		Texture kumo{ U"MiniGameAsset/まんじゅうラッシュ素材/くも.png" };
		Texture kumokumo{ U"MiniGameAsset/まんじゅうラッシュ素材/くもくも.png" };

		Audio BGM{ U"BGM/MiniGameBGM.wav" };
		Audio Clear{ U"MiniGameAsset/Common/クリア2.wav" };
		Audio Touch{ U"MiniGameAsset/まんじゅうラッシュ素材/8bitダメージ8.mp3" };
		Audio Defeat{ U"MiniGameAsset/Common/やられた.wav" };
		Audio Score{ U"MiniGameAsset/まんじゅうラッシュ素材/スコア.wav" };

		RectF player{ 70, 500, 70,100 };
		Array<RectF>enemys;
		Stopwatch jumptime;

		double g = 15;
		double v = 0;
		double speed = 5;
		double rad1 = 0;
		double rad3 = 0;
		double x_e = 0;
		double score = 0;
		bool touch = false;
		bool jump = false;
		int level = 1;
		bool flag = false;
		bool clear = false;
		bool defeat = false;

		Stopwatch stopwatch{ StartImmediately::Yes };


		Effect effect;

		void onPauseStart()override
		{
			stopwatch.pause();
			jumptime.pause();
		}

		void onGameStart()override
		{
			stopwatch.resume();
			jumptime.resume();
		}


		ManjuRush(const InitData& init)//名前を変更してください
			: MiniGameSceneBase{ init }
		{
			enemys << RectF{ 1100,player.y + player.h - 16 - 50,80,60 };
			enemys << RectF{ 2000,player.y - 100,80,60 };
			enemys << RectF{ 1500,player.y - 350 + 5,80,60 };
			enemys << RectF{ 2500,370,80,60 };
		}

		void gameUpdate() override
		{
			if (flag == false) {
				if (stopwatch < 2s) {
					font(U"3").draw(600, 200);
				}
				else if (stopwatch < 3s) {
					font(U"2").draw(600, 200);
				}
				else if (stopwatch < 4s) {
					font(U"1").draw(600, 200);
				}
				else if (stopwatch < 5.5s) {
					font(U"スタート!").draw(400, 200);
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

					if(KeyEnter.down())
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

					if (getData().KeyUp.pressed() && jump == false)
					{
						jump = true;

						v = 22;

						jumptime.restart();

					}
					//上昇
					if (0s < jumptime && jumptime < 0.4s && 0 < v && getData().KeyUp.pressed()) {
						jump = true;
						player.y -= v;
					}
					else if (0 < v && player.y <= 500) {
						//下降
						if (getData().KeyDown.pressed()) {
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

		void gameDraw() const override
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

	};

}
