#pragma once
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

	void lateUpdate()override {
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
			AudioAsset{ U"雪玉壊れる" }.playOneShot();
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

		Vec2 playerPos = manager->get(U"Player")->pos;
		if (Abs(playerPos.x - pos.x) < rect_size * 7+ 5 / 10.0 * Max(playerPos.y - pos.y, 0.0)) {
			attackMode = true;
			left = playerPos.x < pos.x;
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
		else
		{
			attackAccumlater = 1.5;
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

	static constexpr double attackTime = 0.5;
	double attackTimer= 0;

	void update()override {

		attackTimer -= Scene::DeltaTime();

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
				attackTimer = attackTime;
				character.addMotion(U"attack");
			}

			if (attackTimer<= 0) {
				if (manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -25 : 25, 0))) {
					manager->get(U"Player")->damage(2, Vec2{ left ? -200 : 200,-20 });
				}
			}
		}

		character.update(pos, left);
	}

	void lateUpdate()override {
		if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 0);
			manager->add(new CookieItem{ pos });
		}
	}

	void damage(int32 n, const Vec2& force, DamageType damageType)override {
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
		if (not TextureAsset::IsRegistered(U"金平糖0")) {
			TextureAsset::Register(U"金平糖0", U"Characters/cloud/金平糖/konpeitou.png");
			TextureAsset::Register(U"金平糖1", U"Characters/cloud/金平糖/konpeitouB.png");
			TextureAsset::Register(U"金平糖2", U"Characters/cloud/金平糖/konpeitouR.png");
		}
	}

	void update()override {

		vel.x = velX;

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

	}

	void lateUpdate()override {
		constexpr ColorF colorList[] = {Palette::Yellow,Palette::Blue,Palette::Red};
		if (not isActive()) {
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, colorList[type]);
		}
	}

	void draw()const override {
		TextureAsset(U"金平糖{}"_fmt(type)).resized(100).drawAt(pos);
	}

	bool isActive()override {
		return 0 < hp and pos.y < DataManager::get().stageSize.y + 200;
	}
};

class DanmakuKompeito :public Entity {
public:
	size_t type = Random(2);

	double timer;

	DanmakuKompeito(const Vec2& cpos, const Vec2& vel,double timeLim=20) :Entity{ U"Enemy", Circle{25*0.8},cpos,vel,1 },timer{timeLim}
	{
		if (not TextureAsset::IsRegistered(U"金平糖0")) {
			TextureAsset::Register(U"金平糖0", U"Characters/cloud/金平糖/konpeitou.png");
			TextureAsset::Register(U"金平糖1", U"Characters/cloud/金平糖/konpeitouB.png");
			TextureAsset::Register(U"金平糖2", U"Characters/cloud/金平糖/konpeitouR.png");
		}
	}

