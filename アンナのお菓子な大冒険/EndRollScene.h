#pragma once
#include"Common.h"
#include"CharacterSystem.h"
#include"BGMManager.hpp"

static constexpr double endRollTime = 2;


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

	Texture texture{ U"EndRoll/突撃アンナ.png" };

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

		FontAsset{ U"EndRollFont" }(U"アンナちゃんのママ兼パパ\nりゅーの").draw(50, 50);

		texture.mirrored().draw(Arg::bottomRight(Scene::Size() - Vec2{ 50,50 }));
	}
};

class EndRoll4 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/with.png" };

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

		FontAsset{ U"EndRollFont" }(U"ディレクション\nWith Ball").draw(50, 50);

		texture.mirrored().draw(Arg::bottomRight(Scene::Size()-Vec2{0,50}));
	}
};

class EndRoll5 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"EndRoll/まる.png" };

	EndRoll5(const InitData& init) : IScene{ init } {}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll6");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		FontAsset{ U"EndRollFont" }(U"レベルデザイン\nまる\nとりさん").draw(50, 50);

		texture.resized(300).draw(Arg::bottomRight(Scene::Size() - Vec2{ 50,50 }));
	}
};

class EndRoll6 :public App::Scene
{
public:
	double t = 0;

	EndRoll6(const InitData& init) : IScene{ init } {}

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
			changeScene(U"EndRoll7");
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


class EndRoll7 :public App::Scene
{
public:

	const Vec2 posAnnna{ Scene::Size() - Vec2{200,200} };
	const Vec2 posItigo{ Scene::Size() - Vec2{Scene::Width()-200,200} };

	CharacterSystem characterAnnna{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.5,posAnnna,false,true };
	mutable CharacterSystem characterItigo{ U"CharacterImages/itigo/itigo.json" ,U"CharacterImages/itigo/motion.txt" ,0.5,posItigo,true,false };


	double t = 0;

	EndRoll7(const InitData& init) : IScene{ init }
	{
		characterAnnna.addMotion(U"Summer");
		characterItigo.addMotion(U"",true);
	}

	void update()override
	{
		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll8",4s);
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

class EndRoll8 :public App::Scene
{
public:
	double t = 0;

	Texture texture{U"BackGroundTexture/雪原背景.png"};

	EndRoll8(const InitData& init) : IScene{ init }
	{
		LoadAsset::RegisterAudio(U"BGM/StageCloud_Pxtone5.2.mp3", Loop::Yes);
		BGMManager::get().play(U"BGM/StageCloud_Pxtone5.2.mp3", 2s);
	}

	void update()override
	{

		t += Scene::DeltaTime();

		if (3 < t)
		{
			changeScene(U"EndRoll9");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		texture.resized(Scene::Width()).draw();

		Scene::Rect().draw(ColorF{0,0.3});

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"BGM 効果音\nまる").draw(50, 50);
		FontAsset{ U"EndRollFont" }(U"効果音素材\n効果音ラボ\nOn - Jin ～音人～\nザ・マッチメイカァズ").draw(50, 50 + height * 3, AlphaF(Clamp(t - 1, 0.0, 1.0)));
	}
};

class EndRoll9 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"BackGroundTexture/洞窟背景.png" };

	EndRoll9(const InitData& init) : IScene{ init }
	{
	}

	void update()override
	{

		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll10");
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

class EndRoll10 :public App::Scene
{
public:
	double t = 0;

	Texture texture{ U"BackGroundTexture/雲背景.png" };

	EndRoll10(const InitData& init) : IScene{ init }
	{
	}

	void update()override
	{

		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll11");
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

class EndRoll11 :public App::Scene
{
public:
	double t = 0;

	Array<Texture>textureList{
	Texture{U"MiniGameDisplay/HawkDropOut.png"},
	Texture{U"MiniGameDisplay/まんじゅうラッシュ.png"},
	Texture{U"MiniGameDisplay/MAZE.png"},
	Texture{U"MiniGameDisplay/回転迷路.png"},
	Texture{U"MiniGameDisplay/音ゲー.png"},
	Texture{U"MiniGameDisplay/FallingAnna.png"},
	};

	EndRoll11(const InitData& init) : IScene{ init }
	{
	}

	void update()override
	{

		t += Scene::DeltaTime();

		if (endRollTime < t)
		{
			changeScene(U"EndRoll12");
		}
	}

	void draw()const override
	{
		Scene::SetBackground(Palette::Black);

		const double height = FontAsset{ U"EndRollFont" }.height();

		FontAsset{ U"EndRollFont" }(U"ミニゲーム").draw(50, 50);
		FontAsset{ U"EndRollFont" }(U"ぺってぃー").draw(50, 50 + height, AlphaF(Clamp(t, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"まる").draw(50, 50 + height * 2, AlphaF(Clamp(t - 1, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"ひー").draw(50, 50 + height * 3, AlphaF(Clamp(t - 2, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"なかこう").draw(50, 50 + height * 4, AlphaF(Clamp(t - 3, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"ゆーれい").draw(50, 50 + height * 5, AlphaF(Clamp(t - 4, 0.0, 1.0)));
		FontAsset{ U"EndRollFont" }(U"With Ball").draw(50, 50 + height * 6, AlphaF(Clamp(t - 5, 0.0, 1.0)));

		for (size_t i = 0; i < textureList.size(); ++i)
		{
			textureList[i].resized(Scene::Width()).draw(AlphaF(Clamp(t - i, 0.0, 1.0)));
		}
	}
};


class EndRollScene :public App::Scene
{
public:
	App manager;

	EndRollScene(const InitData& init)
		: IScene{ init }
	{
		FontAsset::Register(U"EndRollFont", 60, Typeface::Heavy);

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

		manager.init(U"EndRoll8");
	}

	void update()override
	{
		manager.updateScene();
	}

	void draw()const override
	{
		manager.drawScene();
	}
};






