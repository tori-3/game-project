#include"HawkDropOut.h"
#include"BGMManager.hpp"
#include"ControllerManager.h"
#include"PlayMode.h"

namespace HawkDropOut
{
	void HawkDropOut::onPauseStart()
	{
		gameOverTimerFlg = gameOverTimer.isStarted();

		if (gameOverTimerFlg)
		{
			gameOverTimer.pause();
		}
	}

	void HawkDropOut::onGameStart()
	{
		if (gameOverTimerFlg)
		{
			gameOverTimer.resume();
		}
	}

	HawkDropOut::HawkDropOut(const InitData& init)
		: MiniGameSceneBase{ init,true }
	{
		BGMManager::get().play(U"MiniGameBGM");



		switch (getData().mini_mode)
		{
		case Stage_Mode:
			clearScore = 1800;
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

	void HawkDropOut::gameUpdate()
	{
		Transformer2D transformer{ Mat3x2::Scale(1.5,{0,0 }) * Mat3x2::Translate(0,-15), TransformCursor::Yes };

		if (gameover)
		{
			if (gameOverTimer.reachedZero())
			{
				EndGame(false);
			}

			return;
		}

		if (clear)
		{
			if (getData().menuDecisionKey.down())
			{
				EndGame(true);
			}
			return;
		}

		groundTime += Scene::DeltaTime();


		//鷲がアイテムを入手してアイテム保持数を増やす
		if (cookieCircle.intersects(meatCircle)) {
			numofmeat += 1;
			AudioAsset{ U"食べる" }.playOneShot();
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
			astowndistance = -100 - 200;
			a_stownspeed += 20;
			a_enemyCircle.x = -60 - 200;
			a += 1;
		}

		if (bstowndistance >= 900) {
			bstowndistance = -150 - 200;
			b_stownspeed += 20;
			b_enemyCircle.x = -110 - 200;
			b += 1;
		}

		if (cstowndistance >= 900) {
			cstowndistance = -200 - 200;
			c_stownspeed += 20;
			c_enemyCircle.x = -160 - 200;
			c += 1;
		}

		if (dstowndistance >= 900) {
			dstowndistance = -220 - 200;
			d_stownspeed += 20;
			d_enemyCircle.x = -180 - 200;
			d += 1;
		}

		if (estowndistance >= 900) {
			estowndistance = -250 - 200;
			e_stownspeed += 20;
			e_enemyCircle.x = -210 - 200;
			e += 1;
		}

		//上昇ボタン
		if (ButtonUpdate(Rect{ 650, 180, 60, 60 }, 0.5, 30, 30, upEmoji,farmCount, upTime < 10.0, enemy, true) && ((getData().minigameUpKey.pressed()||ControllerManager::get().UpPressed())))
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
		if (ButtonUpdate(Rect{ 650, 250, 60, 60 }, 0.5, 30, 30, attackfunction, farmCount, numofmeat >= 1, enemy, j < 1) && (getData().minigameLB.pressed())) {
			attack = true;
			attackball.x = cookieCircle.x - 10;
			attackball.y = cookieCircle.y + 10;
			numofmeat -= 1;
			AudioAsset{ U"パンチ" }.playOneShot();
		}

		//攻撃描画
		if (attack) {
			//attackball.draw(Palette::Yellow);
			attackball.x -= Scene::DeltaTime() * 150;
			j = 1;
		}


		//攻撃が障害物に当たった場合に障害物の座標を戻す
		if (attackball.intersects(a_enemyCircle)) {
			AudioAsset{ U"パンチヒット" }.playOneShot();
			astowndistance += 900.0;
		}

		if (attackball.intersects(b_enemyCircle)) {
			AudioAsset{ U"パンチヒット" }.playOneShot();
			bstowndistance += 900.0;
		}

		if (attackball.intersects(c_enemyCircle)) {
			AudioAsset{ U"パンチヒット" }.playOneShot();
			cstowndistance += 900.0;
		}

		if (attackball.intersects(d_enemyCircle)) {
			AudioAsset{ U"パンチヒット" }.playOneShot();
			dstowndistance += 900.0;
		}

		if (attackball.intersects(e_enemyCircle)) {
			AudioAsset{ U"パンチヒット" }.playOneShot();
			estowndistance += 900.0;
		}

		//攻撃が画面外に出た場合の処理
		if (attackball.x < -50 /* || attackball.intersects(a_enemyCircle) || attackball.intersects(b_enemyCircle) || attackball.intersects(c_enemyCircle) || attackball.intersects(d_enemyCircle) || attackball.intersects(e_enemyCircle)*/) {
			attackball.x = -800-10000;
			attack = false;
			j = 0;
		}


		//超上昇ボタン
		if (ButtonUpdate(Rect{ 715, 244, 70, 70 }, 0.6, 40, 40, tornadoEmoji, farmCount, numofmeat >= 2, enemy, true) && (getData().minigameRB.down()))
		{
			tornadoFlg = true;
			numofmeat -= 2;
			AudioAsset{ U"サマーソルト" }.playOneShot();
		}

		if (tornadoFlg)
		{
			cookieCircle.y -= Scene::DeltaTime() * 1500.0;

			const bool pressedTornado = ButtonUpdate(Rect{ 715, 244, 70, 70 }, 0.6, 40, 40, tornadoEmoji, farmCount, true, false, true) && (getData().minigameRB.pressed());

			if (not pressedTornado || cookieCircle.y <= 50)
			{
				tornadoFlg = false;
			}
		}


		//超上昇ボタンのアイテム消費
		//if (numofmeat >= 2 && MouseL.up() && Rect { 690, 228, 100, 100 }.mouseOver()) {
		//	numofmeat -= 2;
		//}

		//下降ボタン
		if (ButtonUpdate(Rect{ 650, 320, 60, 60 }, 0.5, 30, 30, downEmoji, factoryCount, true, enemy, true) && ((getData().minigameDownKey.pressed())||ControllerManager::get().DownPressed()))
		{
			cookieCircle.y += Scene::DeltaTime() * 50.0;

			if (upTime >= 10.0) {
				upTime += Scene::DeltaTime() * 5;    //上昇可能時間を過ぎたら下降時に時間回復量
			}
			//else {
			//	upTime -= Scene::DeltaTime() * 0.1;	 //上昇可能時間内の時間回復量
			//}
		}


		// 回転角度を更新する
		angle += rotateSpeed * Scene::DeltaTime();

		around += aroundSpeed * Scene::DeltaTime();


		if (clearScore <= FlyDistance) {
			getData().saveMiniGameClear();
			clear = true;
			AudioAsset{U"ミニゲームクリア"}.playOneShot();
		}
		else if (cookieCircle.y >= 450)
		{
			gameOverTimer.restart();
			gameover = true;
			AudioAsset{ U"ミニゲームやられた" }.playOneShot();
			BGMManager::get().stop(0.2s);
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

			if (not receive)
			{
				AudioAsset{ U"アンナダメージ" }.playOneShot();
				ControllerManager::get().setVibration(0.3);
			}

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

	void HawkDropOut::gameDraw() const
	{
		{
			Transformer2D transformer{ Mat3x2::Scale(1.5,{0,0 }) * Mat3x2::Translate(0,-15), TransformCursor::Yes };

			//背景画面
			Scene::SetBackground(Palette::Blue);

			switch (getData().mini_mode)
			{
			case Stage_Mode:
				TextureAsset{ U"BackGroundTexture/雲背景.png" }.resized(800).draw();
				break;
			case Easy_Mode:
				TextureAsset{ U"BackGroundTexture/雪原背景.png" }.resized(800).draw();
				break;
			case Normal_Mode:
				TextureAsset{ U"BackGroundTexture/雲背景.png" }.resized(800).draw();
				break;
			case Hard_Mode:
				TextureAsset{ U"BackGroundTexture/宇宙背景.png" }.resized(800).draw();
				break;
			}

			const ColorF cloudColor = getData().mini_mode == Easy_Mode ? ColorF{ Palette::White } : getData().mini_mode == Hard_Mode ? ColorF{1,0.5}:ColorF{ Palette::Lightpink } + ColorF{ 0.3 };

			//雲を描画する
			cloudEmoji.scaled(1).draw(Aclouddistance, 100, cloudColor);

			cloudEmoji.scaled(1).draw(Bclouddistance, 250, cloudColor);

			cloudEmoji.scaled(1).draw(Cclouddistance, 200, cloudColor);

			cloudEmoji.scaled(1).draw(Dclouddistance, 50, cloudColor);

			cloudEmoji.scaled(1).draw(Eclouddistance, 150, cloudColor);

			constexpr double groundSize = 50;

			for (int32 i = -1; i < (800 / groundSize); ++i)
			{
				double pos = i * groundSize + Fmod(groundTime * 130, groundSize);
				groundTexture.resized(groundSize).draw(pos, 500);
			}


			//地面の描画
			//Rect{ -100, 500, 1000, 100 }.draw(Palette::Green);


			//障害物の回転した画像を描画する
			//ishi.scaled(0.2).rotated(angle).draw(astowndistance, 40);

			//ishi.scaled(0.2).rotated(angle).draw(bstowndistance, 120);

			//ishi.scaled(0.2).rotated(angle).draw(cstowndistance, 200);

			//ishi.scaled(0.2).rotated(angle).draw(dstowndistance, 280);

			//ishi.scaled(0.2).rotated(angle).draw(estowndistance, 360);


			ishi.scaled(0.2).rotated(angle).drawAt(a_enemyCircle.center);
			ishi.scaled(0.2).rotated(angle).drawAt(b_enemyCircle.center);
			ishi.scaled(0.2).rotated(angle).drawAt(c_enemyCircle.center);
			ishi.scaled(0.2).rotated(angle).drawAt(d_enemyCircle.center);
			ishi.scaled(0.2).rotated(angle).drawAt(e_enemyCircle.center);


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

			if (not photographyMode)
			{
				ButtonDraw(Rect{ 650, 180, 60, 60 }, 0.5, 30, 30, upEmoji, farmCount, upTime < 10.0, enemy, true,getData().minigameUpKey);
				ButtonDraw(Rect{ 715, 244, 70, 70 }, 0.6, 40, 40, tornadoEmoji, farmCount, numofmeat >= 2 || tornadoFlg, enemy, true, getData().minigameRB);
				ButtonDraw(Rect{ 650, 250, 60, 60 }, 0.5, 30, 30, attackfunction, farmCount, numofmeat >= 1, enemy, j < 1,getData().minigameLB);
				ButtonDraw(Rect{ 650, 320, 60, 60 }, 0.5, 30, 30, downEmoji, factoryCount, true, enemy, true,getData().minigameDownKey);

				if (10 > upTime) {
					font(U"上昇可能時間: {:.2f} 秒"_fmt(10 - upTime)).draw(30, 0, 500, Palette::White);
				}
				else {
					font(U"上昇不可能時間: {:.2f} 秒"_fmt(20 - upTime)).draw(30, 0, 500, Palette::Red);

					if(not enemy)
					{
						FontAsset{ U"NormalFont" }(U"{}で回復"_fmt(ToKeyName(getData().minigameDownKey, getData().gamepadMode))).draw(28, 370, 500, Palette::Yellow);
					}
				}

				font(U"ゴールまで {}"_fmt(clearScore- FlyDistance)).draw(30, 10, 10, getData().mini_mode == Hard_Mode ? Palette::White : Palette::Black);
			}

			if (clear)
			{
				Rect{ 800,600 }.draw(ColorF{ 0,0.5 });
				FontAsset{ U"NormalFont" }(U"Clear!!").drawAt(100, 400, 250);
				FontAsset{ U"NormalFont" }(U"{}で戻る"_fmt(ToKeyName(getData().menuDecisionKey, getData().gamepadMode))).drawAt(30, 400, 350);
			}
		}

		if(not photographyMode)
		{
			FontAsset{ U"NormalFont" }(U"{} ポーズ"_fmt(ToKeyName(getData().pauseKey, getData().gamepadMode))).draw(30, Arg::bottomRight = Vec2{ Scene::Width() - 10,Scene::Height() - 5 });
		}
	}
}
