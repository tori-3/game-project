﻿#pragma once
#include"Common.h"
#include"Entity.h"
#include"Player.h"
#include"MotionLoader.h"
#include"CookieItem.h"
#include"CharacterSystem.h"

class StrawberrySoldier:public Entity {
public:

	bool left=true;

	CharacterSystem character;

	StrawberrySoldier(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),70,69},cpos,{0,0},1}
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

		attack(U"Player", hitBox.getFigure(), 1);

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

	CookieSoldier(const Vec2& cpos) :Entity{ {U"Enemy"}, RectF{Arg::center(0,0),70,69},cpos,{0,0},1},
		character{ U"CharacterImages/cookie/cookie.json",U"CharacterImages/cookie/motion_cookie.txt",0.3,cpos,true,false }
	{
		character.addMotion(U"",true);
	}

	double speed = 0;

	void update()override {

		manager->stage->hit(&hitBox);

		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 5) {

			if (manager->get(U"Player")->pos.x < pos.x) {
				left = true;

				speed -= 600 * Scene::DeltaTime();
			}
			else if (manager->get(U"Player")->pos.x >= pos.x) {
				left = false;
				speed += 600 * Scene::DeltaTime();
			}
		}
		else {
			speed = 0;
		}

		speed = Clamp(speed, -200.0, 200.0);

		vel.x = speed;

		//プレイヤーに近すぎる場合
		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2) {
			left = false;
		}

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

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

		attack(U"Player", hitBox.getFigure(), 1);

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

class ItigoSlave :public Entity {
public:

	bool left = true;

	CharacterSystem character;

	ItigoSlave(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,15),70 * 1.5,69 * 1.5 },cpos,{0,0},2 }
		, character{ U"Characters/itigoSlave/itigoSlave.json" ,U"Characters/itigoSlave/motion.txt" ,0.3,cpos,true,false }
	{
		character.addMotion(U"walk", true);
	}

	Timer attackTimer{ 0.5s };

	void update()override {

		manager->stage->hit(&hitBox);
		if (not character.hasMotion(U"Muteki")) {
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

		if (not character.hasMotion(U"Muteki")) {

			attack(U"Player", hitBox.getFigure(), 1);

			if ((not character.hasMotion(U"attack")) && manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -60 : 60, 0))) {
				attackTimer.restart();
				character.addMotion(U"attack");
			}

			if (attackTimer.sF() == 0) {
				if (manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -25 : 25, 0))) {
					manager->get(U"Player")->damage(2, Vec2{ left ? -200 : 200,-20 });
				}
			}
		}

		character.update(pos, left);
	}

	void lateUpdate() {
		if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 0);
			manager->add(new CookieItem{ pos });
		}
	}

	virtual void damage(int32 n, const Vec2& force = {}) {
		if (not character.hasMotion(U"Muteki")) {
			character.removeMotion(U"attack");
			hp -= n;
			character.addMotion(U"Muteki");
			vel.y = force.y;
			vel.x = force.x * 1.5;
		}
	}

	void draw()const override {
		character.draw();
	}
};

class Kompeito :public Entity {
public:
	double velX;

	size_t type=Random(2);

	Kompeito(const Vec2& cpos,double velX,double velY=-300) :Entity{ U"Enemy", Circle{25},cpos,{0,0},1}, velX{velX}
	{
		vel.y = velY;
		TextureAsset::Register(U"金平糖0",U"Characters/cloud/金平糖/konpeitou.png");
		TextureAsset::Register(U"金平糖1", U"Characters/cloud/金平糖/konpeitouB.png");
		TextureAsset::Register(U"金平糖2", U"Characters/cloud/金平糖/konpeitouR.png");
	}

	void update()override {

		vel.x = velX;

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

	}

	void lateUpdate() {
		constexpr ColorF colorList[] = {Palette::Yellow,Palette::Blue,Palette::Red};
		if (not isActive()) {
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, colorList[type]);
		}
	}

	void draw()const override {
		TextureAsset(U"金平糖{}"_fmt(type)).resized(100).drawAt(pos);
	}
};

class CloudEnemy :public Entity {
public:

	CharacterSystem character;

