#pragma once
#include"Common.h"

namespace HawkDropOut {

	class HawkDropOut : public App::Scene
	{
	public:

		Transformer2D transformer{ Mat3x2::Scale(1.5,{0,0 }) * Mat3x2::Translate(0,-15), TransformCursor::Yes };
		int32 clearScore = 0;

		HawkDropOut(const InitData& init)
			: IScene{ init }
		{
			if (not TextureAsset::IsRegistered(U"BackGroundTexture/雪原背景.png")) {
				TextureAsset::Register(U"BackGroundTexture/雪原背景.png", U"BackGroundTexture/雪原背景.png");
			}

			AudioAsset::Register(U"MiniGameBGM", U"BGM/MiniGameBGM.wav", Loop::Yes);
			BGMManager::get().play(U"MiniGameBGM");





			switch (getData().mini_mode)
			{
			case Stage_Mode:
				clearScore = 3000;
				break;
			case Easy_Mode:
				clearScore = 2000;
				break;
			case Normal_Mode:
				clearScore = 3000;
				break;
			case Hard_Mode:
				clearScore = 5000;
				break;
			}
		}

		~HawkDropOut() {

		}

		bool ButtonUpdate(const Rect& rect, double scale, int32 xdistance, int32 ydistance, const Texture& texture, const Font& font, int32 count, bool enabled, bool enemy, bool att)
		{
			return(enabled && !enemy);
		}

		void ButtonDraw(const Rect& rect, double scale, int32 xdistance, int32 ydistance, const Texture& texture, const Font& font, int32 count, bool enabled, bool enemy, bool att)const
		{
			if (enabled && !enemy && att)
			{
				texture.scaled(scale).drawAt(rect.x + xdistance, rect.y + ydistance);

				if (rect.mouseOver())
				{
					Cursor::RequestStyle(CursorStyle::Hand);
				}
			}
			else
			{
				rect.draw(Palette::Black);

				rect.drawFrame(2, 2, ColorF(0.5));
			}
		}


		double groundTime = 0;

		void draw() const override
		{
			//背景画面
			Scene::SetBackground(Palette::Blue);

			TextureAsset{ U"BackGroundTexture/雪原背景.png" }.resized(800).draw();

			//雲を描画する
			cloudEmoji.scaled(1).draw(Aclouddistance, 100);

			cloudEmoji.scaled(1).draw(Bclouddistance, 250);

			cloudEmoji.scaled(1).draw(Cclouddistance, 200);

			cloudEmoji.scaled(1).draw(Dclouddistance, 50);

			cloudEmoji.scaled(1).draw(Eclouddistance, 150);

			constexpr double groundSize = 50;

			for (int32 i = -1; i < (800 / groundSize); ++i)
			{
				double pos = i * groundSize + Fmod(groundTime * 130, groundSize);
				groundTexture.resized(groundSize).draw(pos, 500);
			}


			//地面の描画
			//Rect{ -100, 500, 1000, 100 }.draw(Palette::Green);

			ButtonDraw(Rect{ 650, 180, 60, 60 }, 0.5, 30, 30, upEmoji, font, farmCount, upTime < 10.0, enemy, true);
			ButtonDraw(Rect{ 650, 250, 60, 60 }, 0.5, 30, 30, attackfunction, font, farmCount, numofmeat >= 1, enemy, j < 1);
			ButtonDraw(Rect{ 715, 244, 70, 70 }, 0.6, 40, 40, tornadoEmoji, font, farmCount, numofmeat >= 2, enemy, true);
			ButtonDraw(Rect{ 650, 320, 60, 60 }, 0.5, 30, 30, downEmoji, font, factoryCount, true, enemy, true);

			if (10 > upTime) {
				font(U"上昇可能時間: {:.2f} 秒"_fmt(10 - upTime)).draw(30, 0, 500, Palette::Black);
			}
			else {
				font(U"上昇不可能時間: {:.2f} 秒"_fmt(20 - upTime)).draw(30, 0, 500, Palette::Red);
			}

			//飛距離の表示
			font(U"飛距離").draw(30, 10, 10, Palette::Black);

			font(FlyDistance).draw(30, 120, 10, Palette::Black);

			//障害物の回転した画像を描画する
			ishi.scaled(0.2).rotated(angle).draw(astowndistance, 40);

			ishi.scaled(0.2).rotated(angle).draw(bstowndistance, 120);

			ishi.scaled(0.2).rotated(angle).draw(cstowndistance, 200);

			ishi.scaled(0.2).rotated(angle).draw(dstowndistance, 280);

			ishi.scaled(0.2).rotated(angle).draw(estowndistance, 360);

			//アイテムの画像を描画する
			meat.scaled(0.5).draw(meatCircle.x - 30, meatCircle.y - 30);

			if (cookieCircle.intersects(a_enemyCircle) || cookieCircle.intersects(b_enemyCircle) || cookieCircle.intersects(c_enemyCircle) || cookieCircle.intersects(d_enemyCircle) || cookieCircle.intersects(e_enemyCircle) || receive) {

				texture.scaled(cookieScale).rotated(-angle).drawAt(cookieCircle.x, cookieCircle.y);
			}
			else {

				//鷲が障害物に当たっていない場合の描画
				texture.scaled(cookieScale).drawAt(cookieCircle.x, cookieCircle.y);
			}


			if (attack) {
				wing.scaled(0.25).draw(attackball.x - 40, attackball.y - 40);
			}

		}

