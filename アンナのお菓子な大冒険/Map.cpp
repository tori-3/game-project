#include"Map.h"
#include"KeyInfo.h"

void Map::updatePos()
{
	Vec2 vec{};

	size_t nextIndex = 0;

	if (drawIndex < index)
	{
		nextIndex = 1;
	}

	if (index < drawIndex)
	{
		nextIndex = -1;
	}

	Vec2 nextPos = stagePosList[drawIndex + nextIndex] + (typeList[drawIndex + nextIndex] == StageType::Boss ? Vec2{ -15,0 } : Vec2{ 0,0 });

	if ((nextPos - playerPos).length() < 0.1)
	{
		drawIndex += nextIndex;
		return;
	}

	playerPos = Math::SmoothDamp(playerPos, nextPos, vec, 0.001, 5000);
}

Map::Map(const InitData& init)
	: IScene{ init }
{
	Scene::SetBackground(Palette::Skyblue);

	clearStage = getData().clearStage;

	//clearStage = saveDatajson[U"ClearStage"].get<int32>();

	//if (getData().mini_clear == true && clearStage < getData().stage)
	//{
	//	saveDatajson[U"ClearStage"] = getData().stage;
	//	saveDatajson.save(U"saveData.json");
	//	clearStage = getData().stage;
	//}

	//if (getData().backFromMainGameScene)
	//{
	//	if (getData().mini_clear || getData().quitStage)
	//	{
	//		saveDatajson[U"MaxHP"][getData().stage - 1] = 5;
	//	}
	//	else
	//	{
	//		saveDatajson[U"MaxHP"][getData().stage - 1] = getData().maxHP + 2;
	//	}

	//	saveDatajson.save(U"saveData.json");
	//}



	index = getData().stage - 1;
	drawIndex = index;

	for (int i = 0; i < rect_num; i++)
	{
		//各ステージの読み込み
		String str{ U"Stage{}"_fmt(i + 1) };
		pictures << Texture{ json[U"StageData"][str][U"Picture"].get<String>() };
		sentences << json[U"StageData"][str][U"Sentence"].get<String>();
		sceneNames << json[U"StageData"][str][U"SceneName"].get<String>();
		Point pos = json[U"StageData"][str][U"StagePos"].get<Point>();
		stagePosList << pos;
		stageList << json[U"StageData"][str][U"MapFile"].getString();
		tagList << json[U"StageData"][str][U"Tag"].getString();
		textList << json[U"StageData"][str][U"Text"].getString();
		
		title << sentences[i].split_lines()[0];

		if (json[U"StageData"][str][U"Type"] == U"Boss")
		{
			typeList << StageType::Boss;
		}
		else if (json[U"StageData"][str][U"Type"] == U"MiniGame")
		{
			typeList << StageType::MiniGame;
		}
		else
		{
			typeList << StageType::Stage;
		}
	}

	playerPos = stagePosList[index] + (typeList[index] == StageType::Boss ? Vec2{ -15,0 } : Vec2{ 0,0 });
	//character.addMotion(U"Walk");
	character.addMotion(U"Standing");
	character.update(playerPos - Point{ 0 ,30 }, false);


	//AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
	BGMManager::get().play(U"BGM/WorldMap2.mp3");


	//アニメーション
	fairy.addMotion(U"Floating", true);

	snowKnightKiriageTimer.start();
	snowKnight.addMotion(U"kiriage");

	cookieDoreishoTimer.start();
	cookieDoreisho.addMotion(U"walk");

	captain.addMotion(U"Mokumoku", true);
	captainTimer.start();

	lastBoss.addMotion(U"Stand");
	lastBossTimer.start();

	itigoSlave.addMotion(U"attack",true);

	itigo.addMotion(U"", true);

	cloud.addMotion(U"walk", true);

	startButton->selected = true;

	manager.setChildren
	({
		startButton
	});
}

Map::~Map()
{
	getData().stage = index + 1;
	getData().save();
}

