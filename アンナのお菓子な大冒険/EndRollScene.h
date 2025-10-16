#pragma once
#include"Common.h"
#include"CharacterSystem.h"
#include"BGMManager.hpp"
#include"SweetsPanel.hpp"

static constexpr double endRollTime = 2;
static constexpr double endRollTimeLong = 3;

class EndRoll1 :public App::Scene
{
public:
	double t = 0;

	EndRoll1(const InitData& init)
		: IScene{ init }
	{
		LoadAsset::RegisterAudio(U"BGM/WorldMap2.mp3", Loop::Yes);
		BGMManager::get().play(U"BGM/WorldMap2.mp3");
	}

	void update()override
	{
		t += Scene::DeltaTime();

		if(endRollTime <t)
		{
			changeScene(U"EndRoll2");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		FontAsset{ U"EndRollFont" }(U"プロデュース\nとりさん").draw(50, 50);

	}
};

class EndRoll2 :public App::Scene
{
public:
	double t = 0;

	Texture texture{U"EndRoll/やっさん.png"};

	EndRoll2(const InitData& init): IScene{ init }{}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll3");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		FontAsset{ U"EndRollFont" }(U"企画\nやっさん").draw(50, 50);

		texture.mirrored().draw(Arg::bottomRight(Scene::Size() - Vec2{50,50}));
	}
};

class EndRoll3 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/with.png" };

	EndRoll3(const InitData& init) : IScene{ init } {}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll4");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		FontAsset{ U"EndRollFont" }(U"ディレクション\nWith Ball").draw(50, 50);

		texture.mirrored().draw(Arg::bottomRight(Scene::Size()-Vec2{0,50}));
	}
};

class EndRoll4 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/まる.png" };

	EndRoll4(const InitData& init) : IScene{ init } {}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll5");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		FontAsset{ U"EndRollFont" }(U"レベルデザイン\nまる\nとりさん").draw(50, 50);

		texture.resized(300).draw(Arg::bottomRight(Scene::Size() - Vec2{ 50,50 }));
	}
};

class EndRoll5 :public App::Scene
{
public:
	double t = 0;

	EndRoll5(const InitData& init) : IScene{ init } {}

	static constexpr double time = 6;

	Array<Texture>textureList{
		Texture{U"EndRoll/いちご.png"},
		Texture{U"EndRoll/ポップコーン.png"},
		Texture{U"EndRoll/ゆきだるナイト.png"},
		Texture{U"EndRoll/船長.png"},
		Texture{U"EndRoll/ビター.png"},
	};

	Array<double>sizeList{
		100,80,250,200,200
	};

