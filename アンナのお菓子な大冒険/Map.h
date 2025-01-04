#pragma once
#include"Common.h"
#include"LoadAsset.h"
#include"LinerMove.h"


static Mat3x2 MapCamera(const Vec2& playerPos)
{
	const Vec2 pos = Scene::Center() + Vec2{ 0,300 };
	double x = -Clamp<double>((playerPos-pos).x, -200, 200);
	double y = -Clamp<double>((playerPos-pos).y, -230, 999999999999);
	return Mat3x2::Translate(x,y).scaled(1.5, pos);
}



class Map : public App::Scene
{
public:

	enum class StageType {
		Stage,Boss,MiniGame
	};


	Texture background{U"BackGroundTexture/ステージセレクト.png"};

	Texture homeIcon{ 0xf015_icon,40 };

	//フォント
	const Font font{ 30 };

	//画面下の四角形の情報
	const int rect_size = 40;//四角形の大きさ
	const int rect_num = 22;//四角形(ゲーム)の個数
	const int rect_gap = 3;//四角形の間隔

	static constexpr Circle backButton{ {35,35},30 };

	//ボタン(開始、イージー、ノーマル、ハード)
	const Rect start{ Arg::center(600,470), 300, 70 };

	//JSONファイル
	JSON json = JSON::Load(U"map.json");
	JSON saveDatajson = JSON::Load(U"saveData.json");

	//各ゲームのデータ
	Array<Texture>pictures;//各ゲームの大きい画像
	Array<String>sentences;//各ゲームの説明文
	Array<String>sceneNames;
	Array<Point>stagePosList;
	Array<String>title;
	Array<String>stageList;
	Array<StageType>typeList;

	//どのゲームを選択しているか
	int index = 0;

	//写真を拡大
	bool largeFlg = false;

	int32 clearStage = 0;

	bool left = false;

	bool walk = false;

	bool panelFlg = false;


	CharacterSystem character{ U"CharacterImages/annna/annna.json",U"CharacterImages/annna/motion.txt",0.25/2,{0,0},false};

	int32 drawIndex = 0;

	Vec2 playerPos{};


	Texture stageFrame{ U"StageImage/stageFrame.png", TextureDesc::Mipped };
	Texture stageBattle{ U"StageImage/stageBattle.png", TextureDesc::Mipped };
	Texture stageColor{ U"StageImage/stageColor.png", TextureDesc::Mipped };
	Texture stageGame{ U"StageImage/stageGame.png", TextureDesc::Mipped };

	void updatePos()
	{
		Vec2 vec{};

		size_t nextIndex = 0;

		if (drawIndex < index)
		{
			nextIndex = 1;
		}

		if (index<drawIndex)
		{
			nextIndex = -1;
		}

		Vec2 nextPos = stagePosList[drawIndex +nextIndex];
		if ((nextPos -playerPos).length()<0.1)
		{
			drawIndex += nextIndex;
			return;
		}

		playerPos=Math::SmoothDamp(playerPos, nextPos, vec, 0.001,5000);
	}
	
