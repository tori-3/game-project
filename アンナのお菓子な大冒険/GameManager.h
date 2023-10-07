#pragma once
#include"Common.h"
//#include"DataManager.h"
#include"Player.h"
#include"setting.h"


class Background
{
public:
	Texture texture;
	double r, rate;
	uint32 num;

	Background(const String& pass, int x, uint32 n = 1) :texture{ pass } {
		r = Scene::Size().y / (double)texture.height();//倍率
		rate = (texture.width() * r * n - Scene::Size().x) / (rect_size * x);//画像を進める速度
		num = n;
	}
	void draw(const Vec2& vec)const {
		for (auto n : step(num)) {
			texture.scaled(r).draw(-vec.x * rate + (texture.width() * r) * n, 0);
		}
	}

};

class GameManager {
public:

	DataManagerStart start;
	Player player{ Point(500, 350+70) };
	Background background{ U"背景.png" ,DataManager::get().stage.width() };
	Background background2{ U"木背景.png" ,DataManager::get().stage.width(),3 };

	GameManager() {
	}

	~GameManager() {
	}

	void update() {

		player.update();

		if (MouseL.down()) {
			DataManager::get().effect.add([pos = Cursor::Pos(), color = RandomColorF()](double t) {Circle{ pos, (t * 100) }.drawFrame(4, color); return (t < 1.0); });
		}

		//落下したらスタートに戻す。(デバッグ用)
		if (player.pos.y > 1000)player.pos = Vec2(500, 350);
		if (player.hp <= 0) {
			player.pos = Vec2(500, 350);
			player.hp = 100;
		}

		DataManager::get().stage.update();
	}


	void draw()const {
		background.draw(player.pos);
		background2.draw(player.pos);

		//座標変換
		double hieght = Scene::Size().y - draw_y;
		if (player.pos.y < change_hieght)hieght += change_hieght - player.pos.y;
		Mat3x2 mat = Mat3x2::Translate(Vec2(draw_x - player.pos.x, hieght));
		const Transformer2D transformer{ mat, TransformCursor::Yes };

		player.draw();
		DataManager::get().stage.draw(player.pos);
		DataManager::get().effect.update();
	}
};