	CloudEnemy(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),70 * 1.5,69 },cpos,{0,0},1 }
		, character{ U"Characters/cloud/cloud.json" ,U"Characters/cloud/motion.txt" ,0.5,cpos,true,false }
	{
		character.addMotion(U"walk", true);
		startY = cpos.y;

		time = Random(1.5, 2.5);
	}

	Timer attackTimer{ 0.5s };

	double accumulatedTime = 0;

	double startY = 0;

	double time = 0;

	void update()override {

		hitBox.update();

		if (manager->get(U"Player")->pos.x < pos.x) {
			vel.x -= 500 * Scene::DeltaTime();
		}
		else if (manager->get(U"Player")->pos.x >= pos.x) {
			vel.x += 500 * Scene::DeltaTime();
		}

		vel.x = Clamp(vel.x, -200.0, 200.0);

		pos.x += (vel.x + Periodic::Sine1_1(time, DataManager::get().time) * 100) * Scene::DeltaTime();

		pos.y = startY+ Periodic::Sine1_1(time,DataManager::get().time)*30;

		attack(U"Player", hitBox.getFigure(), 1);

		if (Abs(manager->get(U"Player")->pos.x - pos.x) < 70 * 6) {

			accumulatedTime += Scene::DeltaTime();
			constexpr double eventInterval = 2.5;
			if (eventInterval <= accumulatedTime)
			{
				manager->add(new Kompeito{ pos,vel.x+vel.x*Random(1.0)});
				character.addMotion(U"attack");

				accumulatedTime -= eventInterval;
			}
		}

		character.update(pos, 0<vel.x);
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

class Corn :public Entity {
public:

	bool left = false;

	bool startShake = false;

	bool corn = true;

	//爆発した後、地面に触れたか。
	bool touch = false;

	CharacterSystem character;

	Corn(const Vec2& cpos) :Entity{ {U"Enemy"}, Circle{30},cpos,{0,0},1 },
		character{ U"Characters/corn/corn.json",U"Characters/corn/motion.txt",0.3,cpos,true,false }
	{
		character.addMotion(U"walk", true);
	}

	void update()override {

		if (corn) {
			if(not startShake)
			if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 5) {

				pos = Math::SmoothDamp(pos, manager->get(U"Player")->pos, vel, 0.1, 50);
				left = vel.x < 0;
			}
			hitBox.update();


			if (startShake) {

				if (not character.hasMotion(U"explode")) {
					character.addMotion(U"change");
					DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200, HSV{ 20,1,1 });
					if ((manager->get(U"Player")->pos - pos).length() < 70 * 2) {
						if (pos.x < manager->get(U"Player")->pos.x) {
							manager->get(U"Player")->damage(1, Vec2{ 100,-20 });							
						}
						else {
							manager->get(U"Player")->damage(1, Vec2{ -100,-20 });
						}
					}

					if (pos.x < manager->get(U"Player")->pos.x) {
						vel = Vec2{ -600,-400 };

					}
					else {
						vel = Vec2{ 600,-400 };
					}


					corn = false;
				}
			}
			else if ((manager->get(U"Player")->pos - pos).length() < 70*1.5) {
				character.clearMotion();
				character.addMotion(U"explode");
				startShake = true;
			}
		}
		else {
			manager->stage->hit(&hitBox);

			if (hitBox.touch(Direction::right))
			{
				left = true;
			}
			else if (hitBox.touch(Direction::left)) {
				left = false;
			}

			if (hitBox.touch(Direction::down)) {
				touch = true;

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

			//着地前に歩くと爆発で吹き飛ばないため。
			if (touch) {
				if (left) {
					vel.x = -100;
				}
				else {
					vel.x = 100;
				}
			}

			hitBox.physicsUpdate();
			hitBox.update();

			attack(U"Player", hitBox.getFigure(), 1);

		}
		

		character.update(pos, left);
	}

	void lateUpdate()override {
		if (not isActive()) {
			if (corn) {
				character.clearMotion();
				startShake = true;
				hp = 1;
			}
			else {
				DataManager::get().effect.add<StarEffect>(pos, 50);
				manager->add(new CookieItem{ pos });
			}
		}
	}

	void draw()const override {
		//hitBox.draw(Palette::Orange);
		character.draw();
	}
};

class CookieMuti :public Entity {
public:

	Timer attackTimer{ 0.4s };