	void update()override {

		timer -= Scene::DeltaTime();

		pos += vel * Scene::DeltaTime();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);
	}

	void lateUpdate()override {
		constexpr ColorF colorList[] = { Palette::Yellow,Palette::Blue,Palette::Red };
		if (not isActive()) {
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, colorList[type]);
		}
	}

	void draw()const override {
		TextureAsset(U"金平糖{}"_fmt(type)).resized(100*0.8).rotated(timer*360_deg).drawAt(pos);
	}

	bool isActive()override {
		return 0 < hp and 0 < timer;
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

	void lateUpdate()override {
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
					AudioAsset{ U"ポップコーン爆発" }.playOneShot();

					if ((manager->get(U"Player")->pos - pos).length() < 70 * 2) {
						if (pos.x < manager->get(U"Player")->pos.x) {
							manager->get(U"Player")->damage(1, Vec2{ 100,-20 }, DamageType::UnBrakable);
						}
						else {
							manager->get(U"Player")->damage(1, Vec2{ -100,-20 }, DamageType::UnBrakable);
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
		character.draw();
	}
};

class CookieMuti :public Entity {
public:

	static constexpr double attackInterval = 0.4;
	double attackTimer=attackInterval;

	bool left = false;

	CharacterSystem character;

	CookieMuti(const Vec2& cpos) :Entity{ {U"Enemy"}, RectF{Arg::center(0,0),50,100},cpos,{0,0},1 },
		character{ U"Characters/cookieMuti/cookieMuti.json",U"Characters/cookieMuti/motion.txt",0.4,cpos,true,false }
	{
	}

	void update()override {

		attackTimer -= Scene::DeltaTime();

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
			attackTimer = attackInterval;
		}

		if (not character.hasMotion(U"attack") && not character.hasMotion(U"walk")) {
			character.addMotion(U"walk");
		}

		if (attackTimer<=0) {
			attackTimer = attackInterval;
			attack(U"Player", hitBox.Get_Box().movedBy(left ? -60 : 60, 0), 2, DamageType::UnBrakable);

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

	static constexpr double attackInterval = 0.4;
	double attackTimer = 0;

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
			attackTimer= attackInterval;
		}

		if (not character.hasMotion(U"attack") && not character.hasMotion(U"walk")) {
			character.addMotion(U"walk");
		}

		if (attackTimer<= 0) {
			attackTimer = attackInterval;
			attack(U"Player", hitBox.Get_Box(), 2, DamageType::UnBrakable);
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
				AudioAsset{ U"ポップコーン爆発" }.playOneShot();


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

	void lateUpdate()override {
		if (not isActive()) {
			DataManager::get().effect.add<StarEffect>(pos, 0);
			manager->add(new CookieItem{ pos });
		}
	}

	void draw()const override {
		character.draw();
	}
};

class GoalDoor:public Entity{
public:

	bool timerStart = false;
	double timer = 1 ;

	GoalDoor(const Vec2& pos) :Entity{ U"Door",RectF{0,0,0,0},pos,{0,0},1 } {
		z = -100;
	}

	void update()override {
		if (RectF{ pos,rect_size * 2 }.intersects(DataManager::get().playerPos)) {
			timerStart = true;
			DataManager::get().table.emplace(U"Clear");
		}

		if (timerStart) {
			timer -= Scene::DeltaTime();
			timer = Max(timer, 0.0);
		}
	}

	void draw()const override{
		RectF{ pos,rect_size * 2 }.draw(Palette::Black);
		double d = Sin(timer * 80_deg+10_deg);
		TextureAsset(U"Door").resized(rect_size * 2).scaled(d,1).draw(pos);
		TextureAsset(U"Door").resized(rect_size * 2).scaled(d,1).mirrored().draw(pos+Vec2::UnitX()*(1-d)*2* rect_size);
	}
};

class FallingRocks :public Entity {
public:
	FallingRocks(const Vec2& cpos) :Entity{ U"Rocks", RectF{ Arg::center(0,0),rect_size / 2.0 },cpos,{0,0},1 }
	{

	}

	void update()override {

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1,DamageType::UnBrakable);
	}

	void draw()const override {


		TextureAsset{ U"PointedCorn" }.resized(rect_size).drawAt(pos);
	}

	bool isActive()override {
		return pos.y < DataManager::get().stageSize.y + 200;
	}
};

class RollingRocks :public Entity {
public:

	RollingRocks(const Vec2& cpos) :Entity{ U"Rocks", Circle{ 0,0,rect_size },cpos,{0,0},1 }
	{

	}

	void update()override {

		manager->stage->hit(&hitBox);

		pos.x -= Scene::DeltaTime() * 200;

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);


		if (hitBox.touch(Direction::left)) {
			hp = 0;
		}

	}

	void lateUpdate()override {


		if (not isActive())
		{
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 100, Palette::Darkgray);
		}

	}

	void draw()const override {

		TextureAsset{ U"Doughnut" }.resized(rect_size*2).rotated(-DataManager::get().time*120_deg).drawAt(pos);
	}

	bool isActive()override {
		return 0<hp and pos.y < DataManager::get().stageSize.y + 200;
	}
};
