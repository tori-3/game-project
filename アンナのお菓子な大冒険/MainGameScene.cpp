#include"MainGameScene.h"
#include"Fairy.h"
#include"KeyInfo.h"
#include"Gimmick.h"

class StageEntity :public Entity
{
public:

	StageEntity(Stage* p_stage, Vec2* p_cameraPos,Vec2* p_playerPos)
		:Entity(U"Stage", RectF::Empty(), {}, {}, 1)
		,p_stage{ p_stage }
		,p_cameraPos{p_cameraPos}
		,p_playerPos{p_playerPos}
	{
		z = 100;
	}

	void update()override
	{
		p_stage->update(*p_playerPos);
	}

	void draw()const override
	{
		p_stage->draw(*p_cameraPos + Scene::Center());
	}

private:
	Stage* p_stage;
	Vec2* p_cameraPos;
	Vec2* p_playerPos;
};

MainGameScene::MainGameScene(const InitData& init)
	: MiniGameSceneBase{ init }
{
	DataManager::get().gameData = &getData();

	if (getData().stage == 12)
	{
		DataManager::get().table.insert(U"Falling");
	}

	if (getData().stage == 17)
	{
		manager.add(new BigElevator{ {stage.width(),stage.height()} });
	}

	if(getData().stage==1)
	{
		TextureAsset::Register(U"kaiso",U"kaiso.png");
	}

	if (getData().stage == 24)
	{
		DataManager::get().addEntity(U"BigCloudEnemy",Vec2{Scene::Center()});
	}

	DataManager::get().maxHP = getData().maxHPList[getData().stage - 1];

	adder.update(manager);
	player = dynamic_cast<Player*>(manager.get(U"Player"));
	player->setDataP(&getData());

	stage.update(player->pos);
	adder.update(manager);
	manager.stage = &stage;


	camera.setStageWidth(stage.width() * rect_size);
	camera.setStageHeight(stage.height() * rect_size);

	if (DataManager::get().table.contains(U"Falling"))
	{
		camera.update({ player->pos.x - draw_x,player->pos.y - 150 });
	}
	else if(DataManager::get().isElevatorStage)
	{
		camera.update({ player->pos.x - Scene::Center().x,player->pos.y - 500 });
	}
	else
	{
		camera.update({ player->pos.x - draw_x,player->pos.y - draw_y });
	}



	if (getData().backgroundTexture == U"BackgroundTexture/洞窟背景.png")
	{
		light = new Spotlight{ Scene::Size() };
	}

	if (getData().stage == 2)
	{
		fairy = new Fairy{};
		manager.add(fairy);
	}

	manager.add(new StageEntity{ &stage,&camera.pos,&player->pos });
}

