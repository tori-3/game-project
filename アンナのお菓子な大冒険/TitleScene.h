#pragma once

#include"Common.h"
#include"CookieButton.h"
#include"LoadAsset.h"


class TitleScene : public App::Scene
{
public:

	Texture stickTexture{ U"SignboardStick.png" };

	bool updateStick(const Vec2& pos)
	{
		constexpr SizeF chocolateSize{ 300,50 };
		const RoundRect rrect{ pos,chocolateSize,chocolateSize.y / 2.0 };

		if (rrect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		return rrect.leftClicked();
	}

	void drawStick(const Vec2& pos, StringView text)const
	{
		constexpr SizeF chocolateSize{ 300,50 };
		const RoundRect rrect{ pos,chocolateSize,chocolateSize.y / 2.0 };
		constexpr double textureWidth=200;
		stickTexture.resized(chocolateSize.y*0.7, textureWidth).rotated(90_deg).drawAt(rrect.rightCenter() + Vec2{ textureWidth / 2.0- rrect.r,0 });
		rrect.draw(Arg::top = Color{ 129,74,42 }, Arg::bottom = Color{ 92,43,20 });
		FontAsset{ U"TitleFont" }(text).drawAt(chocolateSize.y * 0.7, rrect.center());
	}


	const Array<String>pathList{ U"BackGroundTexture/雪原背景.png",U"BackGroundTexture/洞窟背景.png",U"BackGroundTexture/雲背景.png" ,U"BackGroundTexture/ラスボス背景.png" };

	size_t textureIndex = 0;

	Timer timer{ 10s,StartImmediately::Yes };

	Array<String>menuList
	{
		U"マップ",
		U"ミニゲーム",
		U"設定",
		U"ライセンス",
		U"終了",
	};

	size_t selectedIndex = 0;

	LongPressInput upInput{ getData().KeyUp };
	LongPressInput downInput{ getData().KeyDown };

	static constexpr Vec2 targetPlace{ 400,630 };
	static constexpr Vec2 startPlace{ -100,630 };
	Vec2 playerPos = startPlace;

	CharacterSystem character{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.5,startPlace,false};

	Timer changeMapTimer{ 2s };
	Timer changeMiniGameTimer{ 2s };

	static constexpr Vec2 menuPos{ 800,450 };
	static constexpr double menuHeight = 60;

	//CookieButton mapButton{ RectF{Arg::center(Scene::Center().x-250,600),300,80},U"マップ" };
	//CookieButton minigameButton{ RectF{Arg::center(Scene::Center().x+250,600),300,80},U"ミニゲーム" };

	double time = 0;

	bool playerWalkStop = false;

	bool menuClicked = false;

	double translate()const
	{
		return (1 - Min(time / 2.0, 1.0)) * 500;
	}

	TitleScene(const InitData& init)
		: IScene{ init }
	{

		LoadAsset::RegisterTexture(U"BackGroundTexture/雪原背景.png");
		LoadAsset::RegisterTexture(U"BackGroundTexture/雲背景.png");
		LoadAsset::RegisterTexture(U"BackGroundTexture/ラスボス背景.png");
		LoadAsset::RegisterTexture(U"BackGroundTexture/洞窟背景.png");

		LoadAsset::RegisterAudio(U"BGM/StageCloud_Pxtone5.2.mp3", Loop::Yes);
		BGMManager::get().play(U"BGM/StageCloud_Pxtone5.2.mp3");

		LoadAsset::RegisterTexture(U"ロゴ.png");
		character.addMotion(U"Walk", true);
	}

	void update() override
	{
		time += Scene::DeltaTime();

		if (not (timer.isRunning() && 0s < timer))
		{
			timer.restart();

			textureIndex = (textureIndex + 1) % pathList.size();
		}

		if (not playerWalkStop)
		{
			playerPos = startPlace.lerp(targetPlace, Min(time / 3.0, 1.0));

			if (playerPos == targetPlace)
			{
				playerWalkStop = true;
				character.removeMotion(U"Walk");
				character.addMotion(U"Standing");
			}
		}

		if (not menuClicked)
		{
			if (upInput.down() && 0 < selectedIndex)
			{
				--selectedIndex;
			}

			if (downInput.down() && selectedIndex < menuList.size() - 1)
			{
				++selectedIndex;
			}

			if(KeyEnter.down())
			{
				menuClicked = true;
			}

			{
				Transformer2D target{ Mat3x2::Translate(translate(),0),TransformCursor::Yes };
				Vec2 pos = menuPos;

				for (size_t i = 0; i < menuList.size(); ++i)
				{
					if (updateStick(pos + (selectedIndex == i ? Vec2{ -30,0 } : Vec2{})))
					{
						selectedIndex = i;
						menuClicked = true;
					}
					pos.y += menuHeight;
				}
			}

			if (menuClicked)
			{
				switch (selectedIndex)
				{
				case 0:
					playerWalkStop = true;
					character.removeMotion(U"Walk");
					character.addMotion(U"Tosshin", true);
					changeMapTimer.restart();
					break;
				case 1:
					playerWalkStop = true;
					character.removeMotion(U"Walk");
					character.addMotion(U"HeadDrop");
					changeMiniGameTimer.restart();
					break;
				case 2:
					break;
				case 3:
					LicenseManager::ShowInBrowser();
					break;
				case 4:
					System::Exit();
					break;
				default:
					break;
				}
			}
		}

		if(changeMapTimer.isStarted())
		{
			playerPos.x += Scene::DeltaTime() * 500;
		}

		if(changeMapTimer.reachedZero())
		{
			changeScene(U"Map");
		}

		if (changeMiniGameTimer.isStarted())
		{
			if (0.7s<changeMiniGameTimer.duration() - Duration{ changeMiniGameTimer.sF() })
			{
				playerPos.y += Scene::DeltaTime() * 500;
			}
		}

		if (changeMiniGameTimer.reachedZero())
		{
			changeScene(U"MiniGameSelect");
		}


		character.update(playerPos, false);
	}

	void draw() const override
	{

		constexpr double changeTime = 3.0;
		
		if(timer.sF()<=changeTime)
		{
			double t = timer.sF() / changeTime;

			TextureAsset{ pathList[(textureIndex + 1) % pathList.size()] }.resized(Scene::Size()).draw(ColorF{ 1,1 - t });
			TextureAsset{ pathList[textureIndex] }.resized(Scene::Size()).draw(ColorF{ 1,t });
		}
		else
		{
			TextureAsset{ pathList[textureIndex] }.resized(Scene::Size()).draw();
		}


		const Vec2 rogoPos{ Scene::Center() + Vec2{ 0,-150 } };

		TextureAsset{ U"ロゴ.png" }.scaled(1.3).drawAt(rogoPos,ColorF{ 1,Min(time / 2.0,1.0) });

		{
			Transformer2D target{ Mat3x2::Translate(translate(),0),TransformCursor::Yes };


			Vec2 pos = menuPos;

			for(size_t i=0;i< menuList.size();++i)
			{
				drawStick(pos + (selectedIndex==i?Vec2{-30,0}:Vec2{}), menuList[i]);
				pos.y += menuHeight;
			}


			//mapButton.draw();
			//minigameButton.draw();
		}

		character.draw();
	}
};
