#pragma once

#include"Entity.h"
#include"Player.h"
#include"MotionLoader.h"
#include"CookieItem.h"
#include"CharacterSystem.h"

class StrawberrySoldier:public Entity {
public:

	bool left=true;

	CharacterSystem character;

	StrawberrySoldier(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),70,70},cpos,{0,0},1}
		, character{ U"CharacterImages/itigo/itigo.json" ,U"CharacterImages/itigo/motion.txt" ,0.3,cpos,true,false }
	{
		character.addMotion(U"",true);
	}

	void update()override {

		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::right))
		{
			left = true;
		}
		else if (hitBox.touch(Direction::left)) {
			left = false;
		}

		if (hitBox.touch(Direction::down)) {
			if (left) {
				if (not hitBox.leftFloor()) {
					left = false;
				}
			}
			else {
				if (not hitBox.rightFloor()) {
					left = true;
				}
			}
		}

		if (left) {
			vel.x = -100;
		}
		else {
			vel.x = 100;
		}

		hitBox.physicsUpdate();
		hitBox.update();

		if (manager->get(U"Player")->hitBox.intersects(hitBox)) {
			if (pos.x < manager->get(U"Player")->pos.x) {
				manager->get(U"Player")->damage(1, Vec2{ 100,-20 });
			}
			else {
				manager->get(U"Player")->damage(1, Vec2{ -100,-20 });
			}

		}

		character.update(pos,left);
	}

	void lateUpdate() {
		if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 0);
			manager->add(new CookieItem{ pos });
		}
	}

	void draw()const override {
		character.draw();
	}
};

class CookieSoldier :public Entity {
public:

	bool left = false;

	CharacterSystem character;

	CookieSoldier(const Vec2& cpos) :Entity{ {U"Enemy"}, RectF{Arg::center(0,0),70,70},cpos,{0,0},1},
		character{ U"CharacterImages/cookie/cookie.json",U"CharacterImages/cookie/motion_cookie.txt",0.3,cpos,true,false }
	{
		character.addMotion(U"",true);
	}

	void update()override {

		manager->stage->hit(&hitBox);

		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 5) {

			if (manager->get(U"Player")->pos.x < pos.x) {
				left = true;
				vel.x = -200;
			}
			else if (manager->get(U"Player")->pos.x >= pos.x) {
				left = false;
				vel.x = 200;
			}
		}

		//プレイヤーに近すぎる場合
		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2) {
			left = false;
		}

		hitBox.physicsUpdate();
		hitBox.update();

		if (manager->get(U"Player")->hitBox.intersects(hitBox)) {
			if (pos.x < manager->get(U"Player")->pos.x) {
				manager->get(U"Player")->damage(1, Vec2{ 100,-20 });
			}
			else {
				manager->get(U"Player")->damage(1, Vec2{ -100,-20 });
			}
		}

		character.update(pos, left);
	}

	void lateUpdate()override {
		if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 50);
			manager->add(new CookieItem{ pos });
		}
	}

	void draw()const override {
		character.draw();
	}
};

class SnowBall:public Entity {
public:
	bool left;

	SnowBall(const Vec2& pos, bool left) :Entity{ U"Enemy", Circle{0,0,10},pos,{0,0},1 }, left{left} {
		vel.y = -500;
	}

	void update()override {
		bool collisionFlg = false;

		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::left) || hitBox.touch(Direction::right) || hitBox.touch(Direction::up) || hitBox.touch(Direction::down)) {
			collisionFlg = true;
		}

		vel.x = left ? -400 : 400;

		hitBox.physicsUpdate();
		hitBox.update();

		if (manager->get(U"Player")->hitBox.intersects(hitBox)) {
			manager->get(U"Player")->damage(1, Vec2{ left?-100:100,-20 });
			collisionFlg = true;
		}

		if (collisionFlg) {
			hp = 0;
		}

	}

	void lateUpdate()override {
		if (not isActive()) {
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, Palette::White);
		}
	}

	void draw()const override {
		hitBox.Get_Box().draw(Palette::White);
	}

};

class Snowman:public Entity {
public:
	bool left = false;

	bool attackMode = false;

	double attackAccumlater=0;

	bool holdSnowBall = false;

	double t = 0;

	CharacterSystem character;

	Snowman(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),70,130},cpos,{0,0},1 },
		character{ U"Characters/yukidaruma/test.json",U"Characters/yukidaruma/motion.txt",0.5,cpos,true,false }
	{
		character.addMotion(U"Walk");
	}

	void update()override {

		attackMode = false;

		manager->stage->hit(&hitBox);

		double playerX = manager->get(U"Player")->pos.x;
		if (Abs(playerX - pos.x) < rect_size * 6) {
			attackMode = true;
			left = playerX < pos.x;
		}
		else {
			if (not character.character.hasMotion(U"Walk")) {
				character.addMotion(U"Walk");
			}

			if (hitBox.touch(Direction::right))
			{
				left = true;
			}
			else if (hitBox.touch(Direction::left)) {
				left = false;
			}
			if (hitBox.touch(Direction::down)) {
				if (left) {
					if (not hitBox.leftFloor()) {
						left = false;
					}
				}
				else {
					if (not hitBox.rightFloor()) {
						left = true;
					}
				}
			}

			if (left) {
				vel.x = -100;
			}
			else {
				vel.x = 100;
			}
		}

		hitBox.physicsUpdate();
		hitBox.update();

		if (attackMode) {

			constexpr double spawn = 2;
			for (attackAccumlater += Scene::DeltaTime(); spawn <= attackAccumlater; attackAccumlater -= spawn) {

				character.addMotion(U"Attack");
				holdSnowBall = true;
				//manager->add(new SnowBall{ pos,left });
			}

		}

		if (holdSnowBall) {

			t += Scene::DeltaTime();

			if (0.6<t) {
				holdSnowBall = false;
				const Mat3x2 mat = character.character.table.at(U"arm").joint.mat;
				const Vec2 ballPos = mat.transformPoint(Vec2{ 100, -30 });
				manager->add(new SnowBall{ ballPos,left });
				t = 0;
			}

		}

		if (manager->get(U"Player")->hitBox.intersects(hitBox)) {
			if (pos.x < manager->get(U"Player")->pos.x) {
				manager->get(U"Player")->damage(1, Vec2{ 100,-20 });
			}
			else {
				manager->get(U"Player")->damage(1, Vec2{ -100,-20 });
			}

		}

		character.update(pos, left);
	}

	void lateUpdate()override {
		if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 50);
			manager->add(new CookieItem{ pos });
		}
	}

	void draw()const override {
		character.draw();
		if (holdSnowBall) {
			const Mat3x2 mat = character.character.table.at(U"arm").joint.mat;
			const Vec2 ballPos = mat.transformPoint(Vec2{ 100, -30 });
			Circle{ ballPos,10 }.draw(Palette::White);
		}
	}
};
