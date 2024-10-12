#pragma once
#include"Common.h"

class Map : public App::Scene
{
public:

	Texture background{U"背景.png"};

	//フォント
	const Font font{ 30 };

	//画面下の四角形の情報
	const int rect_size = 60;//四角形の大きさ
	const int rect_num = 18;//四角形(ゲーム)の個数
	const int rect_gap = 5;//四角形の間隔

	//アイコン画像
	const Texture Lock_icon{ 0xf023_icon, 120 };//鍵の画像
	const Texture Back_icon{ 0xf0e2_icon, 40 };//戻る矢印の画像

	//ボタンの座標(戻る、戻る、開始)
	const Point back_pos{ 35, 35 };

	//ボタン(開始、イージー、ノーマル、ハード)
	const Rect start{ Arg::center(600,470), 300, 70 };

	//JSONファイル
	JSON json = JSON::Load(U"map.json");

	//各ゲームのデータ
	Array<Texture>pictures;//各ゲームの大きい画像
	Array<String>sentences;//各ゲームの説明文
	//Array<bool>clear;
	Array<String>sceneNames;

	//どのゲームを選択しているか
	int index = 0;

	//写真を拡大
	bool largeFlg = false;

	int32 clearStage = 0;

	bool left = false;

	bool walk = false;

	CharacterSystem character{ U"CharacterImages/annna/annna.json",U"CharacterImages/annna/motion.txt",0.25,{0,0},false};

	Map(const InitData& init) : IScene{ init } {
		Scene::SetBackground(Palette::Skyblue);

		clearStage=json[U"ClearStage"].get<int32>();

		if (getData().mini_clear == true && clearStage< getData().stage) {
			json[U"ClearStage"] = getData().stage;
			json.save(U"map.json");
			clearStage = getData().stage;
		}


		if (getData().backFromMainGameScene)
		{
			if (getData().mini_clear)
			{
				json[U"StageData"][U"Stage{}"_fmt(getData().stage)][U"MaxHP"] = 5;				

			}
			else
			{
				json[U"StageData"][U"Stage{}"_fmt(getData().stage)][U"MaxHP"] = getData().maxHP + 2;
			}

			json.save(U"map.json");
		}



		index = getData().stage - 1;

		for (int i = 0; i < rect_num; i++) {
			//各ステージの読み込み
			String str{ U"Stage{}"_fmt(i + 1) };
			pictures << Texture{ json[U"StageData"][str][U"Picture"].get<String>() };
			sentences << json[U"StageData"][str][U"Sentence"].get<String>();
			sceneNames << json[U"StageData"][str][U"SceneName"].get<String>();
		}

		character.addMotion(U"Walk");
		character.update({ rect_x(0) ,640 }, false);
		//Motion motion;
		//motion.add(new SetPos{ U"body",Vec2{ rect_x(index) ,640 },0 });
		//character.character.addMotion(U"Move", motion);
		//character.update({ rect_x(0) ,640 }, false);


		AudioAsset::Register(U"WorldBGM",U"BGM/WorldMap2.mp3",Loop::Yes);
		BGMManager::get().play(U"WorldBGM");
	}

	void update() override
	{

		if (not largeFlg) {

			if (Circle(back_pos, 25).leftClicked()) {
				getData().mini_mode = Stage_Mode;//念のため
				getData().stage = 1;//念のため
				getData().mini_clear = false;//念のため
				changeScene(U"Start");
			}

			if (start.leftClicked() && index<=clearStage) {
				getData().mini_mode = Stage_Mode;
				getData().stage = index + 1;
				getData().mini_clear = false;
				getData().stageFile = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"MapFile"].getString();
				getData().backgroundTexture = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BackgroundTexture"].getString();
				getData().maxHP = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"MaxHP"].get<int32>();


				getData().backFromMainGameScene = false;

				if (not TextureAsset::IsRegistered(getData().backgroundTexture))
				{
					TextureAsset::Register(getData().backgroundTexture, getData().backgroundTexture);
				}

				String path = json[U"StageData"][U"Stage{}"_fmt(index + 1)][U"BGM"].getString();

				//BGMの読み込み
				if (path) {
					if (FileSystem::BaseName(path) == U"StageBossLast1") {
						constexpr uint64 sampleRate = 44100;
						AudioAsset::Register(U"Stage{}"_fmt(index + 1), path, Arg::loopBegin = 22.588* sampleRate);
					}
					else {
						AudioAsset::Register(U"Stage{}"_fmt(index + 1), path, Loop::Yes);
					}
				}

				BGMManager::get().stop();
				changeScene(sceneNames[index]);
			};