		void update() override
		{
			groundTime += Scene::DeltaTime();


			//鷲がアイテムを入手してアイテム保持数を増やす
			if (cookieCircle.intersects(meatCircle)) {
				numofmeat += 1;
			}

			//鷲がアイテムを入手するかアイテムが鷲を通り過ぎた時にランダム生成する
			if (cookieCircle.intersects(meatCircle) || meatCircle.x >= 900)
			{
				meatCircle.x = Random(-500, -250);
				meatCircle.y = Random(100, 400);
			}


			//飛距離を加算する
			FlyDistance += Scene::DeltaTime() * 100;

			//鷲が常に下降している
			cookieCircle.y += Scene::DeltaTime() * (dropspeed + plasdropspeed);

			//雲を動かす
			Aclouddistance += Scene::DeltaTime() * 100;

			Bclouddistance += Scene::DeltaTime() * 100;

			Cclouddistance += Scene::DeltaTime() * 100;

			Dclouddistance += Scene::DeltaTime() * 100;

			Eclouddistance += Scene::DeltaTime() * 100;

			//障害物を動かす
			astowndistance += Scene::DeltaTime() * a_stownspeed;

			bstowndistance += Scene::DeltaTime() * b_stownspeed;

			cstowndistance += Scene::DeltaTime() * c_stownspeed;

			dstowndistance += Scene::DeltaTime() * d_stownspeed;

			estowndistance += Scene::DeltaTime() * e_stownspeed;

			//アイテムを動かす
			meatCircle.x += Scene::DeltaTime() * e_stownspeed;

			//障害物の当たり判定のための円を動かす
			a_enemyCircle.x += Scene::DeltaTime() * a_stownspeed;

			b_enemyCircle.x += Scene::DeltaTime() * b_stownspeed;

			c_enemyCircle.x += Scene::DeltaTime() * c_stownspeed;

			d_enemyCircle.x += Scene::DeltaTime() * d_stownspeed;

			e_enemyCircle.x += Scene::DeltaTime() * e_stownspeed;

			//雲が画面外に出たら座標を戻す
			if (Aclouddistance >= 900) {
				Aclouddistance = -200;
			}

			if (Bclouddistance >= 900) {
				Bclouddistance = -200;
			}

			if (Cclouddistance >= 900) {
				Cclouddistance = -200;
			}

			if (Dclouddistance >= 900) {
				Dclouddistance = -200;
			}

			if (Eclouddistance >= 900) {
				Eclouddistance = -200;
			}

			//障害物が画面外に出たら座標を戻す
			if (astowndistance >= 900) {
				astowndistance = -100;
				a_stownspeed += 20;
				a_enemyCircle.x = -60;
				a += 1;
			}

			if (bstowndistance >= 900) {
				bstowndistance = -150;
				b_stownspeed += 20;
				b_enemyCircle.x = -110;
				b += 1;
			}

			if (cstowndistance >= 900) {
				cstowndistance = -200;
				c_stownspeed += 20;
				c_enemyCircle.x = -160;
				c += 1;
			}

			if (dstowndistance >= 900) {
				dstowndistance = -220;
				d_stownspeed += 20;
				d_enemyCircle.x = -180;
				d += 1;
			}

			if (estowndistance >= 900) {
				estowndistance = -250;
				e_stownspeed += 20;
				e_enemyCircle.x = -210;
				e += 1;
			}

			//上昇ボタン
			if (ButtonUpdate(Rect{ 650, 180, 60, 60 }, 0.5, 30, 30, upEmoji, font, farmCount, upTime < 10.0, enemy, true) && Rect { 650, 180, 60, 60 }.mouseOver() && MouseL.pressed())
			{
				cookieCircle.y -= Scene::DeltaTime() * (50.0 + plasdropspeed);
				upTime += Scene::DeltaTime();
				Deltadistance += Scene::DeltaTime() * 100;
			}

			//上昇可能時間
			if (upTime >= 10.0) {
				upTime += Scene::DeltaTime();
				if (upTime >= 20) {
					upTime = 0.0;
				}
			}

			//攻撃ボタン
			if (ButtonUpdate(Rect{ 650, 250, 60, 60 }, 0.5, 30, 30, attackfunction, font, farmCount, numofmeat >= 1, enemy, j < 1) && Rect { 650, 250, 60, 60 }.mouseOver() && MouseL.down()) {
				attack = true;
				attackball.x = cookieCircle.x - 10;
				attackball.y = cookieCircle.y + 10;
				numofmeat -= 1;
			}

			//攻撃描画
			if (attack) {
				//attackball.draw(Palette::Yellow);
				attackball.x -= Scene::DeltaTime() * 150;
				j = 1;
			}


			//攻撃が障害物に当たった場合に障害物の座標を戻す
			if (attackball.intersects(a_enemyCircle)) {
				astowndistance += 900.0;
			}

			if (attackball.intersects(b_enemyCircle)) {
				bstowndistance += 900.0;
			}

			if (attackball.intersects(c_enemyCircle)) {
				cstowndistance += 900.0;
			}

			if (attackball.intersects(d_enemyCircle)) {
				dstowndistance += 900.0;
			}

			if (attackball.intersects(e_enemyCircle)) {
				estowndistance += 900.0;
			}

			//攻撃が画面外に出た場合の処理
			if (attackball.x < -50 /* || attackball.intersects(a_enemyCircle) || attackball.intersects(b_enemyCircle) || attackball.intersects(c_enemyCircle) || attackball.intersects(d_enemyCircle) || attackball.intersects(e_enemyCircle)*/) {
				attackball.x = -800;
				attack = false;
				j = 0;
			}

			//超上昇ボタン
			if (ButtonUpdate(Rect{ 715, 244, 70, 70 }, 0.6, 40, 40, tornadoEmoji, font, farmCount, numofmeat >= 2, enemy, true) && Rect { 715, 244, 70, 70 }.mouseOver() && MouseL.pressed())
			{
				cookieCircle.y -= Scene::DeltaTime() * 1500.0;
			}

			//超上昇ボタンのアイテム消費
			if (numofmeat >= 2 && MouseL.up() && Rect { 690, 228, 100, 100 }.mouseOver()) {
				numofmeat -= 2;
			}

			//下降ボタン
			if (ButtonUpdate(Rect{ 650, 320, 60, 60 }, 0.5, 30, 30, downEmoji, font, factoryCount, true, enemy, true) && Rect { 650, 320, 60, 60 }.mouseOver() && MouseL.pressed())
			{
				cookieCircle.y += Scene::DeltaTime() * 50.0;

				if (upTime >= 10.0) {
					upTime += Scene::DeltaTime() * 5;    //上昇可能時間を過ぎたら下降時に時間回復量
				}
				else {
					upTime -= Scene::DeltaTime() * 0.1;	 //上昇可能時間内の時間回復量
				}
			}


			// 回転角度を更新する
			angle += rotateSpeed * Scene::DeltaTime();

			around += aroundSpeed * Scene::DeltaTime();


			if (clearScore <= FlyDistance) {
				EndGame(true);
			}
			else if (cookieCircle.y >= 450)
			{
				EndGame(false);
			}

			//当たり判定の確認
			//a_enemyCircle.draw();

			//b_enemyCircle.draw();

			//c_enemyCircle.draw();

			//d_enemyCircle.draw();

			//e_enemyCircle.draw();

			//cookieCircle.draw();

			//meatCircle.draw();

			//鷲が障害物に当たった場合の描画
			if (cookieCircle.intersects(a_enemyCircle) || cookieCircle.intersects(b_enemyCircle) || cookieCircle.intersects(c_enemyCircle) || cookieCircle.intersects(d_enemyCircle) || cookieCircle.intersects(e_enemyCircle) || receive) {

				receive = true;
				enemy = true;

			}
			else {

				//鷲が障害物に当たっていない場合の描画

				enemy = false;
			}

			////鷲が障害物に当たった場合の他の物体の動き
			if (cookieCircle.intersects(a_enemyCircle) && a < 2) {
				a_stownspeed += 10;
				b_stownspeed -= 5;
				c_stownspeed -= 5;
				d_stownspeed -= 5;
				e_stownspeed -= 5;
				a += 1;
				b += 1;
				c += 1;
				d += 1;
				e += 1;
			}

			if (cookieCircle.intersects(b_enemyCircle) && b < 2) {
				a_stownspeed -= 5;
				b_stownspeed += 10;
				c_stownspeed -= 5;
				d_stownspeed -= 5;
				e_stownspeed -= 5;
				a += 1;
				b += 1;
				c += 1;
				d += 1;
				e += 1;
			}

			if (cookieCircle.intersects(c_enemyCircle) && c < 2) {
				a_stownspeed -= 5;
				b_stownspeed -= 5;
				c_stownspeed += 10;
				d_stownspeed -= 5;
				e_stownspeed -= 5;
				a += 1;
				b += 1;
				c += 1;
				d += 1;
				e += 1;
			}

			if (cookieCircle.intersects(d_enemyCircle) && d < 2) {
				a_stownspeed -= 5;
				b_stownspeed -= 5;
				c_stownspeed -= 5;
				d_stownspeed += 10;
				e_stownspeed -= 5;
				a += 1;
				b += 1;
				c += 1;
				d += 1;
				e += 1;
			}

			if (cookieCircle.intersects(e_enemyCircle) && e < 2) {
				a_stownspeed -= 5;
				b_stownspeed -= 5;
				c_stownspeed -= 5;
				d_stownspeed -= 5;
				e_stownspeed += 10;
				a += 1;
				b += 1;
				c += 1;
				d += 1;
				e += 1;
			}

			//攻撃を受けて2秒間操作不可能になる
			if (receive && SumTime <= stopTime) {
				enemy = true;
				SumTime += Scene::DeltaTime();
				dropspeed = 30;
			}

			//操作不可能状態の解除
			if (SumTime >= stopTime) {
				enemy = false;
				receive = false;
				SumTime = 0.0;
				dropspeed = 10;
				a = 0;
				b = 0;
				c = 0;
				d = 0;
				e = 0;
			}

			//障害物のスピードが大幅に落ちてしまった場合の処理
			if (a_stownspeed <= 50) {
				a_stownspeed = 150;
			}
			if (b_stownspeed <= 50) {
				b_stownspeed = 150;
			}
			if (c_stownspeed <= 50) {
				c_stownspeed = 150;
			}
			if (d_stownspeed <= 50) {
				d_stownspeed = 150;
			}
			if (e_stownspeed <= 50) {
				e_stownspeed = 150;
			}

			//上制限
			if (cookieCircle.y <= 50) {
				cookieCircle.y = 50;
			}

			//距離によって障害物が出てくる頻度が変わるようにする処理
			if (Deltadistance >= 500) {
				plasdropspeed += 2;
				Deltadistance = 0;
			}

		}


