#pragma once
#include"Common.h"
#include"Enemy.h"
#include"setting.h"
#include"Pause.h"
#include"Player.h"
#include"LastBoss.h"
#include"Spawner.h"
#include"BackGround.h"
#include"BGMManager.hpp"

inline Mat3x2 shakeMat(const Timer& timer, double amplitude = 20) {
	constexpr double N = 2;//揺れの回数
	return Mat3x2::Translate(amplitude * (Periodic::Sine0_1(timer.duration() / N, timer.sF()) - 1 / N), 0);
}

class EnemyAdder {
public:

	EntityManager* manager;

	EnemyAdder(EntityManager* manager) :manager{manager} {
		table[U"StrawberrySoldier"] = [](const Vec2& pos) {return new StrawberrySoldier{ pos }; };
		table[U"CookieSoldier"] = [](const Vec2& pos) {return new CookieSoldier{ pos }; };
		table[U"SpawnerStrawberrySoldier"] = [](const Vec2& pos) {return new Spawner{ pos,new StrawberrySoldier{pos} }; };
		table[U"Player"] = [](const Vec2& pos) {return new Player{ pos }; };
		table[U"Snowman"] = [](const Vec2& pos) {return new Snowman{ pos }; };
		table[U"ItigoSlave"] = [](const Vec2& pos) {return new ItigoSlave{ pos }; };
		table[U"CloudEnemy"] = [](const Vec2& pos) {return new CloudEnemy{ pos }; };
		table[U"Corn"] = [](const Vec2& pos) {return new Corn{ pos }; };
		table[U"CookieMuti"] = [](const Vec2& pos) {return new CookieMuti{ pos }; };
		table[U"CookieKaban"] = [](const Vec2& pos) {return new CookieKaban{ pos }; };
		table[U"SnowKnight"] = [](const Vec2& pos) {return new SnowKnight{ pos }; };
		table[U"Zerosen"] = [](const Vec2& pos) {return new Zerosen{ pos }; };
		table[U"SlaversCookie"] = [](const Vec2& pos) {return new SlaversCookie{ pos }; };
		table[U"Captain"] = [](const Vec2& pos) {return new Captain{ pos }; };
		table[U"Door"] = [](const Vec2& pos) {return new GoalDoor{ pos }; };
		table[U"LastBoss"] = [](const Vec2& pos) {return new LastBoss{ pos }; };
		table[U"FallingRocks"] = [](const Vec2& pos) {return new FallingRocks{ pos }; };
		table[U"RollingRocks"] = [](const Vec2& pos) {return new RollingRocks{ pos }; };
	}

	HashTable<String, std::function<Entity* (const Vec2&)>>table;

	void update() {
		for (auto& a : DataManager::get().list) {
			manager->add(table[a.first](a.second));
		}
		DataManager::get().list.clear();
	}
};

class SmoothCamera {
public:

	Vec2 pos;

	double vel=0;

	double stageHeight = 0;

	double stageWidth = 0;

	Timer shakeTimer{ 2s };//カメラの揺れ

	void setPos(const Vec2& _pos) {
		pos = _pos;
	}

	void setStageHeight(double h) {
		stageHeight = h;
	}

	void setStageWidth(double w) {
		stageWidth = w;
	}


	void update(const Vec2& _pos) {

		pos = _pos;

		pos.x = Clamp<double>(pos.x, 0, stageWidth - Scene::Width());

		pos.y = Clamp<double>(pos.y,0, stageHeight-Scene::Height());

		if (DataManager::get().table.contains(U"ShakeCamera")) {
			DataManager::get().table.erase(U"ShakeCamera");
			shakeTimer.restart();
		}

	}

	Mat3x2 getMat3x2()const {
		return Mat3x2::Translate(-pos)* shakeMat(shakeTimer, shakeTimer.sF()*50);
	}
};

//本当はcppですべきなんだろうなー
DataManager* DataManager::instance = nullptr;

