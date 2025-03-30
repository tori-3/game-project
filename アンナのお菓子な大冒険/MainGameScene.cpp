#include"MainGameScene.h"
#include"Fairy.h"

MainGameScene::MainGameScene(const InitData& init)
	: MiniGameSceneBase{ init }
{
	DataManager::get().maxHP = getData().maxHP;

	adder.update(manager);
	player = dynamic_cast<Player*>(manager.get(U"Player"));
	player->setDataP(&getData());

	stage.update(player->pos);
	adder.update(manager);
	manager.stage = &stage;


	camera.setStageWidth(stage.width() * rect_size);
	camera.setStageHeight(stage.height() * rect_size);
	camera.update({ player->pos.x - draw_x,player->pos.y - draw_y });


	if (getData().backgroundTexture == U"BackgroundTexture/洞窟背景.png")
	{
		light = new Spotlight{ Scene::Size() };
	}

	if (getData().stage == 2)
	{
		manager.add(new Fairy{});
	}
}

void MainGameScene::gameUpdate()
{

	if (TalkManager::get().talkWindow.isContinue())
	{
		TalkManager::get().talkWindow.update();

		return;
	}

	stage.update(player->pos);

	if (not bgmStart)
	{
		BGMManager::get().play(getData().BGMPath);
		bgmStart = true;
	}
	ClearPrint();
	DataManager::get().playerPos = player->pos;

	adder.update(manager);

	//座標変換(カーソルだけ)
	{
		const Transformer2D t{ Mat3x2::Identity(),camera.getMat3x2() };
		manager.update();
	}

	if (DataManager::get().table.contains(U"IrisOut"))
	{
		if (not irisOut.isStart)
		{
			irisOut.start(&player->pos, 140);
		}
	}


	if (not DataManager::get().playerAlive)
	{
		if (DataManager::get().table.contains(U"Clear"))
		{
			if (DataManager::get().table.contains(U"LastBoss"))
			{
				changeScene(U"EndRollScene");
			}
			else
			{
				EndGame(true);
			}

			for (const auto& enemy : manager.getArray(U"Enemy"))
			{
				enemy->kill();
			}

		}
		else
		{
			EndGame(false);
		}
	}


	//ステージupdate

	if (KeyEscape.down())goPause();

	if (DataManager::get().cameraPos)
	{
		camera.update(DataManager::get().cameraPos.value());
	}
	else
	{
		camera.update({ player->pos.x - draw_x,player->pos.y - draw_y });
	}

	irisOut.update();

	if (DataManager::get().bossHPRate)
	{
		hpBar.update(DataManager::get().bossHPRate.value());
	}

	DataManager::get().time += Scene::DeltaTime();
}

void MainGameScene::gameDraw() const
{
	Scene::SetBackground(skyColor);

	//ラスボスの時の背景位置
	if (DataManager::get().table.contains(U"LastBoss"))
	{
		background.resized(Scene::Size()).draw(0, -30);
	}
	else
	{
		background.resized(Scene::Size()).draw();
	}

	backgroundManager.draw(camera.pos);

	Rect{ Scene::Size() }.draw(ColorF{ skyColor,0.2 });

	{
		//座標変換
		const Transformer2D t{ camera.getMat3x2() };
		manager.draw();
		stage.draw(camera.pos + Scene::Size() / 2);
		DataManager::get().effect.update();

		{
			const ScopedRenderTarget2D target{ rTexture.clear(ColorF{0.5,0}) };
			const ScopedRenderStates2D blend{ MakeTransparentBlendState() };
			DataManager::get().additiveEffect.update();
		}
	}

	if (light)
	{
		{
			//ライトを描く
			ScopedSpotlight target{ *light,ColorF{0.3} };
			const Transformer2D t{ camera.getMat3x2() };

			Circle{ player->pos,500 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });

			if (DataManager::get().table.contains(U"SlaversCookie"))
			{
				Circle{ manager.get(U"SlaversCookie")->pos,100 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
			}

		}

		light->draw();
	}

	Graphics2D::Flush();
	rTexture.resolve();
	rTexture.draw();

	CookieDisplay(player->itemCount, DataManager::get().tame);
	HpDisplay(player->hp, DataManager::get().maxHP);

	TalkManager::get().talkWindow.draw(RectF{ 0,500,Scene::Size().x,300 });

	FontAsset{ U"NormalFont" }(U"[ESC]ポーズ").draw(Arg::topRight = Vec2{ Scene::Width() - 10,5 });

	if (DataManager::get().bossHPRate)
	{
		hpBar.draw(Palette::Purple, Palette::Red);
	}

	{
		const Transformer2D t{ camera.getMat3x2() };
		irisOut.draw();
	}
}

void MainGameScene::drawFadeIn(double t) const
{
	draw();
	{
		const Transformer2D _{ camera.getMat3x2() };
		Circle{ player->pos,0 }.lerp(Circle{ player->pos ,Scene::Size().length() }, t)
			.drawFrame(0, Scene::Size().length(), Palette::Black);
	}
}

void MainGameScene::CookieDisplay(int32 count, double tame)
{

	if (not TextureAsset::IsRegistered(U"CookieItem"))
	{
		TextureAsset::Register(U"CookieItem", U"🍪"_emoji);
	}

	for (auto i : step(10))
	{

		if ((i + 1) <= tame * 10)
		{
			Circle{ 25 + i * 40, 75,20 }.drawShadow(Vec2{ 0, 0 }, 8, 2, HSV{ 360 / 10 * i, 0.7 });
		}

		if (i < count)
		{
			TextureAsset(U"CookieItem").resized(40).drawAt(25 + i * 40, 75);
		}
		else
		{
			TextureAsset(U"CookieItem").resized(40).drawAt(25 + i * 40, 75, ColorF{ 1,0.3 });
		}

		if ((i + 1) <= tame * 10)
		{
			Circle{ 25 + i * 40, 75,20 }.draw(HSV{ 360 / 10 * i, 0.2 });
		}
	}
}

void MainGameScene::HpDisplay(int32 count, int32 maxHP)
{

	if (not TextureAsset::IsRegistered(U"HP"))
	{
		TextureAsset::Register(U"HP", U"❤"_emoji);
	}

	if (10 < count)
	{
		for (auto i : step(10))
		{
			if (i < count)
			{
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25);
			}
			else
			{
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25, ColorF{ 1,0.3 });
			}
		}

		FontAsset(U"TitleFont")(U"＋{}"_fmt(count - 10)).draw(35, 25 + 10 * 40 - 10, 0);
	}
	else
	{
		for (auto i : step(Min(maxHP, 10)))
		{
			if (i < count)
			{
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25);
			}
			else
			{
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25, ColorF{ 1,0.3 });
			}
		}
	}

}