	bool left = false;

	CharacterSystem character;

	CookieMuti(const Vec2& cpos) :Entity{ {U"Enemy"}, RectF{Arg::center(0,0),50,100},cpos,{0,0},1 },
		character{ U"Characters/cookieMuti/cookieMuti.json",U"Characters/cookieMuti/motion.txt",0.4,cpos,true,false }
	{
	}

	void update()override {

		manager->stage->hit(&hitBox);

		if (manager->get(U"Player")->pos.x < pos.x) {
			left = true;
			vel.x = -200;
		}
		else if (manager->get(U"Player")->pos.x >= pos.x) {
			left = false;
			vel.x = 200;
		}

		//プレイヤーに近すぎる場合
		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2) {
			left = false;
		}

		hitBox.physicsUpdate();
		hitBox.update();

		if ((not character.hasMotion(U"attack")) && manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -60 : 60, 0).scaled(3, 1))) {
			character.removeMotion(U"walk");
			character.addMotion(U"attack");
			attackTimer.start();
		}

		if (not character.hasMotion(U"attack") && not character.hasMotion(U"walk")) {
			character.addMotion(U"walk");
		}

		if (attackTimer.sF() == 0) {
			attackTimer.reset();
			attack(U"Player", hitBox.Get_Box().movedBy(left ? -60 : 60, 0), 2);

		}
		else {
			attack(U"Player", hitBox.getFigure(), 1);
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

class CookieKaban :public Entity {
public:

	Timer attackTimer{ 0.4s };

	bool left = false;

	CharacterSystem character;

	CookieKaban(const Vec2& cpos) :Entity{ {U"Enemy"}, RectF{Arg::center(0,0),50,100},cpos,{0,0},1 },
		character{ U"Characters/cookieKaban/cookieKaban.json",U"Characters/cookieKaban/motion.txt",0.4,cpos,true,false }
	{
	}

	void update()override {

		manager->stage->hit(&hitBox);

		if (manager->get(U"Player")->pos.x < pos.x) {
			left = true;
			vel.x = -200;
		}
		else if (manager->get(U"Player")->pos.x >= pos.x) {
			left = false;
			vel.x = 200;
		}

		//プレイヤーに近すぎる場合
		if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2) {
			left = false;
		}

		hitBox.physicsUpdate();
		hitBox.update();

		if ((not character.hasMotion(U"attack")) && manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -20 : 20, 0).scaled(3, 1))) {
			character.removeMotion(U"walk");
			character.addMotion(U"attack");
			attackTimer.start();
		}

		if (not character.hasMotion(U"attack") && not character.hasMotion(U"walk")) {
			character.addMotion(U"walk");
		}

		if (attackTimer.sF() == 0) {
			attackTimer.reset();
			attack(U"Player", hitBox.Get_Box(), 2);
		}
		else {
			attack(U"Player", hitBox.Get_Box(), 1);
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

class DropCorn :public Entity {
public:

	bool left = false;

	bool startShake = false;

	double velX;

	bool corn = true;

	//爆発した後、地面に触れたか。
	bool touch = false;

	//攻撃されたか
	bool damaged = false;

	CharacterSystem character;

	DropCorn(const Vec2& cpos, double velX) :Entity{ {U"Enemy"}, Circle{30},cpos,{0,0},1 },
		character{ U"Characters/corn/corn.json",U"Characters/corn/motion.txt",0.3,cpos,true,false },velX{velX}
	{
	}

	void update()override {

		if (corn) {
			manager->stage->hit(&hitBox);

			if (hitBox.touch(Direction::down)|| damaged) {
				character.addMotion(U"change");
				DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200, HSV{ 20,1,1 });

				attack(U"Player", Circle{pos,70*2}, 1);

				if (pos.x < manager->get(U"Player")->pos.x) {
					vel = Vec2{ -600,-400 };

				}
				else {
					vel = Vec2{ 600,-400 };
				}
				corn = false;
			}

			vel.x = velX;
			hitBox.physicsUpdate();
			hitBox.update();
		}
		else {
			manager->stage->hit(&hitBox);

			if (hitBox.touch(Direction::right))
			{
				left = true;
			}
			else if (hitBox.touch(Direction::left)) {
				left = false;
			}

			if (hitBox.touch(Direction::down)) {
				touch = true;

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

			//着地前に歩くと爆発で吹き飛ばないため。
			if (touch) {
				if (left) {
					vel.x = -100;
				}
				else {
					vel.x = 100;
				}
			}

			hitBox.physicsUpdate();
			hitBox.update();

			attack(U"Player", hitBox.getFigure(), 1);

		}


		character.update(pos, left);
	}

	void lateUpdate()override {
		if (not isActive()) {
			if (corn) {
				damaged = true;
				hp = 1;
			}
			else {
				DataManager::get().effect.add<StarEffect>(pos, 50);
			}
		}
	}

	void draw()const override {
		//hitBox.draw(Palette::Orange);
		character.draw();
	}
};

class Zerosen :public Entity {
public:

	CharacterSystem character;

	Zerosen(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,-15),70 * 1.5,50 },cpos,{0,0},1 }
		, character{ U"Characters/zerosen/model.json" ,U"Characters/zerosen/motion.txt" ,1,cpos,false,false }
	{
		character.addMotion(U"susumu");
	}

	Timer attackTimer{ 0.5s };

	double accumulatedTime = 0;

	void update()override {

		if (manager->get(U"Player")->pos.x < pos.x) {
			vel.x -= 500 * Scene::DeltaTime();
		}
		else if (manager->get(U"Player")->pos.x >= pos.x) {
			vel.x += 500 * Scene::DeltaTime();
		}

		vel.x = Clamp(vel.x, -400.0, 400.0);

		pos.x += vel.x*Scene::DeltaTime();



		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

		if (Abs(manager->get(U"Player")->pos.x - pos.x) < 70 * 6) {

			accumulatedTime += Scene::DeltaTime();
			constexpr double eventInterval = 2.5;
			if (eventInterval <= accumulatedTime)
			{
				manager->add(new DropCorn{ pos,vel.x + vel.x * Random(1.0) });

				accumulatedTime -= eventInterval;
			}
		}

		character.update(pos, 0 < vel.x);
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

class SnowKnight:public Entity
{
public:
	CharacterSystem character;

	//右と左の定位置
	double rx, lx;
	double centerX=0;

	std::function<void()> f;
	std::function<void()>f2;

	bool left=false;
	double timer;

	HitBox* kenHitbox;

	Array<Entity*>summonList;//召喚したリスト

	SnowKnight(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(-20,40),70 * 1,70*4-30 },cpos,{0,0},1 }
		, character{ U"Characters/yukidarunaito/yukidarunaito.json" ,U"Characters/yukidarunaito/motion.txt" ,0.2,cpos,true,false }
	{	
		centerX = pos.x;

		rx = pos.x + 100;
		lx = pos.x - 100;

		auto& k = character.character.table[U"kenbox"];

		//最初は数秒何もしない
		f = []() {};
		f2 = []() {};
		timer = 3;

	}

	Timer attackTimer{ 0.5s };

	double accumulatedTime = 0;

	int32 yoroi = 1;

	Vec2 kenboxPos{0,0};
	Vec2 kenboxVel{ 0,0 };
	int32 kenboxHp{ 10 };

	Vec2 force{};

//剣を振ったか
	bool kenFlg = false;

	void update()override {

		manager->stage->hit(&hitBox);

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

		if (timer <= 0)
		{
			int32 type = 0;
			if (not summonList) {
				type = 2;
			}
			else {
				type = Random(0, 1);
			}

			switch (type)
			{
			case 0:
			{
				timer = 1;
				int d = 0;
				if (RandomBool())
				{
					d = rx - pos.x;
				}
				else
				{
					d = lx - pos.x;
				}
				left = d < 0;
				f = [=]
				{
					pos.x += (d / 1) * Scene::DeltaTime();
				};
				f2 = [=]
				{
					vel.x = 0;
					left != left;
				};
			}
				break;
			case 1:
			{

				left=manager->get(U"Player")->pos.x<pos.x;

				double maxTime = yoroi ? 5.8 : 7.0;

				timer = maxTime;

				kenFlg = false;

				//0.5 構え
				//1 ためる 何もしない
				//0.4　剣を前に突き出す
				//1.5 何もしない
				//0.2 切り上げ
				//1　何もしない
				character.addMotion(U"kiriage");
				f = [=]
				{
					double t = maxTime - timer;

					if (1.5 <= t and t <= 4.85)
					{
						const Quad ken = character.character.table[U"kenbox"].joint.getQuad();

						attack(U"Player",ken,1);

						if (not kenFlg) {
							AudioAsset{ U"剣を振る" }.play();
							kenFlg = true;
						}

						if (yoroi or true) {

							if (t <= 2.4) {
								if (left) {
									vel.x = -300;
								}
								else {
									vel.x = 300;
								}
							}

						}
						else {

							if (left) {
								vel.x = -400;
							}
							else {
								vel.x = 400;
							}
						}


					}
					else if(3.5<t and t<=3.6){
						if (hitBox.touch(Direction::down)) {
							vel.y = -300;
						}


					}


				};
				f2 = [=]
				{

				};
			}
				break;

			case 2:
			{
				timer = 1.6;
				character.addMotion(U"ageru");

				f = [=]
				{

				};

				f2 = [=]
				{
					for (auto i : step(5)) {
						Vec2 bornPos{ centerX +Random(-400,400),200};

											

						// 選択肢
						const Array<std::function<Entity* (Vec2)>> options =
						{
							[&](Vec2 pos) {
								StrawberrySoldier* ptr = new StrawberrySoldier{ pos };
								ptr->left = RandomBool();
								ptr->vel.y = -500;
								return ptr;
							},
							[&](Vec2 pos) {
								CookieSoldier* ptr = new CookieSoldier{ pos };
								ptr->left = RandomBool();
								ptr->vel.y = -500;
								return ptr;
							},
							[&](Vec2 pos) {
								Snowman* ptr = new Snowman{ pos };
								ptr->left = RandomBool();
								ptr->vel.y = -500;
								return ptr;
							},
						};

						// 選択肢に対応する確率分布
						DiscreteDistribution distribution{
						{
								3,2,1
						} };

						Entity* tmp = DiscreteSample(options, distribution)(bornPos);

						manager->add(tmp);
						summonList << tmp;
						DataManager::get().additiveEffect.add<ExplosionEffect>(bornPos, 60, Palette::Yellowgreen);
					}

					f = []() {};
					f2 = []() {};
					timer = 3;
				};

			}
				break;
			}

		}
		
		if (timer > 0)
		{
			timer -= Scene::DeltaTime();
			f();
			if (timer <= 0)
			{
				f2();
			}
		}

		character.update(pos, left);
		if (not yoroi) {
			character.character.table[U"kabuto"].joint.color = ColorF{ 1,0 };
			character.character.base->table[U"kabuto"].joint.color = ColorF{ 1,0 };
			character.character.table[U"tate"].joint.color = ColorF{ 1,0 };
			character.character.base->table[U"tate"].joint.color = ColorF{ 1,0 };
			character.character.table[U"yoroi"].joint.color = ColorF{ 1,0 };
			character.character.base->table[U"yoroi"].joint.color = ColorF{ 1,0 };

		}
	}

	void lateUpdate() override {
		summonList.remove_if([](Entity* entity) {return not entity->isActive(); });

			if (not isActive()) {
				DataManager::get().table.emplace(U"Clear");
				DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200);
				DataManager::get().additiveEffect.add<ExplosionEffect>(pos + Vec2{ 50,50 }, 100);
				DataManager::get().additiveEffect.add<ExplosionEffect>(pos - Vec2{ 50,50 }, 100);
			}
		/*if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 0);
			manager->add(new CookieItem{ pos });
		}*/
	}

	void damage(int32 n, const Vec2& _force = {}) override {

		if (not character.hasMotion(U"Muteki") and not character.hasMotion(U"YoroiMuteki")) {

			if (yoroi) {
				//5は突進のダメージ
				if (5 <= n) {
					yoroi -= 1;
				}
				if (yoroi <= 0) {
					character.addMotion(U"Nugeru");
				}
				character.addMotion(U"YoroiMuteki");
				force = _force;
				vel.y = force.y;
				vel.x = force.x*1.5;

				AudioAsset{ U"ヘッドドロップ" }.playOneShot();
			}
			else {
				hp -= n;
				character.addMotion(U"Muteki");
				force = _force;
				vel.y = force.y;
				vel.x = force.x;
			}

			if (5 <= n) {
				dynamic_cast<Player*>(manager->get(U"Player"))->stopRush();
				if (pos.x < manager->get(U"Player")->pos.x) {
					manager->get(U"Player")->damage(0, Vec2{ 200,-20 });
				}
				else {
					manager->get(U"Player")->damage(0, Vec2{ -200,-20 });
				}
			}
		}
		else if(character.hasMotion(U"YoroiMuteki")and 5 <= n){
			if (yoroi) {
				yoroi -= 1;
				if (yoroi <= 0) {
					character.addMotion(U"Nugeru");
				}
				character.addMotion(U"Muteki");
				force = _force;
				vel.y = force.y;
				vel.x = force.x * 1.5;
				AudioAsset{ U"ヘッドドロップ" }.playOneShot();
			}
		}
	}

	void draw()const override {
		//hitBox.Get_Box().draw(Palette::Red);

		character.draw();
	}
};