void MainGameScene::gameUpdate()
{
	if(getData().stage==1)
	{
		if (isShowKaiso())
		{
			kaisouAlpha = Min(kaisouAlpha + Scene::DeltaTime() / 0.5, 1.0);
		}
		else
		{
			kaisouAlpha = Max(kaisouAlpha - Scene::DeltaTime() / 0.5, 0.0);
		}
	}

	if (TalkManager::get().talkWindow.isContinue())
	{
		DataManager::get().table.insert(U"TalkWindow");
		TalkManager::get().talkWindow.update(getData().minigameLeftKey.down(),(getData().attackKey|getData().minigameRightKey).down());

		if(fairy)
		{
			fairy->update();
			fairy->lateUpdate();
		}

		return;
	}
	else
	{
		DataManager::get().table.erase(U"TalkWindow");
	}

	//stage.update(player->pos);

	if (not bgmStart)
	{
		BGMManager::get().play(getData().BGMPath);
		bgmStart = true;
	}

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


	if(not saveFlg)
	{
		if (DataManager::get().table.contains(U"Clear"))
		{
			saveFlg = true;
			getData().saveMainGame(true);
		}
		else if (not DataManager::get().playerAlive)
		{
			saveFlg = true;
			getData().saveMainGame(false);
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

	if (getData().pauseKey.down())goPause();

	if (DataManager::get().cameraPos)
	{
		camera.update(DataManager::get().cameraPos.value());
	}
	else
	{
		if (DataManager::get().table.contains(U"Falling"))
		{
			camera.update({ player->pos.x - draw_x,player->pos.y - 150 });
		}
		else if (DataManager::get().isElevatorStage)
		{
			camera.update({ player->pos.x - Scene::Center().x,player->pos.y - 500 });
		}
		else
		{
			camera.update({ player->pos.x - draw_x,player->pos.y - draw_y });
		}
	}

	irisOut.update();

	if (DataManager::get().bossHPRate)
	{
		hpBar.update(DataManager::get().bossHPRate.value());
	}

	DataManager::get().time += Scene::DeltaTime();


	damageTimer = Max(damageTimer - Scene::DeltaTime(), 0.0);

	if (DataManager::get().table.contains(U"Damage"))
	{
		damageTimer = 0.2;

		DataManager::get().table.erase(U"Damage");
	}
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

	{
		if(getData().CandyCloud<=getData().stage&& getData().stage < getData().LastBossStage)
		{
			const ScopedRenderTarget2D target{ backgroundRenderTexture.clear(ColorF{1.0,0}) };
			const ScopedRenderStates2D blend{ MakeBlendState() };
			ScopedColorMul2D mul{ ColorF{1,0.7} };
			backgroundManager.draw(camera.pos);
		}
		else
		{
			const ScopedRenderTarget2D target{ backgroundRenderTexture.clear(ColorF{0.6,0}) };
			const ScopedRenderStates2D blend{ MakeBlendState() };
			backgroundManager.draw(camera.pos);
		}
	}

	//halfRenderTexture
	//Shader::Downsample(backgroundRenderTexture, downsample);
	Shader::GaussianBlur(backgroundRenderTexture, internalTexture, backgroundRenderTexture);
	backgroundRenderTexture.draw();

	{
		//座標変換
		const Transformer2D t{ camera.getMat3x2() };
		manager.draw();
		//stage.draw(camera.pos + Scene::Size() / 2);
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

				if(manager.contains(U"Rocks"))
				{
					Circle{ manager.get(U"Rocks")->pos,150 }.draw(ColorF{ 1.0 }, ColorF{ 0.0 });
				}
			}

		}

		light->draw();
	}

	Graphics2D::Flush();
	rTexture.resolve();
	rTexture.draw();

	CookieDisplay(player->itemCount, DataManager::get().tame);
	HpDisplay(player->hp, DataManager::get().maxHP);

	if (0 < kaisouAlpha)
	{
		TextureAsset{ U"kaiso" }.resized(Scene::Size()).draw(AlphaF(kaisouAlpha));
	}

	constexpr double height = 200;
	constexpr double space = 50;
	TalkManager::get().talkWindow.draw(RectF{ space,Scene::Height() - height - space,Scene::Width() - space * 2 ,height }, { 250,50 },false,ToKeyName(getData().attackKey));

	FontAsset{ U"NormalFont" }(U"{} ポーズ"_fmt(ToKeyName(getData().pauseKey))).draw(Arg::topRight = Vec2{ Scene::Width() - 10,5 });

	if (DataManager::get().bossHPRate)
	{
		RoundRect{ Arg::center(Scene::Center().x, 30),FontAsset{ U"NormalFont" }(DataManager::get().bossName).region().w,20,10}.drawShadow(Vec2{0,0}, 24, 10, ColorF{0,0.3});

		FontAsset{ U"NormalFont" }(DataManager::get().bossName).drawAt(25,Scene::Center().x, 30);
		hpBar.draw(Palette::Purple, Palette::Red);
	}

	if (0 < damageTimer)
	{
		Scene::Rect().drawFrame(40, 0, ColorF{ Palette::Red, 0.0 }, ColorF{ Palette::Red, 0.7 });
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
