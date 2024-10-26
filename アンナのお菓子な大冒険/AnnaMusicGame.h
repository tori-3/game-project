#pragma once
# include"Common.h"

namespace AnnaMusicGame {

	std::pair<Array<OffsetCircular>, Array<OffsetCircular>> LoadNotes(const FilePath& path, double s)
	{
		TextReader reader{ path };
		double spd = s;

		if (!reader)
		{
			throw Error{ U"譜面{}が見つかりません。"_fmt(path) };
		}

		Array<OffsetCircular> rcs;
		Array<OffsetCircular> gcs;

		String line;
		while (reader.readLine(line))
		{
			if (line.isEmpty())
			{
				continue;
			}

			Array<String> x = line.split(U' ');

			if ((Parse<double>(x[1])) == 0)
			{
				rcs.emplace_back(OffsetCircular{ Point{-1450,600}, 2000, ((-1 * (Parse<double>(x[0])) * spd * 1_deg) + 90_deg - spd * 5_deg) });
			}
			else if ((Parse<double>(x[1])) == 1)
			{
				gcs.emplace_back(OffsetCircular{ Point{-1450,600}, 2000, ((-1 * (Parse<double>(x[0])) * spd * 1_deg) + 90_deg - spd * 5_deg) });
			}
		}
		return std::make_pair(rcs, gcs);
	}


	Array<String> LoadDates(const FilePath& path)
	{
		TextReader reader{ path };
		if (not reader)
		{
			throw Error{ U"データ{}が見つかりません。"_fmt(path) };
		}
		Array<String> dates;

		String line;

		while (reader.readLine(line))
		{
			if (line.isEmpty())
			{
				continue;
			}
			String x = line;

			dates.emplace_back(x);
		}
		return dates;
	}

	struct Judge : IEffect
	{
		int32 m_jud;
		int32 m_rg;
		struct Star
		{
			Vec2 start;
			Vec2 velocity;
			ColorF color;
		};



		Array<Star> m_stars;
		Array<Star> m_stars_p;
		const Font m_f{ FontMethod::MSDF,50 };

		explicit Judge(const int32& jud, const int32& rg)
			:m_jud{ jud },
			m_rg{ rg }
		{
			for (int32 i = 0; i < 6; ++i)
			{
				Star star{
						.start = OffsetCircular(Point{550,600},40,i * 60_deg),
						.velocity = Vec2{cos((i * 60) * 1_deg),sin((i * 60) * 1_deg)},

				};
				m_stars << star;
				Star star_p{
					.start = OffsetCircular(Point{550,600},45,i * 60_deg + 60_deg),
					.velocity = Vec2{cos((i * 60) * 1_deg),sin((i * 60) * 1_deg)},

				};
				m_stars_p << star_p;

			}
		}




		bool update(double t)override
		{
			if (m_jud == 0)
			{

				m_f(U"Perfect").draw(50, 650, 450, Palette::Yellow);

				for (auto& star_p : m_stars_p)
				{

					const Vec2 pos = star_p.start + star_p.velocity * t * 100;

					if (m_rg == 0)
					{
						star_p.color = ColorF(Palette::Red, 1.0 - t * 2);
					}
					else if (m_rg == 1)
					{
						star_p.color = ColorF(Palette::Yellowgreen, 1.0 - t * 2);
					}



					Shape2D::Star((30 * (1.0 - t)), pos).draw(star_p.color);
				}


			}
			else if (m_jud == 1)
			{
				m_f(U"Miss").draw(50, 650, 450, Palette::Skyblue);



			}
			else if (m_jud == 2)
			{
				for (auto& star : m_stars)
				{
					const Vec2 pos = star.start + star.velocity * t * 100;

					star.color = ColorF(Palette::White, 1.0 - t * 2);

					Shape2D::Star((30 * (1.0 - t)), pos).draw(star.color);
				}
			}

			return(t < 0.5);

		}

	};

	class SpriteSheetAnimation
	{
	private:
		Texture m_spriteSheet;
		int m_frameCountX;
		int m_frameCountY;
		int m_frameWidth;
		int m_frameHeight;
		int m_totalFrames;
		double m_frameDuration;
		double m_elapsedTime;
		bool m_loop;


	public:
		SpriteSheetAnimation(const FilePath& spriteSheetPath, int frameCountX, int frameCountY, int frameWidthX, int frameheightY, double frameDuration, bool loop = true)
			: m_spriteSheet(spriteSheetPath), m_frameCountX(frameCountX), m_frameCountY(frameCountY),
			m_frameWidth(frameWidthX), m_frameHeight(frameheightY),
			m_totalFrames(frameCountX* frameCountY), m_frameDuration(frameDuration), m_elapsedTime(0.0),
			m_loop(loop)
		{
		}

