#include"TitleScene.h"
#include"BGMManager.hpp"

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
	FontAsset{ U"TitleFont" }(text).drawAt(chocolateSize.y * 0.7, rrect.center());

	if(notify)
	{
		notifyIcon.drawAt(rrect.rect.pos + Vec2{ 15,15 }, Palette::Orange);
	}
}

TitleScene::TitleScene(const InitData& init)
	: IScene{ init }
{
	LoadAsset::RegisterAudio(U"BGM/StageCloud_Pxtone5.2.mp3", Loop::Yes);
	BGMManager::get().play(U"BGM/StageCloud_Pxtone5.2.mp3",0s);

	LoadAsset::RegisterTexture(U"ロゴ.png");
	character.addMotion(U"Walk", true);

	if(getData().clearStage==getData().LastBossStage)
	{
		menuList.insert(menuList.begin() + 2, U"思い出");
		funcList.insert(funcList.begin() + 2, [&] {changeScene(U"GalleryScene"); });
	}
}

void TitleScene::update()
{
	uiManager.update();
	time += Scene::DeltaTime();

	if (not (timer.isRunning() && 0s < timer))
	{
		timer.restart();

		textureIndex = (textureIndex + 1) % pathList.size();
	}

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
		changeScene(U"Map");
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


	character.update(playerPos, false);
}

void TitleScene::draw() const
{

	constexpr double changeTime = 3.0;

	if (timer.sF() <= changeTime)
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

	TextureAsset{ U"ロゴ.png" }.scaled(1.3).drawAt(rogoPos, ColorF{ 1,Min(time / 2.0,1.0) });

	{
		Transformer2D target{ Mat3x2::Translate(translate(),0),TransformCursor::Yes };


		Vec2 pos = menuPos;

		for (size_t i = 0; i < menuList.size(); ++i)
		{
			const bool notify = (i==1&&getData().notifyMiniGameSelect)||(i==2&&getData().notifyGallery);
			drawStick(pos + (selectedIndex == i ? Vec2{ -30,0 } : Vec2{}), menuList[i],notify);

			pos.y += menuHeight;
		}

	}
	FontAsset{ U"NormalFont" }(U"操作：[W]↑ [S]↓ [A]← [D]→ [Enter]決定 [Q]戻る").draw(Arg::bottomLeft(5, Scene::Height() - 5), AlphaF(Min(time / 2.0, 1.0)));

	character.draw();


	uiManager.draw();
}

std::shared_ptr<UIElement> TitleScene::licenseDialog()
{
	auto showBrowserButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 220, .child = TextUI::Create({.text = U"ブラウザで見る",.color = Palette::White}) });
	auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 20,.margine = 10,.width = 220, .child = TextUI::Create({.text = U"\U000F05AD閉じる",.color = Palette::White}) });
	closeButton->selected = true;
	auto scrollbar = SimpleScrollbar::Create
	({
		.flex = 1.0,
		.child = Lisence::LicenseDisplay()
	});


	return SimpleDialog::Create
	({
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
			if (getData().minigameLeftKey.down())
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

			if (getData().minigameRightKey.down())
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

			if (getData().minigameUpKey.pressed())
			{
				scrollbar->addScrollPos(-Scene::DeltaTime() * 1000);
			}

			if (getData().minigameDownKey.pressed())
			{
				scrollbar->addScrollPos(Scene::DeltaTime() * 1000);
			}

			if (getData().menuDecisionKey.pressed())
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
