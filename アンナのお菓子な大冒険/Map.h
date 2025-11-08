#pragma once
#include"Common.h"
#include"LoadAsset.h"
#include"LinerMove.h"
#include"CharacterSystem.h"
#include"BGMManager.hpp"
#include"Shadow.h"

class Map : public App::Scene
{
public:

	enum class StageType
	{
		Stage,Boss,MiniGame
	};

	static Mat3x2 MapCamera(const Vec2& playerPos)
	{
		const Vec2 pos = Scene::Center() + Vec2{ 0,300 };
		double x = -Clamp<double>((playerPos - pos).x, -200, 200);
		double y = -Clamp<double>((playerPos - pos).y, -230, 999999999999);
		return Mat3x2::Translate(x, y).scaled(1.5, pos);
	}

	Texture background{U"BackGroundTexture/ステージセレクト.png"};

	Texture homeIcon{ 0xf015_icon,40 };

	//フォント
	//const Font font{ 30 };

	//画面下の四角形の情報
	const int rect_size = 40;//四角形の大きさ
	const int rect_num = getData().LastBossStage;//四角形(ゲーム)の個数
	const int rect_gap = 3;//四角形の間隔

	static constexpr Circle backButton{ {35,35},30 };

	//ボタン(開始、イージー、ノーマル、ハード)
	//const Rect start{ Arg::center(600,470), 300, 70 };

	//JSONファイル
	JSON json = JSON::Load(U"map.json");

	//各ゲームのデータ
	Array<Texture>pictures;//各ゲームの大きい画像
	Array<String>sentences;//各ゲームの説明文
	Array<String>sceneNames;
	Array<Point>stagePosList;
	Array<String>title;
	Array<String>stageList;
	Array<StageType>typeList;
	Array<String>tagList;
	Array<String>textList;

	//どのゲームを選択しているか
	int index = 0;

	//写真を拡大
	bool largeFlg = false;

	int32 clearStage = 0;

	//bool left = false;

	bool walk = false;

	bool panelFlg = false;

	bool standing = true;

	LongPressInput leftInput{ getData().minigameLeftKey,ControllerManager::Direction::Left };
	LongPressInput rightInput{ getData().minigameRightKey,ControllerManager::Direction::Right };


	CharacterSystem character{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.25/2,{0,0},false};

	int32 drawIndex = 0;

	Vec2 playerPos{};

	Shadow shadow;

	Texture stageFrame{ U"StageImage/stageFrame.png", TextureDesc::Mipped };
	Texture stageBattle{ U"StageImage/stageBattle.png", TextureDesc::Mipped };
	Texture stageColor{ U"StageImage/stageColor.png", TextureDesc::Mipped };
	Texture stageGame{ U"StageImage/stageGame.png", TextureDesc::Mipped };

	static constexpr Vec2 fairyPos{ 80,700 - 30 };
	CharacterSystem fairy{ U"Characters/yousei1/yousei.json",U"Characters/yousei1/motion.txt",0.25 * 0.5,fairyPos,true };

	static constexpr Vec2 snowKnightPos{ 233+15,610 - 40+5 };
	CharacterSystem snowKnight{ U"Characters/yukidarunaito/yukidarunaito.json" ,U"Characters/yukidarunaito/motion.txt" ,0.2 * 0.2,snowKnightPos,true,false };
	Timer snowKnightKiriageTimer{ 10s };

	static constexpr Vec2 cookieDoreishoPos{ 460+2+15,480 - 25 };
	CharacterSystem cookieDoreisho{ U"Characters/cookieDoreisho/model.json" ,U"Characters/cookieDoreisho/motion.txt" ,0.4*0.4,cookieDoreishoPos,true,false };
	Timer cookieDoreishoTimer{ 4s };

	static constexpr Vec2 captainPos{ 930-2+15-15,490 - 32-60 };
	CharacterSystem captain{ U"Characters/sentyo/model.json" ,U"Characters/sentyo/motion.txt" ,1 * 0.2,captainPos,true,false };
	Timer captainTimer{ 8s };
	Timer captainCloseTimer{ 4s };

	static constexpr Vec2 lastBossPos{ 1050+15,300 - 35 };
	CharacterSystem lastBoss{ U"Characters/bitter/model1.json" ,U"Characters/bitter/motion1.txt" ,0.3*0.5,lastBossPos,false,false };
	Timer lastBossTimer{ 6s };
	Timer lastBossStandTimer{ 4s };

	static constexpr Vec2 itigoSlavePos{ 490,660 };
	CharacterSystem itigoSlave{ U"Characters/itigoSlave/itigoSlave.json" ,U"Characters/itigoSlave/motion.txt" ,0.3*0.4,itigoSlavePos,true,false };

	static constexpr Vec2 itigoPos{ 490-50,680-5 };
	static constexpr double itigoRoadLength = 180;
	CharacterSystem itigo{ U"Characters/itigo/itigo.json" ,U"Characters/itigo/motion.txt" ,0.3*0.4,itigoPos,true,false };
	Stopwatch time;

	static constexpr Vec2 cloudPos{ 730,490-120 };
	static constexpr double cloudRoadLength = 250;
	CharacterSystem cloud{ U"Characters/cloud/cloud.json" ,U"Characters/cloud/motion.txt" ,0.5*0.4,cloudPos,true,false };

	Texture snowPanel{ U"StageImage/雪原パネル.png" };
	Texture chocoPanel{ U"StageImage/チョコパネル.png" };
	Texture cloudPanel{ U"StageImage/雲パネル.png" };
	Texture lastBossPanel{ U"StageImage/ラスボスパネル.png" };

	UIManager manager;

	std::shared_ptr<ChocolateButton> startButton = ChocolateButton::Create
	({
		.color = Palette::Orange,
		.width = 280,
		.height = 70,
		.relative = Relative{0.5,0.595},
		.child = TextUI::Create
		({
			.text = U"\U000F040A はじめる",
			.fontSize = 40,
			.color = Palette::White
		})
	});

	void updatePos();
	
	Map(const InitData& init);

	~Map();

	void update() override;

	void draw() const override;

	void drawCharacters()const;
};
