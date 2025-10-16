#pragma once

#include"Common.h"
#include"LoadAsset.h"
#include"SweetsPanel.hpp"

class GalleryScene : public App::Scene
{
public:

	Grid<Texture>textureGrid
	{
		{
			Texture{ U"BackGroundTexture/ステージセレクト.png" },
			Texture{ U"kaiso.png" },
			TextureAsset{ U"BackGroundTexture/雪原背景.png" },
			TextureAsset{ U"BackGroundTexture/洞窟背景.png" },
		},
		{
			TextureAsset{ U"BackGroundTexture/雲背景.png" },
			TextureAsset{ U"BackGroundTexture/ラスボス背景.png" },
			TextureAsset{ U"BackGroundTexture/宇宙背景.png" },
			Texture{ U"EndRoll/ハッピーエンド.png" },
		},
	};

	Texture homeIcon{ 0xf015_icon,40 };
	static constexpr Circle backButton{ {35,35},30 };

	Point selectPoint{ 0,0 };

	UIManager manager;

	LongPressInput leftInput{ getData().minigameLeftKey,ControllerManager::Direction::Left };
	LongPressInput rightInput{ getData().minigameRightKey,ControllerManager::Direction::Right };

	CSV textCsv{ U"図鑑.txt" };

	GalleryScene(const InitData& init);

	void update() override;

	void draw() const override;
};