	private:

		int32 j = 0;

		bool attack = false;

		double upTime = 0.0;

		int32 numofmeat = 0;

		double plasdropspeed = 0.0;

		double Deltadistance = 0.0;

		int32 a = 0;

		int32 b = 0;

		int32 c = 0;

		int32 d = 0;

		int32 e = 0;

		double dropspeed = 10;

		bool receive = false;

		double SumTime = 0.0;

		double stopTime = 2.0;

		double around = 0.0;

		bool enemy = false;

		double cookienumber = 500;

		double astowndistance = -240;

		double bstowndistance = -500;

		double cstowndistance = -1000;

		double dstowndistance = -1600;

		double estowndistance = -2800;

		double a_stownspeed = 150;

		double b_stownspeed = 150;

		double c_stownspeed = 150;

		double d_stownspeed = 150;

		double e_stownspeed = 150;
		// 回転角度
		double angle = 0.0;

		// 回転速度（1秒あたりの回転角度）
		const double rotateSpeed = 360.0; // 1秒あたり360度

		const double aroundSpeed = 30.0;

		// タイマー
		double timer = 0.0;

		Image image{ U"HawkDropOut/StrawberrySoldier.png" };

		Texture ishi{ image };

		Image feather{ U"HawkDropOut/wing.png" };

