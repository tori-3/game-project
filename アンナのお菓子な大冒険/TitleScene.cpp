#include"TitleScene.h"
#include"BGMManager.hpp"
#include"KeyInfo.h"
#include"ControllerInput.h"
#include"MyBlendState.h"

bool TitleScene::updateStick(const Vec2& pos)
{
	constexpr SizeF chocolateSize{ 300,50 };
	const RoundRect rrect{ pos,chocolateSize,chocolateSize.y / 2.0 };

	if (rrect.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	return rrect.leftClicked();
}

void TitleScene::drawStick(const Vec2& pos, StringView text,bool notify)const
{
	constexpr SizeF chocolateSize{ 300,50 };
	const RoundRect rrect{ pos,chocolateSize,chocolateSize.y / 2.0 };
	constexpr double textureWidth = 200;
	stickTexture.resized(chocolateSize.y * 0.7, textureWidth).rotated(90_deg).drawAt(rrect.rightCenter() + Vec2{ textureWidth / 2.0 - rrect.r,0 });
	rrect.draw(Arg::top = Color{ 129,74,42 }, Arg::bottom = Color{ 92,43,20 });
	FontAsset{ U"NormalFont" }(text).drawAt(chocolateSize.y * 0.7, rrect.center());

	if(notify)
	{
		notifyIcon.drawAt(rrect.rect.pos + Vec2{ 15,15 }, Palette::Orange);
	}
}

TitleScene::TitleScene(const InitData& init)
	: IScene{ init }
{
	BGMManager::get().play(U"BGM/StageCloud_Pxtone5.2.mp3", 0s);

	LoadAsset::RegisterTexture(U"ロゴ.png");
	character.addMotion(U"Walk", true);

	if(getData().clearStage==getData().LastBossStage)
	{
		menuList.insert(menuList.begin() + 3, U"思い出");
		funcList.insert(funcList.begin() + 3, [&] {changeScene(U"GalleryScene"); });
	}

	if(getData().ChocoMountain<= getData().clearStage)
	{
		pathList << U"BackGroundTexture/洞窟背景.png";
	}

	if (getData().CandyCloud <= getData().clearStage)
	{
		pathList << U"BackGroundTexture/雲背景.png";
	}

	if (getData().LastBossStage <= getData().clearStage)
	{
		pathList << U"BackGroundTexture/ラスボス背景.png";
		pathList << U"BackGroundTexture/宇宙背景.png";
	}

	if (getData().pressAnyKeyFlg)
	{
		Size size{ 800,300 };

		downsample = RenderTexture{ size / 4,ColorF{0,0} };
		const RenderTexture internalTexture2{ size / 4 };

		{
			const ScopedRenderTarget2D target{ downsample };
			const ScopedRenderStates2D blend{ MakeBlendState() };
			const Transformer2D trans{ Mat3x2::Scale(1.0/4.0) };
			RectF rect{ Arg::center = size/2,400,90 };

			ColorF color{ 0,0.4 };
			ColorF transparent{ color,0.0 };

			RectF{ Arg::topRight = rect.pos,100,rect.h }.draw(Arg::left = transparent, Arg::right = color);
			rect.draw(color);
			RectF{ Arg::topLeft = rect.tr(),100,rect.h}.draw(Arg::left = color, Arg::right = transparent);

		}

		//Shader::Downsample(original, downsample);
		Shader::GaussianBlur(downsample, internalTexture2, downsample);
	}
	else
	{
		//timer.start();
		omakeTimer.start();
	}
}

void TitleScene::update()
{
	timer.start();

	rogoTimer += Scene::DeltaTime();

	if (not (timer.isRunning() && 0s < timer))
	{
		timer.restart();

		textureIndex = (textureIndex + 1) % pathList.size();
	}

	if(getData().pressAnyKeyFlg)
	{
		bool anyKey = false;

		if (Scene::Rect().leftClicked() || Scene::Rect().rightClicked())
		{
			anyKey = true;
		}
		else if (AnyXInputPressed())
		{
			anyKey = true;
		}
		else
		{
			for (const auto& key : Keyboard::GetAllInputs())
			{
				if (key.down())
				{
					anyKey = true;
					break;
				}
			}
		}

		if(anyKey && not anyKeyTimer.isRunning())
		{
			AudioAsset{ U"決定ボタンを押す6" }.playOneShot();
			anyKeyTimer.start();
		}

		if(anyKeyTimer.reachedZero())
		{
			//timer.start();
			omakeTimer.start();
			getData().pressAnyKeyFlg = false;
		}

		return;
	}

	if(getData().garellyNotificationAddonFlg)
	{
		NotificationAddon::Show(U"[思い出]が追加されました", U"エンドロールなどを確認できます", NotificationAddon::Type::Garelly);
		getData().garellyNotificationAddonFlg = false;
	}

	uiManager.update();
	time += Scene::DeltaTime();

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
		if (upInput.down())
		{
			if (0 < selectedIndex)
			{
				--selectedIndex;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (downInput.down())
		{
			if (selectedIndex < menuList.size() - 1)
			{
				++selectedIndex;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (getData().menuDecisionKey.down())
		{
			menuClicked = true;
			AudioAsset{ U"決定ボタン" }.playOneShot();
		}

		{
			Transformer2D target{ Mat3x2::Translate(translate(),0),TransformCursor::Yes };
			Vec2 pos = menuPos;

			for (size_t i = 0; i < menuList.size(); ++i)
			{
				if (updateStick(pos + (selectedIndex == i ? Vec2{ -30,0 } : Vec2{})))
				{
					AudioAsset{ U"決定ボタン" }.playOneShot();
					selectedIndex = i;
					menuClicked = true;
				}
				pos.y += menuHeight;
			}
		}

		if (menuClicked)
		{
			funcList[selectedIndex]();
		}
	}

	if (menuClicked && selectedIndex == 2 && uiManager.getChildren().size() == 0)
	{
		getData().save();
		menuClicked = false;
	}


	if (changeMapTimer.isStarted())
	{
		playerPos.x += Scene::DeltaTime() * 500 * 2;
	}

	if (changeMapTimer.reachedZero())
	{
		if (getData().clearStage == 0)
		{
			JSON json = JSON::Load(U"map.json");
			getData().initGame();
			getData().mini_mode = Stage_Mode;
			getData().stage = 1;
			getData().stageFile = json[U"StageData"][U"Stage1"][U"MapFile"].getString();;
			getData().backgroundTexture = json[U"StageData"][U"Stage1"][U"BackgroundTexture"].getString();
			getData().sceneName = json[U"StageData"][U"Stage1"][U"SceneName"].get<String>();
			getData().description = U"Space or W：ジャンプ\nA：左\nD：右\nS：しゃがむ\nEnter：技を発動\nEnter長押し：突進(クッキーが10個貯まったら)";
			getData().BGMPath = json[U"StageData"][U"Stage1"][U"BGM"].getString();
			changeScene(getData().sceneName);
			AudioAsset::Register(getData().BGMPath, getData().BGMPath, Loop::Yes);
			BGMManager::get().stop();
		}
		else
		{
			changeScene(U"Map");
		}
	}

	if (changeMiniGameTimer.isStarted())
	{
		if (0.7s < changeMiniGameTimer.duration() - Duration{ changeMiniGameTimer.sF() })
		{
			playerPos.y += Scene::DeltaTime() * 500 * 2;
		}
	}

	if (changeMiniGameTimer.reachedZero())
	{
		changeScene(U"MiniGameSelect");
	}

	if (omakeTimer.reachedZero())
	{
		omakeTimer.restart();

		if(RandomBool(0.6))
		{
			character.addMotion(U"Akubi");
		}
	}

	character.update(playerPos, false);
}

void TitleScene::draw() const
{

	constexpr double changeTime = 3.0;

	if (1 < pathList.size() && timer.sF() <= changeTime)
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

	TextureAsset{ U"ロゴ.png" }.scaled(1.3).drawAt(rogoPos, ColorF{ 1,Min(rogoTimer / 2.0,1.0) });


	if (getData().pressAnyKeyFlg)
	{
		if (not anyKeyTimer.isRunning())
		{
			downsample.scaled(4).drawAt(anyKeyPos,ColorF{ 1,Min(rogoTimer / 2.0,1.0) });

			const ColorF color = AlphaF(Periodic::Sine0_1(2s) * Min(rogoTimer / 2.0, 1.0));
			FontAsset{ U"NormalFont" }(U"Press Any Button").drawAt(40, anyKeyPos - Vec2{ 0,20 }, color);
			FontAsset{ U"NormalFont" }(U"なにかボタンを押してください").drawAt(30, anyKeyPos + Vec2{ 0,23 }, color);
		}
		else
		{
			downsample.scaled(4, Max(0.0, (1 - anyKeyTimer.progress0_1() * 10) * 4)).drawAt(anyKeyPos);
		}

		return;
	}

	{
		Transformer2D target{ Mat3x2::Translate(translate(),0),TransformCursor::Yes };


		Vec2 pos = menuPos;

		for (size_t i = 0; i < menuList.size(); ++i)
		{
			const bool notify = (i==1&&getData().notifyMiniGameSelect)||(i==3&&getData().notifyGallery);
			drawStick(pos + (selectedIndex == i ? Vec2{ -30,0 } : Vec2{}), menuList[i],notify);

			pos.y += menuHeight;
		}

	}

	character.draw();

	const String explanation = U" {}-上  {}-下  {}-左  {}-右 {}-決定  {}-戻る"_fmt(ToKeyName(getData().minigameUpKey, getData().gamepadMode), ToKeyName(getData().minigameDownKey, getData().gamepadMode), ToKeyName(getData().minigameLeftKey, getData().gamepadMode), ToKeyName(getData().minigameRightKey, getData().gamepadMode), ToKeyName(getData().menuDecisionKey, getData().gamepadMode), ToKeyName(getData().menuBackKey, getData().gamepadMode));
	FontAsset{ U"NormalFont" }(explanation).draw(30,Arg::bottomLeft(5, Scene::Height() - 5), AlphaF(Min(time / 2.0, 1.0)));

	uiManager.draw();

}

std::shared_ptr<UIElement> TitleScene::licenseDialog()
{
	auto showBrowserButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 220, .child = TextUI::Create({.text = U"\U000F059FLicenses",.color = Palette::White}) });
	auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 20,.margine = 10,.width = 220, .child = TextUI::Create({.text = U"\U000F05AD閉じる",.color = Palette::White}) });
	closeButton->selected = true;
	auto scrollbar = SimpleScrollbar::Create
	({
		.flex = 1.0,
		.child = Lisence::LicenseDisplay()
	});


	return SimpleDialog::Create
	({
		.erasable = false,
		.child = SweetsPanel::Create
		({
			.margine = 5,
			.child = Column::Create
			({
				.children
				{
					scrollbar,
					Row::Create
					({
						.children
						{
							showBrowserButton,
							closeButton
						}
					})
				}
			}),
		}),
		.updateFunc = [=](SimpleDialog* dialog)
		{
			if (getData().minigameLeftKey.down()|| ControllerManager::LeftDown())
			{
				if (closeButton->selected)
				{
					showBrowserButton->selected = true;
					closeButton->selected = false;
					AudioAsset{U"カーソル移動"}.playOneShot();
				}
				else
				{
					AudioAsset{ U"ビープ音" }.playOneShot();
				}
			}

			if (getData().minigameRightKey.down()||ControllerManager::RightDown())
			{
				if (showBrowserButton->selected)
				{
					showBrowserButton->selected = false;
					closeButton->selected = true;
					AudioAsset{ U"カーソル移動" }.playOneShot();
				}
				else
				{
					AudioAsset{ U"ビープ音" }.playOneShot();
				}
			}

			if (getData().minigameUpKey.pressed()|| ControllerManager::UpPressed())
			{
				scrollbar->addScrollPos(-Scene::DeltaTime() * 1000);
			}

			if (getData().minigameDownKey.pressed()|| ControllerManager::DownPressed())
			{
				scrollbar->addScrollPos(Scene::DeltaTime() * 1000);
			}

			if (getData().menuDecisionKey.down())
			{
				if (showBrowserButton->selected)
				{
					LicenseManager::ShowInBrowser();
					AudioAsset{ U"決定ボタン" }.playOneShot();
				}
				else
				{
					dialog->close();
					menuClicked = false;

					KeyConfigUtility::CleapInput(getData().menuDecisionKey);

					AudioAsset{ U"キャンセル" }.playOneShot();
				}
			}
			else
			{
				if (closeButton->clicked() || getData().menuBackKey.down())
				{
					dialog->close();
					menuClicked = false;
					AudioAsset{ U"キャンセル" }.playOneShot();
				}
				else if (showBrowserButton->clicked())
				{
					LicenseManager::ShowInBrowser();
					AudioAsset{ U"決定ボタン" }.playOneShot();
				}
			}
		}
	});
}
