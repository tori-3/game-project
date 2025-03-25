#pragma once
#include"Common.h"
#include"LoadAsset.h"
#include"LinerMove.h"
#include"CharacterSystem.h"
#include"BGMManager.hpp"


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
	const Font font{ 30 };

	//画面下の四角形の情報
	const int rect_size = 40;//四角形の大きさ
	const int rect_num = 23;//四角形(ゲーム)の個数
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

	LongPressInput leftInput{ getData().KeyLeft };
	LongPressInput rightInput{ getData().KeyRight };


	CharacterSystem character{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.25/2,{0,0},false};

	int32 drawIndex = 0;

	Vec2 playerPos{};


	Texture stageFrame{ U"StageImage/stageFrame.png", TextureDesc::Mipped };
	Texture stageBattle{ U"StageImage/stageBattle.png", TextureDesc::Mipped };
	Texture stageColor{ U"StageImage/stageColor.png", TextureDesc::Mipped };
	Texture stageGame{ U"StageImage/stageGame.png", TextureDesc::Mipped };

	static constexpr Vec2 fairyPos{ 80,700 - 30 };
	CharacterSystem fairy{ U"Characters/yousei1/yousei.json",U"Characters/yousei1/motion.txt",0.25 * 0.5,fairyPos,true };

	static constexpr Vec2 snowKnightPos{ 233,610 - 40 };
	CharacterSystem snowKnight{ U"Characters/yukidarunaito/yukidarunaito.json" ,U"Characters/yukidarunaito/motion.txt" ,0.2 * 0.2,snowKnightPos,true,false };
	Timer snowKnightKiriageTimer{ 10s };

	static constexpr Vec2 cookieDoreishoPos{ 460+2,480 - 25 };
	CharacterSystem cookieDoreisho{ U"Characters/cookieDoreisho/model.json" ,U"Characters/cookieDoreisho/motion.txt" ,0.4*0.4,cookieDoreishoPos,true,false };
	Timer cookieDoreishoTimer{ 4s };

	static constexpr Vec2 captainPos{ 930-2,490 - 32 };
	CharacterSystem captain{ U"Characters/sentyo/model.json" ,U"Characters/sentyo/motion.txt" ,1 * 0.2,captainPos,true,false };
	Timer captainTimer{ 8s };
	Timer captainCloseTimer{ 4s };

	static constexpr Vec2 lastBossPos{ 1050,300 - 35 };
	CharacterSystem lastBoss{ U"Characters/bitter/model1.json" ,U"Characters/bitter/motion1.txt" ,0.3*0.5,lastBossPos,false,false };
	Timer lastBossTimer{ 6s };
	Timer lastBossStandTimer{ 4s };





	void updatePos();
	
	Map(const InitData& init);

	~Map();

	void update() override;

	void draw() const override;
};
