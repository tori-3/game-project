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
	}

	const Array<String>pathList{ U"BackGroundTexture/雪原背景.png",U"BackGroundTexture/洞窟背景.png",U"BackGroundTexture/雲背景.png" ,U"BackGroundTexture/ラスボス背景.png" };

	size_t textureIndex = 0;

	Timer timer{ 10s,StartImmediately::Yes };

	CookieButton mapButton{ RectF{Arg::center(Scene::Center().x-250,600),300,80},U"マップ" };
	CookieButton minigameButton{ RectF{Arg::center(Scene::Center().x+250,600),300,80},U"ミニゲーム" };


	void update() override
	{
		if (not (timer.isRunning() && 0s < timer))
		{
			timer.restart();

			textureIndex = (textureIndex + 1) % pathList.size();
		}

		if(mapButton.update())
		{
			changeScene(U"Map");
		}

		if (minigameButton.update())
		{

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


		mapButton.draw();
		minigameButton.draw();






	}
};