		void update(double deltaTime)
		{


			m_elapsedTime += deltaTime;

			if (!m_loop && m_elapsedTime >= m_totalFrames * m_frameDuration)
			{
				m_elapsedTime = m_totalFrames * m_frameDuration;

			}
		}
		void reset()
		{
			m_elapsedTime = 0.0;

		}

		void draw(const Vec2& pos) const
		{


			int currentFrame = static_cast<int>(m_elapsedTime / m_frameDuration);

			if (m_loop)
			{
				currentFrame %= m_totalFrames; // ループ再生の場合
			}
			else if (currentFrame >= m_totalFrames)
			{
				currentFrame = m_totalFrames - 1; // ループしない場合、最後のフレームで固定
			}

			int frameX = currentFrame % m_frameCountX;
			int frameY = currentFrame / m_frameCountX;
			Rect frameRect(frameX * m_frameWidth, frameY * m_frameHeight, m_frameWidth, m_frameHeight);




			m_spriteSheet(frameRect).draw(pos);
		}



	};




	//名前を変更してください
	class AnnaMusicGame : public App::Scene
	{
	public:
		//ここに変数などを宣言
		double s = 9.0;


		int32 combo = 0;
		int32 maxcombo = 0;

		int32 jud = 0;

		int32 perfect = 0;
		int32 miss = 0;

		Effect effect;


		Array<String> detas = LoadDates(U"AnnaMusicGame/{}_dates.txt"_fmt(getData().mini_mode));
		int32 bpm = Parse<double>(detas[0]);

		int32 st = Parse<double>(detas[1]);

		String credit = detas[2];

		std::pair<Array<OffsetCircular>, Array<OffsetCircular>> notes = LoadNotes(U"AnnaMusicGame/{}_notes.txt"_fmt(getData().mini_mode), s);
		Array<OffsetCircular>& rcs = notes.first;
		Array<OffsetCircular>& gcs = notes.second;

		const Audio music{ U"AnnaMusicGame/{}.mp3"_fmt(getData().mini_mode) ,Loop::No };

		const Font font{ FontMethod::MSDF,50 };

		const Circle C0{ 550,600,40 };
		const Circle C1{ 550, 600, 35 };
		const Circle C2{ 550,600,30 };
		const Circle C3{ OffsetCircular{ Point{-1450,600},2000,(100_deg) },130 };

		const Audio SE{ U"AnnaMusicGame/Tambourine02-01(Open).mp3" };
		Stopwatch stopwatch{ StartImmediately::Yes };

		Texture candyr{ U"AnnaMusicGame/candyr.png" };
		Texture candyg{ U"AnnaMusicGame/candyg.png" };

		int32 rg = 0;

		double frameduration = 1.0 / (bpm / 60) / 10;

		Array<SpriteSheetAnimation>anime =
		{
			SpriteSheetAnimation(U"AnnaMusicGame/anime_00.png", 10, 4, 400, 400, frameduration, true),
			SpriteSheetAnimation(U"AnnaMusicGame/anime_11.png", 10, 4, 400, 400, frameduration, true),
			SpriteSheetAnimation(U"AnnaMusicGame/anime_22.png", 5, 1, 400, 400, frameduration, true)

		};
		int32 nowanimation = 1;


		Vec2 pos = { 100,300 };


		double t = 0;
		bool change = false;


	public:

		AnnaMusicGame(const InitData& init)
			: IScene{ init }
		{

		}