			//index
			if (((KeyLeft | KeyA).down()) && index != 0) {
				index--;
				//Motion motion;
				//motion.add(new SetPos{ U"body",Vec2{ rect_x(index) ,640 },1 });
				//character.character.addMotion(U"Move", motion);
				left = true;
				walk = true;
			}
			if (((KeyRight | KeyD).down()) && index != rect_num - 1 && (index < clearStage)) {
				index++;
				//Motion motion;
				//motion.add(new SetPos{ U"body",Vec2{ rect_x(index) ,640 },1 });
				//character.character.addMotion(U"Move", motion);
				left = false;
			}

			//for (int i : step(rect_num)) {
			//	if (Rect{ Arg::center(rect_x(i),700), rect_size }.leftClicked()) {
			//		index = i;
			//	}
			//}

			if (RectF{ 100, 100,pictures[index].resized(450).size }.leftClicked() && index<clearStage) {
				largeFlg = true;
			}

			character.update({ rect_x(index) ,640 }, left);
				

		}
		else if (MouseL.down())largeFlg = false;
	}

	void draw() const override
	{
		background.resized(Scene::Width()).draw(Arg::bottomRight=Scene::Size());

		Rect{ Arg::center(600,300),1100,450 }.draw(ColorF{ Palette::Deeppink,0.8 }).drawFrame(3, 0);
		//Back_icon.drawAt(back_pos);

		if (index<=clearStage) {
			font(sentences[index]).draw(600, 100);
			start.drawFrame(3);
			font(U"開始").drawAt(start.center(), ColorF{ 1.0, Periodic::Sine0_1(3s) });
			pictures[index].resized(450).draw(100, 100);
		}
		else {
			Lock_icon.drawAt(600, 300, Palette::White);
		}

		font(U"A ←").drawAt(50, 700);
		font(U"D →").drawAt(1150, 700);
		for (int i = 0; i < rect_num; i++) {
			if (i == clearStage) {
				if (i == index) {
					Circle{ Arg::center(rect_x(i), 700) ,rect_size / 3 }.draw(Palette::Red).drawFrame(5);
				}
				else {
					Circle{ Arg::center(rect_x(i), 700) ,rect_size / 4 }.draw(Palette::Red).drawFrame(5);
				}
			}
			else if (i<=clearStage) {
				if (i == index) {
					Circle{ Arg::center(rect_x(i), 700) ,rect_size / 3 }.draw(Palette::Blue).drawFrame(5);
				}
				else {
					Circle{ Arg::center(rect_x(i), 700) ,rect_size / 4 }.draw(Palette::Blue).drawFrame(5);
				}
			}
			else {
					Circle{ Arg::center(rect_x(i), 700) ,rect_size / 4 }.draw(Palette::Black).drawFrame(5);
			}
		}

		character.draw();

		if (largeFlg) {
			Rect{ Scene::Size() }.draw(ColorF(0, 0.6));
			pictures[index].resized(1000).drawAt(Scene::Center());
		}
	}

private:
	int rect_x(int i)const {
		int gap = rect_size + rect_gap;
		return  600 - (rect_num - 1) * gap / 2 + i * gap;
	}
};
