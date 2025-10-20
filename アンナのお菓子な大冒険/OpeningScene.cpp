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

	animation
		.set(U"light2", { 1s,0 }, { 3s, 0.1 })
		.set(U"light2", { firstTime,1 }, { firstTime + 0.05s, 0.5 })
		.set(U"light2", { firstTime + 0.1s, 0.5 }, { firstTime + 0.15s, 1 })
		.set(U"light2", { secondTime,1 }, { secondTime + 0.05s, 0.5 })
		.set(U"light2", { secondTime + 0.1s, 0.5 }, { secondTime + 0.15s, 1 });
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
	else if (AnyXInputPressed())
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

void Brick(const Rect& rect, int32 n, const ColorF& color = Color(162, 72, 43), const ColorF& back_color = Palette::White) {

	const ScopedViewport2D viewport{ rect.draw(back_color) };
	const double hight = rect.h / (double)n;
	const double width = hight * 2;
	const double d = hight * 0.1;

	for (auto x : step((int32)(rect.w / hight) + 2))
	{
		for (auto y : step(n))
		{
			if (IsEven(x + y))
			{
				RectF{ (x * hight + d / 2 - hight), (y * hight + d / 2), width - d,hight - d }.draw(color);
			}
		}
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
		const double alpha2 = animation[U"light2"];

		//Brick(Rect{ Scene::Size() }, 10, ColorF{ 0.1 }, ColorF{ 0.2 });

		siv3dLogoFrame.drawAt(Scene::Center(), AlphaF(alpha));

		{
			ScopedLightBloom target{ light };
			siv3dLogoFrameBold.drawAt(Scene::Center(), ColorF(Palette::Cyan, alpha2));
			//siv3dLogo.drawAt(Scene::Center(), ColorF(Palette::Cyan, alpha2));
		}
		light.draw();

	}
}



