#include"AnnaMusicGame.h"
#include"KeyInfo.h"

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

	Judge::Judge(const int32& jud, const int32& rg)
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

	bool Judge::update(double t)
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


	void SpriteSheetAnimation::draw(const Vec2& pos) const
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


	AnnaMusicGame::AnnaMusicGame(const InitData& init)
		: MiniGameSceneBase{ init }
	{
		if (not TextureAsset::IsRegistered(U"BackGroundTexture/宇宙背景.png"))
		{
			TextureAsset::Register(U"BackGroundTexture/宇宙背景.png", U"BackGroundTexture/宇宙背景.png");
		}

		//const RenderTexture internalTexture{ Scene::Size() };
		//const RenderTexture internalTexture2{ Scene::Size() };
		//Shader::GaussianBlur(TextureAsset{ U"BackGroundTexture/宇宙背景.png" }, internalTexture, internalTexture2);
		//Shader::GaussianBlur(internalTexture2, internalTexture, background);


		//const RenderTexture downsample2{ Scene::Size() / 2 };
		//const RenderTexture internalTexture2{ Scene::Size() / 2 };
		//Shader::Downsample(TextureAsset{ U"BackGroundTexture/宇宙背景.png" }, downsample2);
		//Shader::GaussianBlur(downsample2, internalTexture2, background);


		const RenderTexture downsample4{ Scene::Size() / 4 };
		const RenderTexture internalTexture4{ Scene::Size() / 4 };
		Shader::Downsample(TextureAsset{ U"BackGroundTexture/宇宙背景.png" }, downsample4);
		Shader::GaussianBlur(downsample4, internalTexture4, background);
	}

	void AnnaMusicGame::gameUpdate()
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

		if (bpm / (60 * s) * st * s < stopwatch.ms())
		{
			music.play(BGMMixBus);
		}
		if (getData().minigameLeftKey.down() or getData().minigameRightKey.down())
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
				//判定
				if (getData().minigameLeftKey.down())
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


				//candyg.drawAt(gc);

				Line{ gc + Vec2{-20,0},gc + Vec2{20,0} }.drawArrow(10, SizeF{ 15,15 });

				//判定
				if (getData().minigameRightKey.down())
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
		rcs.remove_if([&](const OffsetCircular rc) {return((getData().minigameLeftKey.down()) and C0.intersects(Circle{rc,30}) and rc.theta > 0); });
		rcs.remove_if([&](const OffsetCircular rc) {return(C3.contains(Circle{ rc,30 }) and rc.theta > 0); });
		gcs.remove_if([&](const OffsetCircular gc) {return((getData().minigameRightKey.down()) and C0.intersects(Circle{ gc,30 }) and gc.theta > 0); });
		gcs.remove_if([&](const OffsetCircular gc) {return(C3.contains(Circle{ gc,30 }) and gc.theta > 0); });

		//終了
		if (rcs.empty() and gcs.empty())
		{
			if (not saveFlg && miss <= 9)
			{
				getData().saveMiniGameClear();
				saveFlg = true;
			}

			if (maxcombo < combo)
			{
				maxcombo = combo;
			}
			if (getData().menuDecisionKey.down() and miss <= 9)//miss9以下でクリア
			{

				EndGame(true);
			}
			else if (getData().menuDecisionKey.down())
			{
				EndGame(false);
			}
		}
	}

	void AnnaMusicGame::gameDraw() const
	{
		Scene::SetBackground(Palette::Pink);

		background.resized(Scene::Size()).draw();
		Scene::Rect().draw(ColorF{ 0,0.3 });

		// アニメーションを更新
		anime[0].draw(pos);
		anime[nowanimation].draw(pos);

		//判定マーカー
		C0.draw(ColorF{ 0,255,0,0.0 });
		C1.draw(Palette::White);
		C2.draw(ColorF{ 0,0,0,0.0 });
		C3.draw(ColorF{ 255,0,0,0.0 });


		for (auto& rc : rcs)
		{
			if (rc.theta > 0)//判定　90　最初0からマイナス方向に並んでいる　回転で＋になったのだけ表示、判定
			{
				candyr.drawAt(rc);

				Line{ rc + Vec2{20,0},rc + Vec2{-20,0} }.drawArrow(10, SizeF{ 15,15 });
			}
		}
		for (auto& gc : gcs)
		{
			if (gc.theta > 0)//判定　90　最初0からマイナス方向に並んでいる　回転で＋になったのだけ表示、判定
			{
				candyg.drawAt(gc);

				Line{ gc + Vec2{-20,0},gc + Vec2{20,0} }.drawArrow(10, SizeF{ 15,15 });
			}
		}

		if (rcs.empty() and gcs.empty())
		{
			FontAsset(U"NormalFont")(U"Perfect{}\nMiss{}\nMaxcombo{}"_fmt(perfect, miss, maxcombo)).draw(60, 50, 50, Palette::Yellow);
			FontAsset(U"NormalFont")(U"{}で戻る"_fmt(ToKeyName(getData().menuDecisionKey))).draw(30, 250, 300, Palette::Skyblue);

			if (miss <= 9)
			{
				FontAsset(U"NormalFont")(U"クリア！").draw(100, Arg::bottomRight = Scene::Size() - Vec2{ 50,50 }, Palette::Yellow);
			}
			else
			{
				FontAsset(U"NormalFont")(U"失敗、、、").draw(100, Arg::bottomRight = Scene::Size() - Vec2{ 50,50 }, Palette::Red);
			}


		}

		effect.update();

		FontAsset(U"NormalFont")(U"赤(←)は[{}]"_fmt(ToKeyName(getData().minigameLeftKey))).draw(40, 700, 50, Palette::Red);
		FontAsset(U"NormalFont")(U"黄緑(→)は[{}]"_fmt(ToKeyName(getData().minigameRightKey))).draw(40, 700, 90, Palette::Yellowgreen);
		FontAsset(U"NormalFont")(U"をタイミング良く押そう！！\nMiss9以下でクリア").draw(40, 700, 130, Palette::Skyblue);
		FontAsset(U"NormalFont")(U"{}"_fmt(credit)).draw(30, 5, 750, Palette::Skyblue);
		FontAsset(U"NormalFont")(U"Combo{}"_fmt(combo)).draw(60, 650, 380, Palette::Yellowgreen);

		FontAsset{ U"NormalFont" }(U"[{}]ポーズ"_fmt(ToKeyName(getData().pauseKey))).draw(Arg::topRight = Vec2{ Scene::Width() - 10,5 });
	}

}
