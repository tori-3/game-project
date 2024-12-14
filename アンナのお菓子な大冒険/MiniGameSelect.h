#pragma once
#include"Common.h"

class MiniGameSelect : public App::Scene
{
public:

	struct MiniGameData
	{
		Texture pictures;
		String sentence;
		int32 stageNum;
	};

	static constexpr Circle backButton{ {35,35},30 };

	Texture homeIcon{ 0xf015_icon,40 };

	Texture background{ U"BackGroundTexture/ステージセレクト.png" };

	Array<MiniGameData>miniGameList;

	MiniGameSelect(const InitData& init)
		: IScene{ init }
	{
		JSON json = JSON::Load(U"map.json");

		for(const auto& object: json[U"MiniGameStage"].arrayView())
		{
			const int32 stageNum = object.get<int32>();

			MiniGameData data;

			JSON miniGame=json[U"StageData"][U"Stage{}"_fmt(stageNum)];

			data.pictures = Texture{ miniGame[U"Picture"].getString() };
			data.sentence = miniGame[U"Sentence"].getString();
			data.stageNum = stageNum;

			miniGameList << data;
		}

		AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
		BGMManager::get().play(U"WorldBGM");
	}

	void update()override
	{
		if (backButton.leftClicked())
		{
			changeScene(U"TitleScene");
		}

		if (backButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}




	}

	void draw()const override
	{
		background.resized(Scene::Width()).draw();


		for(int32 i=0;i<miniGameList.size();++i)
		{
			//int32 x = Scene::Width() / 4 * (i % 3 + 1);
			//int32 y = Scene::Height() / 3 * (i / 3 + 1);

			int32 x = Scene::Width() / 7 * (i+1);
			int32 y = 150 + i % 2 * 200;

			miniGameList[i].pictures.resized(220).drawAt(x,y);



		}




		homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);


	}


};
