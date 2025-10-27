#include"OpeningScene.h"
#include"ControllerInput.h"
#include"LoadAsset.h"

OpeningScene::OpeningScene(const InitData& init)
	: IScene{ init }
{
	animation
		.set(U"angle", { 1.751s,0 }, { 1.8s,5_deg })
		.set(U"angle", { 1.801s,5_deg }, { 1.9s,-5_deg })
		.set(U"angle", { 1.901s,-5_deg }, { 2.0s,5_deg })
		.set(U"angle", { 2.001s,5_deg }, { 2.1s,-5_deg })
		.set(U"angle", { 2.101s,-5_deg }, { 2.2s,5_deg })
		.set(U"angle", { 2.201s,5_deg }, { 2.3s,-5_deg })
		.set(U"angle", { 2.301s,-5_deg }, { 2.35s,0_deg })
		.set(U"alpha", { 3.5s,1 }, { 4.5s, 0 });
}

OpeningScene::~OpeningScene()
{
	LoadAsset::Wait();
}

void OpeningScene::update()
{
	animation.start();

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

	if (6.5 < time)
	{
		changeScene(U"TitleScene", 2.0s);
	}
}

void OpeningScene::draw() const
{
	if (time < 3.5)
	{
		Scene::SetBackground(ColorF{ 0.2 });

		ctrlLogo.resized(Scene::Width() * 0.4).rotated(animation[U"angle"]).drawAt(Scene::Center() + Vec2{ 0,-30 });
		{
			ScopedSpotlight target{ spotLight,ColorF{0} };

			Circle{ spotLightLeft ,250 }.drawShadow(Vec2{ 0,0 }, 20, 3, ColorF{ 0.7 * Clamp((2.5 - time) + 1, 0.0, 1.0) });
			Circle{ spotLightRight ,250 }.drawShadow(Vec2{ 0,0 }, 20, 3, ColorF{ 0.7 * Clamp((2.5 - time) + 1, 0.0, 1.0) });
		}

		spotLight.draw();
	}
	else
	{
		Scene::SetBackground(Palette::White);

		siv3dLogo.resized(Scene::Width() * 0.8).drawAt(Scene::Center());

		Scene::Rect().draw(ColorF{0,animation[U"alpha"]});

	}
}
