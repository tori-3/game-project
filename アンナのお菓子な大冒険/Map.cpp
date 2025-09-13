#include"Map.h"

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

	Vec2 nextPos = stagePosList[drawIndex + nextIndex];

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

	playerPos = stagePosList[index];
	character.addMotion(U"Walk");
	character.update(playerPos - Point{ 0 ,30 }, false);


	AudioAsset::Register(U"WorldBGM", U"BGM/WorldMap2.mp3", Loop::Yes);
	BGMManager::get().play(U"WorldBGM");


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

	if (getData().menuBackKey.down())
	{
		changeScene(U"TitleScene");
		AudioAsset{ U"決定ボタン" }.playOneShot();
	}

	if (not largeFlg)
	{

		if (backButton.leftClicked())
		{
			changeScene(U"TitleScene");
			AudioAsset{ U"決定ボタン" }.playOneShot();
		}

		if (panelFlg && (start.leftClicked() || getData().menuDecisionKey.down()) && index <= clearStage)
		{

			AudioAsset{ U"決定ボタン" }.playOneShot();

			getData().mini_mode = Stage_Mode;
			getData().stage = index + 1;
			//getData().mini_clear = false;
			getData().stageFile = stageList[index];
			getData().backgroundTexture = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BackgroundTexture"].getString();
			//getData().maxHP = saveDatajson[U"MaxHP"][index].get<int32>();
			getData().backFromMainGameScene = false;
			getData().quitStage = false;
			getData().sceneName = sceneNames[index];

			if (sceneNames[index] == U"MainGameScene")
			{
				getData().description = U"Space or W：ジャンプ\nA：左\nD：右\nS：しゃがむ\nEnter：技を発動\nEnter長押し：突進(クッキーが10個貯まったら)";
			}
			else
			{
				getData().description = sentences[index];
			}

			LoadAsset::RegisterTexture(getData().backgroundTexture);

			String path = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BGM"].getString();

			getData().BGMPath = path;

			//BGMの読み込み
			if (path)
			{
				if (FileSystem::BaseName(path) == U"StageBossLast1") {
					constexpr uint64 sampleRate = 44100;
					AudioAsset::Register(path, path, Arg::loopBegin = 22.588 * sampleRate);
				}
				else
				{
					AudioAsset::Register(path, path, Loop::Yes);
				}
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
				left = true;
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
				left = false;
				panelFlg = false;
				AudioAsset{ U"カーソル移動" }.playOneShot();

			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (RectF{ 100, 100,pictures[index].resized(450).size }.leftClicked() && index < clearStage && panelFlg) {
			largeFlg = true;
		}

		character.update(playerPos - Vec2{ 0,30 }, left);


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

	}
	else if (MouseL.down())largeFlg = false;

	if (getData().menuDecisionKey.down())
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


			stageFrame.resized(r * 2).drawAt(pos);

			stageColor.resized(r * 2).drawAt(pos, color);

			switch (typeList[i])
			{
			case StageType::Stage:
				break;
			case StageType::Boss:
				stageBattle.resized(r * 2).drawAt(pos);
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
		fairy.draw();
		snowKnight.draw();
		cookieDoreisho.draw();
		captain.draw();
		lastBoss.draw();
		itigoSlave.draw();
		itigo.draw();
		cloud.draw();
		character.draw();
	}

	if (panelFlg)
	{
		Rect{ Arg::center(600,300),1100,450 }.draw(ColorF{ Palette::Deeppink,0.8 }).drawFrame(3, 0);
		font(sentences[index]).draw(600, 100);
		start.drawFrame(3);
		font(U"開始(Enter)").drawAt(start.center(), ColorF{ 1.0, Periodic::Sine0_1(3s) });

		if (start.mouseOver())
		{
			start.draw(ColorF{ 0,0.1 });
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		pictures[index].resized(450).draw(100, 100);
	}
	else
	{
		font(title[index]).drawAt(Scene::Center() + Vec2{ 0,-300 });
		font(U"← [A]              [Enter]              [D] →").drawAt(Scene::Center() + Vec2{ 0,-250 });

	}

	if (backButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}
	homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);
	FontAsset{ U"NormalFont" }(U"[Q]タイトルに戻る").draw(Arg::leftCenter = backButton.center + Vec2{ 30,0 });

	if (largeFlg) {
		Rect{ Scene::Size() }.draw(ColorF(0, 0.6));
		pictures[index].resized(1000).drawAt(Scene::Center());
	}
}