	Map(const InitData& init) : IScene{ init } {
		Scene::SetBackground(Palette::Skyblue);

		clearStage = saveDatajson[U"ClearStage"].get<int32>();

		if (getData().mini_clear == true && clearStage< getData().stage) {
			saveDatajson[U"ClearStage"] = getData().stage;
			saveDatajson.save(U"saveData.json");
			clearStage = getData().stage;
		}

		if (getData().backFromMainGameScene)
		{
			if (getData().mini_clear|| getData().quitStage)
			{
				saveDatajson[U"MaxHP"][getData().stage - 1] = 5;
			}
			else
			{
				saveDatajson[U"MaxHP"][getData().stage - 1] = getData().maxHP + 2;
			}

			saveDatajson.save(U"saveData.json");
		}



		index = getData().stage - 1;
		drawIndex = index;

		for (int i = 0; i < rect_num; i++) {
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
		character.update(playerPos -Point{ 0 ,30 }, false);


		AudioAsset::Register(U"WorldBGM",U"BGM/WorldMap2.mp3",Loop::Yes);
		BGMManager::get().play(U"WorldBGM");
	}

	void update() override
	{
		updatePos();

		if (not largeFlg) {

			if (backButton.leftClicked())
			{
				changeScene(U"TitleScene");
			}

			if (panelFlg&&(start.leftClicked()||KeyEnter.down()) && index<=clearStage) {
				getData().mini_mode = Stage_Mode;
				getData().stage = index + 1;
				getData().mini_clear = false;
				getData().stageFile = stageList[index];
				getData().backgroundTexture = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BackgroundTexture"].getString();
				getData().maxHP = saveDatajson[U"MaxHP"][index].get<int32>();
				getData().backFromMainGameScene = false;
				getData().quitStage = false;

				LoadAsset::RegisterTexture(getData().backgroundTexture);

				String path = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BGM"].getString();

				getData().BGMPath = path;

				//BGMの読み込み
				if (path) {
					if (FileSystem::BaseName(path) == U"StageBossLast1") {
						constexpr uint64 sampleRate = 44100;
						AudioAsset::Register(path, path, Arg::loopBegin = 22.588* sampleRate);
					}
					else {
						AudioAsset::Register(path, path, Loop::Yes);
					}
				}

				BGMManager::get().stop();
				changeScene(sceneNames[index]);
			};

			//index
			if (((KeyLeft | KeyA).down()) && index != 0) {
				index--;
				left = true;
				walk = true;
				panelFlg = false;
			}
			if (((KeyRight | KeyD).down()) && index != rect_num - 1 && (index < clearStage)) {
				index++;
				left = false;
				panelFlg = false;
			}

			if (RectF{ 100, 100,pictures[index].resized(450).size }.leftClicked() && index<clearStage) {
				largeFlg = true;
			}

			character.update(playerPos-Vec2{0,30}, left);
				

		}
		else if (MouseL.down())largeFlg = false;

		if (KeyEnter.down())
		{
			panelFlg = true;
		}
	}

	void draw() const override
	{
		{
			Transformer2D target{ MapCamera(playerPos) };

			background.resized(Scene::Width()).draw(Arg::bottomRight = Scene::Size());

			for (int i = 0; i < rect_num-1; i++)
			{
				Line{ stagePosList[i],stagePosList[i + 1] }.draw(2,ColorF{0.8});
			}

			for (int i = 0; i < rect_num; i++)
			{
				ColorF color = Palette::Red;
				if (i < clearStage)
				{
					color = ColorF{ 0.3,0.3,1.0 };
				}
				else if (clearStage<i)
				{
					color = Palette::Gray;
				}

				double r = (i == index)?rect_size / 2.0: rect_size / 2.5;

				constexpr Vec2 supplementary{0,-5};

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
					stageGame.resized(r * 2).drawAt(pos);
					break;
				default:
					break;
				}
			}
			character.draw();
		}

		if (panelFlg)
		{
			Rect{ Arg::center(600,300),1100,450 }.draw(ColorF{ Palette::Deeppink,0.8 }).drawFrame(3, 0);
			font(sentences[index]).draw(600, 100);
			start.drawFrame(3);
			font(U"開始").drawAt(start.center(), ColorF{ 1.0, Periodic::Sine0_1(3s) });

			if(start.mouseOver())
			{
				start.draw(ColorF{ 0,0.1 });
				Cursor::RequestStyle(CursorStyle::Hand);
			}

			pictures[index].resized(450).draw(100, 100);
		}
		else
		{
			font(title[index]).drawAt(Scene::Center() + Vec2{0,-300});
			font(U"← [A]              [Enter]              [D] →").drawAt(Scene::Center() + Vec2{0,-250});

		}

		if (backButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
		homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);


		if (largeFlg) {
			Rect{ Scene::Size() }.draw(ColorF(0, 0.6));
			pictures[index].resized(1000).drawAt(Scene::Center());
		}
	}
};