class SlaversCookie :public Entity {
public:

	Array<Entity*>summonListItigo;//召喚したリスト
	Entity* summonSnowLeft=nullptr;
	Entity* summonSnowRight=nullptr;

	bool left = true;

	std::function<void()> f;
	std::function<void()>f2;

	double timer;

	CharacterSystem character;

	size_t type=1;

	double rx, lx;

	SlaversCookie(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),50,100},cpos,{0,0},3 }
		, character{ U"Characters/cookieDoreisho/model.json" ,U"Characters/cookieDoreisho/motion.txt" ,0.4,cpos,true,false }
	{
		f = []() {};
		f2 = []() {};
		timer = 3;

		rx = pos.x + rect_size * 6.5;
		lx = pos.x - rect_size * 6.5;

	}

	void update()override {

		manager->stage->hit(&hitBox);

		if (rx<=pos.x)
		{
			left = true;
		}
		else if (pos.x<=lx) {
			left = false;
		}

		bool poleHit = false;
		if (DataManager::get().table.contains(U"PoleHit")) {
			DataManager::get().table.erase(U"PoleHit");
			poleHit = true;
			timer = 0;
		}


		if (timer <= 0) {

			if (type == 0) {

				type = Random(1, 3);

				if (not summonListItigo) {
					type = 1;
				}
			}
			else {
				type = 0;
			}

			if (poleHit) {
				type = 4;
			}

			switch (type)
			{
			case 0: {

				timer = 2;

				character.addMotion(U"walk", true);

				f = [&]() {
					if (left) {
						vel.x = -300;
					}
					else {
						vel.x = 300;
					}
				};

				f2 = [&]() {
					character.removeMotion(U"walk");
				};

			}break;
			case 1: {

				if (summonListItigo.size()<10) {
					for (auto i : step(2)) {
						Vec2 bornPos{};

						if (i == 0) {
							bornPos = { rect_size*1.5,pos.y + 200 };
						}
						else {
							bornPos = { DataManager::get().stageSize.x- rect_size * 1.5,pos.y + 200 };
						}


						Entity* tmp = new ItigoSlave{ bornPos };
						manager->add(tmp);
						summonListItigo << tmp;
						DataManager::get().additiveEffect.add<ExplosionEffect>(bornPos, 60, Palette::Yellowgreen);
					}
					character.addMotion(U"meirei");

					f = [&]() {

					};

					f2 = [&]() {
						character.removeMotion(U"meirei");
					};

					timer = 2;

				}
				else {
					timer = 0;
					f = []() {};
					f2 = []() {};
				}

			}break;
			case 2: {
				timer = 1;

				character.addMotion(U"nageire");


				f = [&]() {};

				f2 = [&]() {

						Vec2 bornPos{ pos.x+Random(-100,100),pos.y + 200};
						Entity* tmp = new DropCorn{ bornPos,100 * Random(1.0) };
						manager->add(tmp);
						//summonListItigo << tmp;
						DataManager::get().additiveEffect.add<ExplosionEffect>(bornPos, 60, Palette::Yellowgreen);
					f = [&]() {};
					f2 = [&]() {};
					timer = 1;
				};


			}break;
			case 3: {

				character.addMotion(U"meirei");

				f = [&]() {};
				f2 = [&]() {};
				timer = 0;

				if (not summonSnowLeft) {
					Vec2 bornPos{ rect_size * 5,pos.y + 200 };

					Entity* tmp = new Snowman{ bornPos };
					manager->add(tmp);
					summonSnowLeft = tmp;
					DataManager::get().additiveEffect.add<ExplosionEffect>(bornPos, 60, Palette::Yellowgreen);

					timer += 1.0;
				}
				if (not summonSnowRight) {
					Vec2 bornPos{ DataManager::get().stageSize.x - rect_size * 5,pos.y + 200 };

					Entity* tmp = new Snowman{ bornPos };
					manager->add(tmp);
					summonSnowRight = tmp;
					DataManager::get().additiveEffect.add<ExplosionEffect>(bornPos, 60, Palette::Yellowgreen);

					timer += 1.0;
				}

			}break;
			case 4: {
				character.clearMotion();
				character.addMotion(U"shobon");
				hp -= 1;

				f = [&]() {};
				f2 = [&]() {};
				timer = 3;

			}break;

			default:
				break;
			}
		}

		hitBox.physicsUpdate();
		hitBox.update();

		if (timer > 0)
		{
			timer -= Scene::DeltaTime();
			f();
			if (timer <= 0)
			{
				f2();
			}
		}

		character.update(pos, left);
	}

	void lateUpdate() {
		summonListItigo.remove_if([](Entity* entity) {return not entity->isActive(); });
		if (summonSnowLeft and not summonSnowLeft->isActive()) {
			summonSnowLeft = nullptr;
		}
		if (summonSnowRight and not summonSnowRight->isActive()) {
			summonSnowRight = nullptr;
		}

		if (not isActive()) {
			DataManager::get().table.emplace(U"Clear");
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200);
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos + Vec2{ 50,50 }, 100);
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos - Vec2{ 50,50 }, 100);
		}
	}

	void draw()const override {
		character.draw();
	}
};

