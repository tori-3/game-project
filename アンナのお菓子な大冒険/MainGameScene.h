#pragma once
#include"Common.h"
#include"Enemy.h"
#include"setting.h"
#include"Pause.h"
#include"Player.h"
#include"BackGround.h"

class EnemyAdder {
public:

	EntityManager* manager;

	EnemyAdder(EntityManager* manager) :manager{manager} {
		table[U"CookieItem"]= [](const Vec2& pos) {return new CookieItem{ pos }; };
		table[U"StrawberrySoldier"] = [](const Vec2& pos) {return new StrawberrySoldier{ pos }; };
		table[U"CookieSoldier"] = [](const Vec2& pos) {return new CookieSoldier{ pos }; };
	}

	HashTable<String, std::function<Entity* (const Vec2&)>>table;

	void update() {
		for (auto& a : DataManager::get().list) {
			manager->add(table[a.first](a.second));
		}
		DataManager::get().list.clear();
	}
};

//本当はcppですべきなんだろうなー
DataManager* DataManager::instance = nullptr;

class MainGameScene : public IPauseScene
{
public:
	void cookieDisplay(int32 count)const {

		TextureAsset::Register(U"CookieItem", U"🍪"_emoji);

		for (auto i : step(10)) {
			if (i < count) {
				TextureAsset(U"CookieItem").resized(40).drawAt(25+  i * 40, 75);
			}
			else {
				TextureAsset(U"CookieItem").resized(40).drawAt(25+  i * 40, 75, ColorF{ 1,0.3 });
			}
		}
	}

	void hpDisplay(int32 count)const {

		TextureAsset::Register(U"HP", U"❤"_emoji);

		for (auto i : step(3)) {
			if (i < count) {
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25);
			}
			else {
				TextureAsset(U"HP").resized(40).drawAt(25 + i * 40, 25, ColorF{ 1,0.3 });
			}
		}
	}

	Stage stage{ U"stage1.json" };

	MSRenderTexture rTexture{ Scene::Size(),ColorF{0,0} };

	DataManagerStart start;
	//Player player{ Point(500, 350 + 70) };
	Background background{ U"背景.png" ,stage.width() };
	Background background2{ U"お菓子の背景.png" ,stage.width(),2 };

	EntityManager manager;

	EnemyAdder adder{ &manager };

	//Player player{ Point(500, 350 + 70) };

	CloudManager cloud;

	//	ColorF skyColor{ 0.8, 0.9, 1.0};
	ColorF skyColor{Palette::Skyblue};

	Player* player=nullptr;

	// コンストラクタ（必ず実装）
	MainGameScene(const InitData& init)
		: IPauseScene{ init }
	{
		player = new Player{ Point(500, 350 + 70) };
		manager.add(player);
		manager.stage = &stage;

		manager.add(new Snowman{ Point(500 + 70 * 6, 350 + 70) });
	}

	// 更新関数（オプション）
	void gameUpdate() override
	{
		DataManager::get().playerPos = player->pos;

		adder.update();

		//座標変換(カーソルだけ)
		double hieght = Scene::Size().y - draw_y;
		//if (player.pos.y < change_hieght)hieght += change_hieght - player.pos.y;
		Mat3x2 mat = Mat3x2::Translate(Vec2{ draw_x - player->pos.x, hieght });
		const Transformer2D transformer{ Mat3x2::Identity(),mat };
		{
			manager.update();
		}

		////落下したらスタートに戻す。(デバッグ用)
		if (player->hp <= 0||1000<player->pos.y) {
			player->pos = Vec2(500, 350);
			player->hp = 3;
		}

		stage.update(player->pos);

		DataManager::get().talkWindow.update(MouseL.down());

		cloud.update();

		if (KeyEscape.down())goPause();

		if (DataManager::get().table.contains(U"Clear")) {
			if (KeyEnter.down()) {
				changeScene(U"MainGameScene");
			}
		}

		ClearPrint();
		Print << player->pos.y;
	}

	// 描画関数（オプション）
	void gameDraw() const override
	{
		Scene::SetBackground(skyColor);

		cloud.draw1();
		background.draw(player->pos);
		cloud.draw2();
		Rect{ Scene::Size() }.draw(ColorF{ skyColor,0.2 });
		background2.draw(player->pos);
		Rect{ Scene::Size() }.draw(ColorF{ skyColor,0.2 });

		{
			//座標変換
			double hieght = Scene::Size().y - draw_y;
			//if (player.pos.y < change_hieght)hieght += change_hieght - player.pos.y;
			Mat3x2 mat = Mat3x2::Translate(Vec2{ draw_x - player->pos.x, hieght });
			const Transformer2D transformer{ mat, TransformCursor::Yes };

			stage.draw(player->pos);

			manager.draw();

			DataManager::get().effect.update();

			{
				const ScopedRenderTarget2D target{ rTexture.clear(ColorF{0.5,0}) };
				DataManager::get().additiveEffect.update();
			}
		}
		Graphics2D::Flush();
		rTexture.resolve();
		rTexture.draw();

		Circle{ Cursor::Pos(),50 }.drawFrame(5, ColorF{ Palette::Red,0.3 });
		Shape2D::Plus(80, 5, Cursor::Pos()).draw(ColorF{ Palette::Red,0.3 });

		cookieDisplay(player->itemCount);
		hpDisplay(player->hp);

		if (DataManager::get().talkWindow.isContinue()) {
			DataManager::get().talkWindow.draw(RectF{ 0,500,Scene::Size().x,300});
		}


		FontAsset(U"TitleFont")(U"Escで操作方法").draw(40,850,0,Palette::Orange);

		if (DataManager::get().table.contains(U"Clear")) {
			Rect{ Scene::Size() }.draw(ColorF{ 0,0.5 });
			FontAsset(U"TitleFont")(U"クリア！！").drawAt(100,Scene::Center());

			FontAsset(U"TitleFont")(U"Enterキー：もう一度").drawAt(50, Scene::Center().x,600);
		}
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
		FontAsset(U"TitleFont")(U"Wキー：ジャンプ\nAキー：左\nDキー：右\nカーソル：標準を合わせる\n左クリック：波動拳\n右クリック：突進(クッキーが10個貯まったら)").drawAt(50,Scene::Center());
		auto _=SimpleGUI::ButtonAt(U"ゲームに戻る", { Scene::Center().x,700 });
	}

};