class MainGameScene : public IPauseScene
{
public:
	void cookieDisplay(int32 count,double tame)const {

		if (not TextureAsset::IsRegistered(U"CookieItem")) {
			TextureAsset::Register(U"CookieItem", U"🍪"_emoji);
		}

		for (auto i : step(10)) {

			if ((i + 1) <= tame * 10) {
				Circle{ 25 + i * 40, 75,20 }.drawShadow(Vec2{ 0, 0 }, 8, 2,HSV{ 360 / 10 * i, 0.7 });
			}

			if (i < count) {
				TextureAsset(U"CookieItem").resized(40).drawAt(25+  i * 40, 75);
			}
			else {
				TextureAsset(U"CookieItem").resized(40).drawAt(25+  i * 40, 75, ColorF{ 1,0.3 });
			}

			if ((i + 1) <= tame * 10) {
				Circle{ 25 + i * 40, 75,20 }.draw(HSV{ 360 / 10 * i, 0.2});
			}
		}
	}

	void hpDisplay(int32 count)const {

		if (not TextureAsset::IsRegistered(U"HP")) {
			TextureAsset::Register(U"HP", U"❤"_emoji);
		}

		for (auto i : step(5)) {
			if (i < count) {
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25);
			}
			else {
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25, ColorF{ 1,0.3 });
			}
		}
	}

	DataManagerStart start;

	JSON json = JSON::Load(getData().stageFile);
	Stage stage{ json };
	StageBackGroundManager backgroundManager{ json };

	MSRenderTexture rTexture{ Scene::Size(),ColorF{0,0} };

	//Player player{ Point(500, 350 + 70) };
	Background background{ U"雪原背景.png" ,stage.width() };
	//Background background2{ U"お菓子の背景.png" ,stage.width(),2 };

	EntityManager manager;

	EnemyAdder adder{ &manager };

	//Player player{ Point(500, 350 + 70) };

	CloudManager cloud;

	//	ColorF skyColor{ 0.8, 0.9, 1.0};
	ColorF skyColor{Palette::Skyblue};

	Player* player=nullptr;

	SmoothCamera camera;

	double startY = 0;




	//テスト
	//StageBackGround stageBackGround{U"backGround.json"};


	const RenderTexture backGroundTexture{ Scene::Size()};
	const RenderTexture internalTexture{ backGroundTexture.size() };
	const RenderTexture renderTexture{ backGroundTexture.size() };

	void loadAudio() {

		for (auto& path : FileSystem::DirectoryContents(U"Audio"))
		{
			String name = FileSystem::BaseName(path);
			AudioAsset::Register(name, path);
			AudioAsset::LoadAsync(name);
		}

		AudioAsset{ U"足音" }.setVolume(3);

	}



	// コンストラクタ（必ず実装）
	MainGameScene(const InitData& init)
		: IPauseScene{ init }
	{
		loadAudio();

		adder.update();
		player = dynamic_cast<Player*>(manager.get(U"Player"));
		player->setDataP(&getData());

		stage.update(player->pos);
		adder.update();
		manager.stage = &stage;
		startY = player->pos.y;


		camera.setStageWidth(stage.width() * rect_size);
		camera.setStageHeight(stage.height()*rect_size);
		camera.update({ player->pos.x - draw_x,player->pos.y - draw_y });
	}

	bool bgmStart = false;

	// 更新関数（オプション）
	void gameUpdate() override
	{

		if (not bgmStart) {
			BGMManager::get().play(U"Stage{}"_fmt(getData().stage));
			bgmStart = true;
		}
		ClearPrint();
		DataManager::get().playerPos = player->pos;

		adder.update();

		//座標変換(カーソルだけ)
		//double hieght = Scene::Size().y - draw_y;
		////if (player.pos.y < change_hieght)hieght += change_hieght - player.pos.y;
		//Mat3x2 mat = Mat3x2::Translate(Vec2{ draw_x - player->pos.x, hieght });
		//const Transformer2D transformer{ Mat3x2::Identity(),mat };
		{
			const Transformer2D t{ Mat3x2::Identity(),camera.getMat3x2() };
			manager.update();
		}

		////落下したらスタートに戻す。(デバッグ用)
		if (not DataManager::get().playerAlive /* player->hp <= 0|| 1000 < player->pos.y*/) {

			if (DataManager::get().table.contains(U"Clear")) {
				EndGame(true);
				for (const auto& enemy : manager.getArray(U"Enemy")) {
					enemy->kill();
				}

			}
			else {
				EndGame(false);
			}
		}

		stage.update(player->pos);

		DataManager::get().talkWindow.update(MouseL.down());

		cloud.update();

		if (KeyEscape.down())goPause();

		if(DataManager::get().cameraPos){
			camera.update(DataManager::get().cameraPos.value());
		}
		else {
			camera.update({ player->pos.x - draw_x,player->pos.y - draw_y });
		}


		backgroundManager.update(camera.pos);
	}

	// 描画関数（オプション）
	void gameDraw() const override
	{
		Scene::SetBackground(skyColor);

		cloud.draw1();
		background.draw(camera.pos+Scene::Size()/2);
		cloud.draw2();
		Rect{ Scene::Size() }.draw(ColorF{ skyColor,0.4 });

		backgroundManager.draw(camera.pos);

		Rect{ Scene::Size() }.draw(ColorF{ skyColor,0.2 });

		{
			//座標変換
			const Transformer2D t{ camera.getMat3x2() };
			manager.draw();
			stage.draw(camera.pos+Scene::Size() / 2);
			DataManager::get().effect.update();

			{
				const ScopedRenderTarget2D target{ rTexture.clear(ColorF{0.5,0}) };
				const ScopedRenderStates2D blend{ MakeBlendState() };
				DataManager::get().additiveEffect.update();
			}
		}
		Graphics2D::Flush();
		rTexture.resolve();
		rTexture.draw();

		cookieDisplay(player->itemCount, DataManager::get().tame);
		hpDisplay(player->hp);

		if (DataManager::get().talkWindow.isContinue()) {
			DataManager::get().talkWindow.draw(RectF{ 0,500,Scene::Size().x,300});
		}


		FontAsset(U"TitleFont")(U"Escで操作方法").draw(40,850,0,Palette::Orange);

	}

	//ポーズ画面
	void pauseUpdate()override {

		if (SimpleGUI::ButtonAt(U"ゲームに戻る", { Scene::Center().x,700 })) {
			goGame();
		}
	}

	void pauseDraw()const override {
		Rect{ Scene::Size() }.draw(ColorF{ 0,0.7 });
		RoundRect{ Arg::center = Scene::Center(),1150,750,10 }.draw(ColorF{ Palette::Skyblue,0.7});
		FontAsset(U"TitleFont")(U"ポーズ").drawAt(Scene::Center().x, 100);
		FontAsset(U"TitleFont")(U"Space or W：ジャンプ\nA：左\nD：右\S：しゃがむ\nEnter：技を発動\nEnter長押し：突進(クッキーが10個貯まったら)\n").drawAt(50,Scene::Center());
		auto _=SimpleGUI::ButtonAt(U"ゲームに戻る", { Scene::Center().x,700 });
	}

	void EndGame(bool clear) {
		getData().mini_clear = clear;//クリア状況保存
		changeScene(U"Map",0s);
	}

	void drawFadeIn(double t) const override
	{
		draw();		
		Circle circle = Circle{player->pos,0}.lerp(Circle{ player->pos ,Scene::Size().length() },t);

		for (const auto& polygon : Geometry2D::Subtract(RectF{ Arg::center = player->pos,Scene::Size() * 2 }, circle.asPolygon()))
		{
			polygon.draw(Palette::Black);
		}

	}
};
