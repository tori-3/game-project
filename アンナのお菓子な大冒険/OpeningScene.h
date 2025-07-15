#pragma once

#include"Common.h"
#include"LightBloom.h"

class OpeningScene : public App::Scene
{
public:

	Texture siv3dLogo{ U"Siv3DLogo.png" };

	Texture ctrlLogo{ U"CTRLLogo.png"};

	LightBloom light{ Scene::Size() };

	Spotlight spotLight{ Scene::Size() };

	double time = 0;

	Vec2 spotLightLeft{ -400,0 };
	Vec2 spotLightRight{ Scene::Width() + 400,0 };

	Vec2 spotLightLeftVel{};
	Vec2 spotLightRightVel{};

	SimpleAnimation animation;

	OpeningScene(const InitData& init)
		: IScene{ init }
	{
		Duration firstTime = 2.2s;
		Duration secondTime = 2.4s;

		animation
			.set(U"light", { 1s,0 }, { 2s, 0.7 })
			.set(U"light", { firstTime,1 }, { firstTime + 0.05s, 0.5 })
			.set(U"light", { firstTime + 0.1s, 0.5 }, { firstTime + 0.15s, 1 })
			.set(U"light", { secondTime,1 }, { secondTime + 0.05s, 0.5 })
			.set(U"light", { secondTime + 0.1s, 0.5 }, { secondTime + 0.15s, 1 });
	}

	void update()override
	{
		time += Scene::DeltaTime();

		spotLightLeft=Math::SmoothDamp(spotLightLeft, Scene::Center()+Vec2{-20,10}, spotLightLeftVel, 0.5);
		spotLightRight = Math::SmoothDamp(spotLightRight, Scene::Center() + Vec2{20,-10 }, spotLightRightVel, 0.5);

		if (3.5 <= time)
		{
			animation.start();
		}

		if(7.5<time)
		{
			changeScene(U"TitleScene",2.0s);
		}
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.0 });

		if(time<3.5)
		{
			Scene::Rect().draw(ColorF{ 0.2 });

			ctrlLogo.resized(Scene::Width() * 0.4).drawAt(Scene::Center() + Vec2{ 0,-30 });
			{
				ScopedSpotlight target{ spotLight,ColorF{0} };

				Circle{ spotLightLeft ,250 }.drawShadow(Vec2{ 0,0 }, 20, 3, ColorF{ 0.7 * Clamp((2.5 - time)+1, 0.0, 1.0) });
				Circle{ spotLightRight ,250 }.drawShadow(Vec2{ 0,0 }, 20, 3, ColorF{ 0.7 * Clamp((2.5 - time)+1, 0.0, 1.0) });
			}

			spotLight.draw();
		}
		else
		{
			const double alpha = animation[U"light"];

			siv3dLogo.resized(Scene::Width() * 0.7).drawAt(Scene::Center(), AlphaF(alpha));

			{
				ScopedLightBloom target{ light };
				siv3dLogo.resized(Scene::Width() * 0.7).drawAt(Scene::Center(), ColorF(1, alpha));
			}
			light.draw();

		}
	}
};