		void update() override
		{

			// 経過時間を取得
			double deltaTime = Scene::DeltaTime();
			anime[nowanimation].update(deltaTime);
			anime[0].update(deltaTime);

			if (change == true and t < stopwatch.ms())
			{
				anime[nowanimation].reset();
				nowanimation = 1;
				change = false;
			}





			// アニメーションを更新
			anime[0].draw(pos);
			anime[nowanimation].draw(pos);





			Cursor::RequestStyle(CursorStyle::Hidden);
			Scene::SetBackground(Palette::Pink);
			//判定マーカー
			C0.draw(ColorF{ 0,255,0,0.0 });
			C1.draw(Palette::White);
			C2.draw(ColorF{ 0,0,0,0.0 });
			C3.draw(ColorF{ 255,0,0,0.0 });

			music.setVolume(getData().BGM_volume);

			SE.setVolume(getData().Effect_volume);
			// Enter
			if (KeyEnter.down())
			{
				EndGame(false);

			}


			if (bpm / (60 * s) * st * s < stopwatch.ms())
			{
				music.play();
			}
			if (KeyD.down() or KeyF.down() or KeyJ.down() or KeyK.down())
			{
				effect.add<Judge>(2, 2);
				t = stopwatch.ms() + 490;
				nowanimation = 2;
				pos = { 100,300 };
				anime[nowanimation].reset();
				change = true;
			}

			for (auto& rc : rcs)
			{

				rc.rotate(Scene::DeltaTime() * bpm / 60 * s * 1_deg);

				if (rc.theta > 0)//判定　90　最初0からマイナス方向に並んでいる　回転で＋になったのだけ表示、判定
				{


					candyr.drawAt(rc);

					//判定
					if (KeyF.down() or KeyJ.down())
					{



						if (C2.intersects(Circle{ rc, 30 }))
						{
							SE.playOneShot();
							perfect += 1;
							combo += 1;
							jud = 0;
							rg = 0;
							effect.add<Judge>(jud, rg);
						}
						else if (C0.intersects(Circle{ rc,30 }))
						{
							if (maxcombo < combo)
							{
								maxcombo = combo;
							}
							miss += 1;
							combo = 0;
							jud = 1;
							rg = 0;
							effect.add<Judge>(jud, rg);
						}
					}
					else if (C3.contains(Circle{ rc,30 }))
					{
						if (maxcombo < combo)
						{
							maxcombo = combo;
						}
						miss += 1;
						combo = 0;
						jud = 1;

					}


				}
			}
			for (auto& gc : gcs)
			{

				gc.rotate(Scene::DeltaTime() * bpm / 60 * s * 1_deg);

				if (gc.theta > 0)//判定　90　最初0からマイナス方向に並んでいる　回転で＋になったのだけ表示、判定
				{


					candyg.drawAt(gc);

					//判定
					if (KeyD.down() or KeyK.down())
					{


						if (C2.intersects(Circle{ gc, 30 }))
						{
							SE.playOneShot();
							perfect += 1;
							combo += 1;
							jud = 0;
							rg = 1;
							effect.add<Judge>(jud, rg);
						}
						else if (C0.intersects(Circle{ gc,30 }))
						{
							if (maxcombo < combo)
							{
								maxcombo = combo;
							}
							miss += 1;
							combo = 0;
							jud = 1;
							rg = 1;
							effect.add<Judge>(jud, rg);
						}
					}
					else if (C3.contains(Circle{ gc,30 }))
					{
						if (maxcombo < combo)
						{
							maxcombo = combo;
						}
						miss += 1;
						combo = 0;
						jud = 1;

					}


				}
			}

			//削除
			rcs.remove_if([&](const OffsetCircular rc) {return((KeyF.down() or KeyJ.down()) and C0.intersects(Circle{ rc,30 }) and rc.theta > 0); });
			rcs.remove_if([&](const OffsetCircular rc) {return(C3.contains(Circle{ rc,30 }) and rc.theta > 0); });
			gcs.remove_if([&](const OffsetCircular gc) {return((KeyD.down() or KeyK.down()) and C0.intersects(Circle{ gc,30 }) and gc.theta > 0); });
			gcs.remove_if([&](const OffsetCircular gc) {return(C3.contains(Circle{ gc,30 }) and gc.theta > 0); });

			//終了
			if (rcs.empty() and gcs.empty())
			{
				if (maxcombo < combo)
				{
					maxcombo = combo;
				}
				font(U"Perfect{}\nMiss{}\nMaxcombo{}"_fmt(perfect, miss, maxcombo)).draw(60, 50, 50, Palette::Yellow);
				font(U"Enterで戻る").draw(30, 250, 300, Palette::Skyblue);
				if (KeyEnter.down() and miss <= 9)//miss9以下でクリア
				{

					EndGame(true);
				}
				else if (KeyEnter.down())
				{
					EndGame(false);
				}



			}

			effect.update();



		}

		void draw() const override
		{
			font(U"赤はFJキー").draw(40, 700, 50, Palette::Red);
			font(U"黄緑はDKキー").draw(40, 700, 90, Palette::Yellowgreen);
			font(U"をタイミング良く押そう！！\nMiss9以下でクリア").draw(40, 700, 130, Palette::Skyblue);
			font(U"{}"_fmt(credit)).draw(30, 5, 750, Palette::Skyblue);
			font(U"Combo{}"_fmt(combo)).draw(60, 650, 380, Palette::Yellowgreen);
		}

	private:
		void EndGame(bool clear) {
			getData().mini_clear = clear;//クリア状況保存
			if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
			else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
		}

	};







}
