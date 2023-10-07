#pragma once

#include"Entity.h"
#include"Player.h"
#include"MotionLoader.h"
#include"CookieItem.h"
#include"CharacterSystem.h"

class StrawberrySoldier:public Entity {
public:

	bool left=false;

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

		if (left) {
			vel.x = -100;
		}
		else {
			vel.x = 100;
		}

		if (hitBox.touch(Direction::down) && hitBox.touch(Direction::up) && hitBox.touch(Direction::left) && hitBox.touch(Direction::right)) {
			hp = 0;
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

	void draw()const override {
		character.draw();
	}

	void end()override {
		DataManager::get().effect.add<StarEffect>(pos, 0);
		DataManager::get().addEntity(U"CookieItem",pos);
		//manager->add(new CookieItem{ pos });
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
				vel .x = 200;
			}
		}
		//プレイヤーに近すぎる場合
		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2) {
			left = false;
		}

		if (hitBox.touch(Direction::down) && hitBox.touch(Direction::up) && hitBox.touch(Direction::left) && hitBox.touch(Direction::right)) {
			hp = 0;
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

	void draw()const override {
		character.draw();
	}

	void end()override {
		DataManager::get().effect.add<StarEffect>(pos, 50);
		//manager->add(new CookieItem{ pos });
		DataManager::get().addEntity(U"CookieItem", pos);
	}
};

class Snowman:public Entity {
public:
	bool left = false;

	double attackAccumlater=0;

	CharacterSystem character;

	Snowman(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),90,130},cpos,{0,0},1 },
		character{ U"Characters/yukidaruma/test.json",U"Characters/yukidaruma/motion.txt",0.5,cpos,true,false }
	{
		character.addMotion(U"Attack", true);
	}

	void update()override {

		manager->stage->hit(&hitBox);

		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 5) {

			if (manager->get(U"Player")->pos.x < pos.x) {
				left = true;
				vel.x = -100;
			}
			else if (manager->get(U"Player")->pos.x >= pos.x) {
				left = false;
				vel.x = 100;
			}
		}

		//プレイヤーに近すぎる場合
		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2) {
			left = false;
		}

		if (hitBox.touch(Direction::down) && hitBox.touch(Direction::up) && hitBox.touch(Direction::left) && hitBox.touch(Direction::right)) {
			hp = 0;
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

	void draw()const override {
		character.draw();

		Mat3x2 mat=character.character.table.at(U"arm").joint.mat;
		{
			const Transformer2D transformer{ mat };

			Circle{ 100,-30,20 }.draw(Palette::Red);

		}
	}

	void end()override {
		DataManager::get().effect.add<StarEffect>(pos, 50);
		//manager->add(new CookieItem{ pos });
		DataManager::get().addEntity(U"CookieItem", pos);
	}


};
