#pragma once

#include"Common.h"
#include"CookieButton.h"
#include"LoadAsset.h"


class TitleScene : public App::Scene
{
public:


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
	}

	const Array<String>pathList{ U"BackGroundTexture/雪原背景.png",U"BackGroundTexture/洞窟背景.png",U"BackGroundTexture/雲背景.png" ,U"BackGroundTexture/ラスボス背景.png" };

	size_t textureIndex = 0;

	Timer timer{ 10s,StartImmediately::Yes };

	CookieButton mapButton{ RectF{Arg::center(Scene::Center().x-250,600),300,80},U"マップ" };
	CookieButton minigameButton{ RectF{Arg::center(Scene::Center().x+250,600),300,80},U"ミニゲーム" };

	double time = 0;

	double translate()const
	{
		return (1 - Min(time / 2.0, 1.0)) * 300;
	}

	void update() override
	{
		time += Scene::DeltaTime();

		if (not (timer.isRunning() && 0s < timer))
		{
			timer.restart();

			textureIndex = (textureIndex + 1) % pathList.size();
		}

		{
			Transformer2D target{ Mat3x2::Translate(0,translate()),TransformCursor::Yes };

			if (mapButton.update())
			{
				changeScene(U"Map");
			}

			if (minigameButton.update())
			{

			}
		}
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
			Transformer2D target{ Mat3x2::Translate(0,translate()),TransformCursor::Yes };
			mapButton.draw();
			minigameButton.draw();
		}
	}
};