class Captain :public Entity {
public:

	double accumulatedTime = 0;

	Array<Entity*>summonList;//召喚したリスト

	bool left = true;

	double center;

	Vec2 r, l;

	std::function<void()> f;
	std::function<void()>f2;

	double timer;

	CharacterSystem character;

	size_t type = 1;

	Captain(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,-30),230,100},cpos,{0,0},30 }
		, character{ U"Characters/sentyo/model.json" ,U"Characters/sentyo/motion.txt" ,1,cpos,true,false }
	{
		r = { pos.x + 400,pos.y};
		l = { pos.x - 400,pos.y };
		center = pos.x;

		f = []() {};
		f2 = []() {};
		timer = 3;
		character.addMotion(U"Mokumoku",true);

		//pos.y = 500;
	}

	void update()override;

	void lateUpdate() {
		summonList.remove_if([](Entity* entity) {return not entity->isActive(); });

		if (not isActive()) {
			DataManager::get().table.emplace(U"Clear");
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200);
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos + Vec2{ 50,50 }, 100);
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos - Vec2{ 50,50 }, 100);
		}
	}

	void draw()const override {
		character.draw();
		hitBox.Get_Box().drawFrame(5.0,Palette::Orange);
	}

	void damage(int32 n, const Vec2& force = {})override {
		if (not character.hasMotion(U"Muteki")) {
			hp -= n;
			character.addMotion(U"Muteki");
		}
	}
};

class GoalDoor:public Entity{
public:

	Timer timer{ 1s };

	GoalDoor(const Vec2& pos) :Entity{ U"Door",RectF{0,0,0,0},pos,{0,0},1 } {
		z = -100;
	}

	void update()override {
		if (RectF{ pos,rect_size * 2 }.intersects(DataManager::get().playerPos)) {
			timer.start();
			DataManager::get().table.emplace(U"Clear");
		}
	}

	void draw()const override{
		RectF{ pos,rect_size * 2 }.draw(Palette::Black);
		double d = Sin(timer.sF() * 80_deg+10_deg);
		TextureAsset(U"Door").resized(rect_size * 2).scaled(d,1).draw(pos);
		TextureAsset(U"Door").resized(rect_size * 2).scaled(d,1).mirrored().draw(pos+Vec2::UnitX()*(1-d)*2* rect_size);
	}
};
