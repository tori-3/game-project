#include"OpeningScene.h"
#include"ControllerInput.h"
#include"LoadAsset.h"

OpeningScene::OpeningScene(const InitData& init)
	: IScene{ init }
{
	Duration firstTime = 3.2s;
	Duration secondTime = 3.4s;

	animation
		.set(U"light", { 1s,0 }, { 3s, 0.3 })
		.set(U"light", { firstTime,1 }, { firstTime + 0.05s, 0.5 })
		.set(U"light", { firstTime + 0.1s, 0.5 }, { firstTime + 0.15s, 1 })
		.set(U"light", { secondTime,1 }, { secondTime + 0.05s, 0.5 })
		.set(U"light", { secondTime + 0.1s, 0.5 }, { secondTime + 0.15s, 1 });
}

OpeningScene::~OpeningScene()
{
	LoadAsset::Wait();
}

void OpeningScene::update()
{
	if (Scene::Rect().leftClicked() || Scene::Rect().rightClicked())
	{
		AudioAsset{ U"キャンセル2" }.playOneShot();
		changeScene(U"TitleScene", 2.0s);
	}
	else if (GetDownXInput())
	{
		AudioAsset{ U"キャンセル2" }.playOneShot();
		changeScene(U"TitleScene", 2.0s);
	}
	else
	{
		for (const auto& key : Keyboard::GetAllInputs())
		{
			if (key.down())
			{
				AudioAsset{ U"キャンセル2" }.playOneShot();
				changeScene(U"TitleScene", 2.0s);
				break;
			}
		}
	}


	time += Scene::DeltaTime();

	spotLightLeft = Math::SmoothDamp(spotLightLeft, Scene::Center() + Vec2{ -20,10 }, spotLightLeftVel, 0.5);
	spotLightRight = Math::SmoothDamp(spotLightRight, Scene::Center() + Vec2{ 20,-10 }, spotLightRightVel, 0.5);

	if (3.5 <= time)
	{
		animation.start();
	}

	if (8.5+0.5 < time)
	{
		changeScene(U"TitleScene", 2.0s);
	}
}

void OpeningScene::draw() const
{
	Scene::SetBackground(ColorF{ 0.0 });

	if (time < 3.5)
	{
		Scene::Rect().draw(ColorF{ 0.2 });

		ctrlLogo.resized(Scene::Width() * 0.4).drawAt(Scene::Center() + Vec2{ 0,-30 });
		{
			ScopedSpotlight target{ spotLight,ColorF{0} };

			Circle{ spotLightLeft ,250 }.drawShadow(Vec2{ 0,0 }, 20, 3, ColorF{ 0.7 * Clamp((2.5 - time) + 1, 0.0, 1.0) });
			Circle{ spotLightRight ,250 }.drawShadow(Vec2{ 0,0 }, 20, 3, ColorF{ 0.7 * Clamp((2.5 - time) + 1, 0.0, 1.0) });
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