	void update()override
	{
		t += Scene::DeltaTime();

		if (time < t)
		{
			changeScene(U"EndRoll6");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"キャラクターデザイン").draw(50, 50);
		FontAsset{ U"EndRollFont" }(U"まる").draw(50, 50 + height, AlphaF(Clamp(t, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"たわし").draw(50, 50 + height * 2, AlphaF(Clamp(t - 1, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"With Ball").draw(50, 50 + height * 3, AlphaF(Clamp(t - 2, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"やっさん").draw(50, 50 + height * 4, AlphaF(Clamp(t - 3, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"Sco Ttishfold").draw(50, 50 + height * 5, AlphaF(Clamp(t - 4, 0.0, 1.0)));

		for(size_t i=0;i<textureList.size();++i)
		{
			const double width = Scene::Width() / textureList.size();
			textureList[i].resized(sizeList[i]).drawAt(width * (i+0.5), 650, AlphaF(Clamp(t - i, 0.0, 1.0)));
		}
	}
};


class EndRoll6 :public App::Scene
{
public:

	const Vec2 posAnnna{ Scene::Size() - Vec2{200,200} };
	const Vec2 posItigo{ Scene::Size() - Vec2{Scene::Width()-200,200} };

	CharacterSystem characterAnnna{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.5,posAnnna,false,true };
	mutable CharacterSystem characterItigo{ U"Characters/itigo/itigo.json" ,U"Characters/itigo/motion.txt" ,0.5,posItigo,true,false };


	double t = 0;

	EndRoll6(const InitData& init) : IScene{ init }
	{
		characterAnnna.addMotion(U"Summer");
		characterItigo.addMotion(U"",true);
	}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTimeLong < t)
		{
			changeScene(U"EndRoll7",4s);
			BGMManager::get().stop(2s);
		}

		characterAnnna.update(posAnnna, false);
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		FontAsset{ U"EndRollFont" }(U"モーション\nやっさん\nまる").draw(50, 50);

		characterItigo.update(posItigo, false);

		characterItigo.draw();
		characterAnnna.draw();
	}
};

class EndRoll7 :public App::Scene
{
public:
	double t = 0;

	Texture texture{U"EndRoll/雪原.png"};

	EndRoll7(const InitData& init) : IScene{ init }
	{
		LoadAsset::RegisterAudio(U"BGM/StageCloud_Pxtone5.2.mp3", Loop::Yes);
		BGMManager::get().play(U"BGM/StageCloud_Pxtone5.2.mp3", 2s);
	}

	void update()override
	{

		t += Scene::DeltaTime();

		if (endRollTimeLong+2 < t)
		{
			changeScene(U"EndRoll8");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();

		Scene::Rect().draw(ColorF{0,0.3});

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"BGM・効果音\nまる").draw(50, 50);
		FontAsset{ U"EndRollFont" }(U"効果音素材 BGM").draw(50, 50 + height * 3.5, AlphaF(Clamp(t - 1, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"効果音ラボ\nOn - Jin ～音人～\nザ・マッチメイカァズ\nOtoLogic（ミニゲーム-暗闇迷路 BGM）\n夢にみた緑 (ミニゲーム-FallingAnna BGM)").draw(40,50, 50 + height * 4.5, AlphaF(Clamp(t - 1, 0.0, 1.0)));
	}
};

class EndRoll8 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/洞窟.png" };

	EndRoll8(const InitData& init) : IScene{ init }{}

	void update()override
	{

		t += Scene::DeltaTime();

		if (endRollTimeLong < t)
		{
			changeScene(U"EndRoll9");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();

		Scene::Rect().draw(ColorF{ 0,0.3 });

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"イラスト素材\nやっさん\nまる\nたわし\nSco Ttishfold").draw(50, 50);
	}
};

class EndRoll9 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/雲.png" };

	EndRoll9(const InitData& init) : IScene{ init }{}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTimeLong < t)
		{
			changeScene(U"EndRoll10", 4s);
			BGMManager::get().stop(2s);
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();

		Scene::Rect().draw(ColorF{ 0,0.3 });

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"プログラマー\nとりさん\nやっさん").draw(50, 50);
	}
};

class EndRoll10 :public App::Scene
{
public:
	double t = 0;

	Array<Texture>textureList
	{
	Texture{U"MiniGameDisplay/まんじゅうラッシュ.png"},
	Texture{U"MiniGameDisplay/MAZE.png"},
	Texture{U"MiniGameDisplay/回転迷路.png"},
	Texture{U"MiniGameDisplay/音ゲー.png"},
	Texture{U"MiniGameDisplay/HawkDropOut.png"},
	Texture{U"MiniGameDisplay/FallingAnna.png"},
	};

	EndRoll10(const InitData& init) : IScene{ init }
	{
		LoadAsset::RegisterAudio(U"BGM/MiniGameBGM.wav", Loop::Yes);
		BGMManager::get().play(U"BGM/MiniGameBGM.wav",2s);
	}

	void update()override
	{

		t += Scene::DeltaTime()/2.0;

		if (7*1.5 < t)
		{
			changeScene(U"EndRoll11", 4s);
			BGMManager::get().stop(2s);

		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		const double height = FontAsset{ U"EndRollFont" }.height();

		for (size_t i = 0; i < textureList.size(); ++i)
		{
			textureList[i].resized(Scene::Width()).draw(AlphaF(Clamp(t - i*1.5, 0.0, 1.0)));
		}

		Scene::Rect().draw(ColorF{ 0,0.4 });

		FontAsset{ U"EndRollFont" }(U"ミニゲーム").draw(50, 50);
		FontAsset{ U"EndRollFont" }(U"まる").draw(50, 50 + height, AlphaF(Clamp(t, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"ひー").draw(50, 50 + height * 2, AlphaF(Clamp(t - 1*1.5, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"なかこう").draw(50, 50 + height * 3, AlphaF(Clamp(t - 2 * 1.5, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"ゆーれい").draw(50, 50 + height * 4, AlphaF(Clamp(t - 3 * 1.5, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"ぺってぃー").draw(50, 50 + height * 5, AlphaF(Clamp(t - 4 * 1.5, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"With Ball").draw(50, 50 + height * 6, AlphaF(Clamp(t - 5 * 1.5, 0.0, 1.0)));
	}
};

class EndRoll11 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/雪原ボス.png" };

	EndRoll11(const InitData& init) : IScene{ init }
	{
		LoadAsset::RegisterAudio(U"BGM/StageBoss6.wav", Loop::Yes);
		BGMManager::get().play(U"BGM/StageBoss6.wav", 2s);
	}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTimeLong < t)
		{
			changeScene(U"EndRoll12");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();

		Scene::Rect().draw(ColorF{ 0,0.3 });

		FontAsset{ U"EndRollFont" }(U"アンナちゃんのママ兼パパ\nりゅーの").draw(50, 50);
	}
};

class EndRoll12 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/洞窟ボス.png" };

	EndRoll12(const InitData& init) : IScene{ init }
	{
		LoadAsset::RegisterAudio(U"BGM/StageBoss6.wav", Loop::Yes);
		BGMManager::get().play(U"BGM/StageBoss6.wav", 2s);
	}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTimeLong < t)
		{
			changeScene(U"EndRoll13");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();

		Scene::Rect().draw(ColorF{ 0,0.3 });

		FontAsset{ U"EndRollFont" }(U"開発環境\nSiv3D/C++\nVisual Studio").draw(50, 50);
	}
};

class EndRoll13 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/雲ボス.png" };

	EndRoll13(const InitData& init) : IScene{ init } {}

	void update()override
	{
		t += Scene::DeltaTime();

		if (5 < t)
		{
			changeScene(U"EndRoll14");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();
		Scene::Rect().draw(ColorF{ 0,0.3 });

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"東京都市大学").drawAt(Scene::Center()+Vec2{ 0,-height/2.0 });
		FontAsset{ U"EndRollFont" }(U"コンピュータ技術研究会").drawAt(Scene::Center() + Vec2{ 0,height / 2.0 });
	}
};

class EndRoll14 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/ラスボス.png" };

	EndRoll14(const InitData& init) : IScene{ init } {}

	void update()override
	{
		t += Scene::DeltaTime();

		if (10 < t)
		{
			changeScene(U"EndRoll15",3s);
			BGMManager::get().stop(2s);
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);
		
		const double centerX = Scene::Center().x;
		const double height = FontAsset{ U"EndRollFont" }.height();

		texture.resized(Scene::Width()).draw();
		Scene::Rect().draw(ColorF{ 0,0.3 });

		FontAsset{ U"EndRollFont" }(U"スペシャルサンクス").drawAt(centerX, 250, AlphaF(Clamp(t, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"CTRL部員").drawAt(centerX, 250+height, AlphaF(Clamp(t-1*1.5, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"学園祭で遊んでくれた方々").drawAt(centerX, 250+height*2, AlphaF(Clamp(t-2*1.5, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"and You").drawAt(120,centerX, 250+height*3.5, AlphaF(Clamp((t-4*1.5)/3.0, 0.0, 1.0)));
	}
};

class EndRoll15 :public App::Scene
{
public:
	TalkWindow talk;

	double t = 0;

	bool next = false;

	EndRoll15(const InitData& init) : IScene{ init }
	{
		talk.setTalk
		({
			{ U"",U"懲らしめられた魔女は改心し、みんなのためにお菓子を作り、ふるまうようになった。"},
			{ U"",U"アンナちゃんの勇気と食欲によって、お菓子の国に平穏が訪れたのだった。" }
		});
	}

	void update()override
	{
		t += Scene::DeltaTime();
		talk.update(false,false);

		if (not next&&4 < t)
		{
			talk.next();
			next = true;
		}

		if (8 < t)
		{
			changeScene(U"EndRoll16", 6s);
		}
	}

	void draw()const override
	{
		constexpr double height = 200;
		constexpr double space = 50;
		talk.draw(RectF{ space,Scene::Height() - height - space,Scene::Width() - space * 2 ,height },{},true);
	}

	//void drawFadeOut(double t) const override
	//{
	//	Rect{ Scene::Size() }.draw(Palette::Black);
	//	draw();
	//}
};

class EndRoll16 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/ハッピーエンド.png" };

	EndRoll16(const InitData& init) : IScene{ init }{}

	void update()override
	{
		t += Scene::DeltaTime();

		if (10 < t)
		{
			changeScene(U"EndRoll17",2s);			
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();
		FontAsset{ U"EndRollFont" }(U"End").draw(TextStyle::Outline(0.2, ColorF{0,Clamp(t - 3, 0.0, 1.0) }), 200, Arg::bottomRight(Scene::Size() - Vec2{30,0}), AlphaF(Clamp(t - 3, 0.0, 1.0)));
	}

	void drawFadeIn(double t) const override
	{
		ScopedColorMul2D mul{ AlphaF(t) };
		Rect{ Scene::Size() }.draw(Palette::Black);
		draw();
	}
};

class EndRoll17 :public App::Scene
{
public:
	EndRoll17(const InitData& init) : IScene{ init } {}

	void update()override
	{
		notifyError();
	}
};

class EndRollScene :public App::Scene
{
public:
	App manager;

	UIManager uiManager;

	EndRollScene(const InitData& init)
		: IScene{ init }
	{
		FontAsset::Register(U"EndRollFont", FontMethod::MSDF, 60, Typeface::Heavy);

		manager.add<EndRoll1>(U"EndRoll1");
		manager.add<EndRoll2>(U"EndRoll2");
		manager.add<EndRoll3>(U"EndRoll3");
		manager.add<EndRoll4>(U"EndRoll4");
		manager.add<EndRoll5>(U"EndRoll5");
		manager.add<EndRoll6>(U"EndRoll6");
		manager.add<EndRoll7>(U"EndRoll7");
		manager.add<EndRoll8>(U"EndRoll8");
		manager.add<EndRoll9>(U"EndRoll9");
		manager.add<EndRoll10>(U"EndRoll10");
		manager.add<EndRoll11>(U"EndRoll11");
		manager.add<EndRoll12>(U"EndRoll12");
		manager.add<EndRoll13>(U"EndRoll13");
		manager.add<EndRoll14>(U"EndRoll14");
		manager.add<EndRoll15>(U"EndRoll15");
		manager.add<EndRoll16>(U"EndRoll16");
		manager.add<EndRoll17>(U"EndRoll17");
	}

	void update()override
	{
		uiManager.update();

		if(0<uiManager.getChildren().size())
		{
			return;
		}

		if(getData().pauseKey.down()||getData().menuBackKey.down())
		{
			AudioAsset{ U"決定ボタン" }.playOneShot();

			auto skipButton = CreateChocolateButton(0xF04AC_icon, U"スキップ", Palette::Darkred);
			auto continueButton = CreateChocolateButton(0xF040A_icon, U"続きを見る", Palette::Chocolate);
			continueButton->selected = true;

			uiManager.addChild
			({
				SimpleDialog::Create
				({
					.erasable=false,
					.child = SweetsPanel::Create
					({
						.margine = 15,
						.child = Column::Create
						({
							.children
							{
								TextUI::Create({.text = U"ポーズ",.fontSize=40,.color = Palette::White,}),
								TextUI::Create({.text = U"エンドロールをスキップしますか？",.color = Palette::White,}),
								Row::Create
								({
									.children
									{
										skipButton,
										continueButton
									}
								})
							}
						}),
					}),
					.updateFunc = [=](SimpleDialog* dialog)
					{
						if (getData().minigameLeftKey.down())
						{
							if (continueButton->selected)
							{
								continueButton->selected = false;
								skipButton->selected = true;
								AudioAsset{ U"カーソル移動" }.playOneShot();
							}
							else
							{
								AudioAsset{ U"ビープ音" }.playOneShot();
							}
						}

						if (getData().minigameRightKey.down())
						{
							if (skipButton->selected)
							{
								continueButton->selected = true;
								skipButton->selected = false;
								AudioAsset{ U"カーソル移動" }.playOneShot();
							}
							else
							{
								AudioAsset{ U"ビープ音" }.playOneShot();
							}
						}

						if (getData().menuBackKey.down() || continueButton->pressed() || (continueButton->selected&&getData().menuDecisionKey.down()))
						{
							dialog->close();
							AudioAsset{ U"キャンセル" }.playOneShot();
						}

						if (skipButton->pressed() || (skipButton->selected && getData().menuDecisionKey.down()))
						{
							dialog->close();
							AudioAsset{ U"決定ボタン" }.playOneShot();
							changeScene(U"TitleScene");
						}
					}
				})
			});
		}


		if(not manager.updateScene())
		{
			changeScene(U"TitleScene",2s);
		}
	}

	void draw()const override
	{
		if (uiManager.getChildren().isEmpty())
		{
			Cursor::RequestStyle(CursorStyle::Hidden);
		}

		Scene::SetBackground(Palette::Black);

		manager.drawScene();

		uiManager.draw();
	}
};
