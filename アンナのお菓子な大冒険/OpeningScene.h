#pragma once

#include"Common.h"
#include"LightBloom.h"
#include"Spotlight.hpp"

class OpeningScene : public App::Scene
{
public:

	Texture siv3dLogoFrame{ U"Siv3DLogoFrame.png" };
	Texture siv3dLogoFrameBold{ U"Siv3DLogoFrameBold.png" };

	Texture ctrlLogo{ U"CTRLLogo.png" };

	LightBloom light{ Scene::Size() };

	Spotlight spotLight{ Scene::Size() };

	double time = 0;

	Vec2 spotLightLeft{ -400,0 };
	Vec2 spotLightRight{ Scene::Width() + 400,0 };

	Vec2 spotLightLeftVel{};
	Vec2 spotLightRightVel{};

	SimpleAnimation animation;

	OpeningScene(const InitData& init);

	~OpeningScene();

	void update()override;

	void draw() const override;

};