		Texture wing{ feather };

		Texture attackfunction{ U"💥"_emoji };

		Texture meat{ U"🍪"_emoji };

		// クッキーの絵文字
		Texture texture{ U"🦅"_emoji };

		// 農場の絵文字
		Texture upEmoji{ U"↖️"_emoji };

		Texture tornadoEmoji{ U"🌪️"_emoji };

		// 工場の絵文字
		Texture downEmoji{ U"↙️"_emoji };

		Texture cloudEmoji{ U"☁️"_emoji };

		Texture groundTexture{ U"StageTexture/CakeSurface.png" };

		// フォント
		Font font{ FontMethod::MSDF, 48, Typeface::Bold };

		// クッキーの表示サイズ（倍率）
		double cookieScale = 0.9;

		// 農場の所有数
		int32 farmCount = 0;

		// 工場の所有数
		int32 factoryCount = 0;

		// 農場の価格
		int32 farmCost = 0;

		// 工場の価格
		int32 factoryCost = 0;

		int32 speed = 50;

		double Aclouddistance = 100;

		double Bclouddistance = 300;

		double Cclouddistance = 600;

		double Dclouddistance = -200;

		double Eclouddistance = -300;

		int32 FlyDistance = 0;

		// クッキーの毎秒の生産量
		int32 cps = 0;

		// ゲームの経過時間の蓄積
		double accumulatedTime = 0.0;

		// クッキーのクリック円
		Circle cookieCircle{ 500, 280, 50 };

		Circle cookieCirclea{ 500, 180, 70 };

		Circle a_enemyCircle{ -200, 80, 20 };

		Circle b_enemyCircle{ -460, 160, 20 };

		Circle c_enemyCircle{ -960, 240, 20 };

		Circle d_enemyCircle{ -1560, 320, 20 };

		Circle e_enemyCircle{ -2760, 400, 20 };

		Circle meatCircle{ 1000, 360, 30 };

		Circle attackball{ 0, 0, 40 };

	private:
		void EndGame(bool clear) {
			getData().mini_clear = clear;//クリア状況保存
			if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
			else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
		}

	};

}