void Map::update()
{
	int32 lastDrawIndex = drawIndex;

	updatePos();

	time.start();

	fairy.update(fairyPos, false);
	snowKnight.update(snowKnightPos,true);
	cookieDoreisho.update(cookieDoreishoPos, true);
	captain.update(captainPos, true);
	lastBoss.update(lastBossPos, true);
	itigoSlave.update(itigoSlavePos, true);

	itigo.update(itigoPos + Vec2{ Periodic::Triangle0_1(15s,time.sF()) * itigoRoadLength,0 }, not Periodic::Square0_1(15s,time.sF()));
	cloud.update(cloudPos + Vec2{ Periodic::Triangle0_1(16s,time.sF()) * itigoRoadLength,0 }, Periodic::Square0_1(16s, time.sF()));


	if(snowKnightKiriageTimer.reachedZero())
	{
		snowKnight.addMotion(U"kiriage");
		snowKnightKiriageTimer.restart();
	}

	if (cookieDoreishoTimer.reachedZero())
	{
		cookieDoreisho.addMotion(U"walk");
		cookieDoreishoTimer.restart();
	}

	if(captainTimer.reachedZero())
	{
		captainTimer.reset();
		captainCloseTimer.restart();
		captain.addMotion(U"Gaaa");
	}

	if(captainCloseTimer.reachedZero())
	{
		captainCloseTimer.reset();
		captainTimer.restart();
		captain.addMotion(U"Tojiru");
	}

	if(lastBossTimer.reachedZero())
	{
		lastBossTimer.reset();
		lastBoss.addMotion(U"ごめんあそばせ");
		lastBossStandTimer.restart();
	}

	if(lastBossStandTimer.reachedZero())
	{
		lastBossStandTimer.reset();
		lastBoss.addMotion(U"Stand");
		lastBossTimer.restart();
	}

	if (not panelFlg&&(getData().menuBackKey.down()|| backButton.leftClicked()))
	{
		changeScene(U"TitleScene");
		AudioAsset{ U"決定ボタン" }.playOneShot();
	}

	//if (not largeFlg)
	//{

	if (panelFlg)
	{
		manager.update();
	}

		if(panelFlg&&getData().menuBackKey.down())
		{
			panelFlg = false;
		}

		if (panelFlg && (startButton->clicked() || getData().menuDecisionKey.down()) && index <= clearStage)
		{

			//AudioAsset{ U"決定ボタン" }.playOneShot();
			AudioAsset{ U"決定ボタンを押す16" }.playOneShot();

			getData().initGame();
			getData().mini_mode = Stage_Mode;
			getData().stage = index + 1;
			getData().stageFile = stageList[index];
			getData().backgroundTexture = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BackgroundTexture"].getString();
			getData().sceneName = sceneNames[index];
			getData().tag = tagList[index];
			getData().text = textList[index];

			if (sceneNames[index] == U"MainGameScene")
			{
				getData().description = U"{5}-ジャンプ\n{1}-左移動\n{3}-右移動\n{2}-しゃがむ\n{4}-攻撃\n{4}長押し-突進(🍪が10個貯まったら)\n------------------------------------\n空中で攻撃：サマーソルトキック\nしゃがみながら攻撃：スライディング\n空中で{2}：ヘッドドロップ";
			}
			else
			{
				getData().description = sentences[index];
			}

			//LoadAsset::RegisterTexture(getData().backgroundTexture);

			String path = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BGM"].getString();

			getData().BGMPath = path;

			//BGMの読み込み
			if (path)
			{
				//if (FileSystem::BaseName(path) == U"StageBossLast1") {
				//	constexpr uint64 sampleRate = 44100;
				//	AudioAsset::Register(path, path, Arg::loopBegin = 22.588 * sampleRate);
				//}
				//else
				//{
					AudioAsset::Register(path, path, Loop::Yes);
				//}
			}

			BGMManager::get().stop();
			changeScene(sceneNames[index]);
		};

		//index
		if (leftInput.down())
		{
			if (0 < index)
			{
				index--;
				//left = true;
				walk = true;
				panelFlg = false;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (rightInput.down())
		{
			if (index != rect_num - 1 && (index < clearStage))
			{
				index++;
				//left = false;
				panelFlg = false;
				AudioAsset{ U"カーソル移動" }.playOneShot();

			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		//if (RectF{ 100, 100,pictures[index].resized(450).size }.leftClicked() && index < clearStage && panelFlg) {
		//	largeFlg = true;
		//}

		if (index == drawIndex)
		{
			if(not standing)
			{
				standing = true;
				character.clearMotion();
				character.addMotion(U"Standing");
			}
		}
		else
		{
			if(standing)
			{
				standing = false;
				character.clearMotion();
				character.addMotion(U"Walk");
			}
		}


		character.update(playerPos - Vec2{ 0,30 }, index < drawIndex);


		{
			Transformer2D target{ MapCamera(playerPos),TransformCursor::Yes };

			for (int i = 0; i < rect_num; i++)
			{
				double r = (i == index) ? rect_size / 2.0 : rect_size / 2.5;

				constexpr Vec2 supplementary{ 0,-5 };

				Vec2 pos = stagePosList[i] + supplementary;

				if (Circle{ pos,r }.mouseOver())
				{

				}

				if (Circle{ pos,r }.leftClicked() && i <= clearStage)
				{
					index = i;
					AudioAsset{ U"決定ボタン" }.playOneShot();
				}
			}
		}

	//}
	//else
	//{
	//	if (MouseL.down() || getData().menuBackKey.down())
	//	{
	//		largeFlg = false;
	//	}
	//}

	if (panelFlg==false&&getData().menuDecisionKey.down())
	{
		AudioAsset{ U"決定ボタン" }.playOneShot();
		panelFlg = true;
	}

}

void Map::draw() const
{
	{
		Transformer2D target{ MapCamera(playerPos) };

		background.resized(Scene::Width()).draw(Arg::bottomRight = Scene::Size());

		for (int i = 0; i < rect_num - 1; i++)
		{
			Line{ stagePosList[i],stagePosList[i + 1] }.draw(2, ColorF{ 0.8 });
		}

		for (int i = 0; i < rect_num; i++)
		{
			ColorF color = Palette::Red;
			if (i < clearStage)
			{
				color = ColorF{ 0.3,0.3,1.0 };
			}
			else if (clearStage < i)
			{
				color = Palette::Gray;
			}

			double r = (i == index) ? rect_size / 2.0 : rect_size / 2.5;

			constexpr Vec2 supplementary{ 0,-5 };

			Vec2 pos = stagePosList[i] + supplementary;


			const double size = typeList[i] == StageType::Boss ? r * 2 + 15 : r * 2;

			stageFrame.resized(size).drawAt(pos);

			stageColor.resized(size).drawAt(pos, color);

			switch (typeList[i])
			{
			case StageType::Stage:
				break;
			case StageType::Boss:
				stageBattle.resized(size).drawAt(pos);
				break;
			case StageType::MiniGame:
				stageGame.resized(r * 2, r * 2 * 0.7).drawAt(pos);
				break;
			default:
				break;
			}

			if (i == index)
			{
				Ellipse{ pos + Vec2{-1,5},r * 1.2,r * 0.7 }.drawFrame(0, 3, Palette::Deeppink);
			}

		}

		if (getData().ChocoMountain - 2 <= getData().clearStage)
		{
			snowKnight.draw();
		}

		if (getData().ChocoMountain + 1 <= getData().clearStage)
		{
			itigoSlave.draw();
		}

		if (2 <= getData().clearStage)
		{
			fairy.draw();
			itigo.draw();
		}

		if (getData().CandyCloud - 2 <= getData().clearStage)
		{
			cookieDoreisho.draw();
		}

		if (getData().CandyCloud  <= getData().clearStage)
		{
			cloud.draw();
		}

		if (getData().LastBossStage - 3 <= getData().clearStage)
		{
			captain.draw();
		}

		if (getData().LastBossStage - 1 <= getData().clearStage)
		{
			lastBoss.draw();
		}


		character.draw();
	}

	if (panelFlg)
	{
		//Rect{ Arg::center(600,300),1100,450 }.draw(ColorF{ Palette::Deeppink,0.8 }).drawFrame(3, 0);
		{
			ScopedShadow s{ shadow };

			if (index + 1 < getData().ChocoMountain)
			{
				snowPanel.resized(1250).drawAt(600, 300);
			}
			else if (index + 1 < getData().CandyCloud)
			{
				chocoPanel.resized(1250).drawAt(600, 300);
			}
			else if (index + 1 < getData().LastBossStage)
			{
				cloudPanel.resized(1250).drawAt(600, 300);
			}
			else
			{
				lastBossPanel.resized(1250).drawAt(600, 300);
			}
		}
		shadow.draw();

		if (index + 1 < getData().ChocoMountain)
		{
			snowPanel.resized(1250).drawAt(600, 300);
		}
		else if (index + 1 < getData().CandyCloud)
		{
			chocoPanel.resized(1250).drawAt(600, 300);
		}
		else if (index + 1 < getData().LastBossStage)
		{
			cloudPanel.resized(1250).drawAt(600, 300);
		}
		else
		{
			lastBossPanel.resized(1250).drawAt(600, 300);
		}

		constexpr RoundRect rrect{ 600 - 10 - 15-5,100,550,325 - 10+5,20 };
		rrect.draw(ColorF{ 0,0.5 }).drawFrame(5, Palette::White);

		FontAsset{ U"NormalFont" }(getData().fmt(sentences[index])).draw(30,600-15, 100+5);
		//start.drawFrame(3);
		//font(U"開始(Enter)").drawAt(start.center(), ColorF{ 1.0, Periodic::Sine0_1(3s) });

		//if (start.mouseOver())
		//{
		//	start.draw(ColorF{ 0,0.1 });
		//	Cursor::RequestStyle(CursorStyle::Hand);
		//}

		pictures[index].resized(460).regionAt(305, rrect.center().y).drawShadow(Vec2{ 0, 0 }, 20, 10,Palette::White);
		pictures[index].resized(460).drawAt(305, rrect.center().y);

		if (getData().clearHPList[index] != 0)
		{
			//Vec2 pos{ 280, 465 + 20-2 };
			const Vec2 pos{ 280, 465 };
			FontAsset{ U"NormalFont" }(U"❤クリアHP：{}"_fmt(getData().clearHPList[index])).regionAt(30, pos).stretched(10,5).draw(ColorF{ 0,0.5 });
			FontAsset{ U"NormalFont" }(U"❤クリアHP：{}"_fmt(getData().clearHPList[index])).drawAt(30, pos, getData().clearHPList[index]==5?Palette::Yellow:Palette::White);
		}

		manager.draw();
	}
	else
	{
		FontAsset{ U"NormalFont" }(title[index]).drawAt(30,Scene::Center() + Vec2{ 0,-300 });

		FontAsset{ U"NormalFont" }(ToKeyName(getData().minigameLeftKey, getData().gamepadMode)).drawAt(30, Scene::Center() + Vec2{ -180,-250 });
		FontAsset{ U"NormalFont" }(ToKeyName(getData().menuDecisionKey, getData().gamepadMode)).drawAt(30, Scene::Center() + Vec2{ 0,-250 });
		FontAsset{ U"NormalFont" }(ToKeyName(getData().minigameRightKey, getData().gamepadMode)).drawAt(30, Scene::Center() + Vec2{ 180,-250 });

		FontAsset{ U"NormalFont" }(U"←").drawAt(20,Scene::Center() + Vec2{ -180,-220 });
		FontAsset{ U"NormalFont" }(U"決定").drawAt(20, Scene::Center() + Vec2{ 0,-220 });
		FontAsset{ U"NormalFont" }(U"→").drawAt(20, Scene::Center() + Vec2{ 180,-220 });

	}
	
	if(not panelFlg)
	{
		if (backButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);
		FontAsset{ U"NormalFont" }(U"{}-タイトルに戻る"_fmt(ToKeyName(getData().menuBackKey, getData().gamepadMode))).draw(30, Arg::leftCenter = backButton.center + Vec2{ 30,0 });
	}
	else
	{
		const String explanation = U" {}-はじめる  {}-閉じる"_fmt(ToKeyName(getData().menuDecisionKey, getData().gamepadMode), ToKeyName(getData().menuBackKey, getData().gamepadMode));
		FontAsset{ U"NormalFont" }(explanation).region(30, Arg::bottomLeft(5, Scene::Height())).stretched(5, 3).draw(ColorF{ 0,0.5 });
		FontAsset{ U"NormalFont" }(explanation).draw(30, Arg::bottomLeft(5, Scene::Height()));
	}

	//if (largeFlg) {
	//	Rect{ Scene::Size() }.draw(ColorF(0, 0.6));
	//	pictures[index].resized(1000).drawAt(Scene::Center());
	//}
}
