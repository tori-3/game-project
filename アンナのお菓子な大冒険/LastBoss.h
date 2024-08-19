#pragma once
#include"Common.h"
#include"Entity.h"
#include"Effect.h"
#include"CharacterSystem.h"
#include"SimpleAction.h"
#include"BGMManager.hpp"
#include"MagicEffect.h"
#include"MagicCircle.h"
#include"LaserEffect.h"
#include"Enemy.h"

class ClosedUmbrella:public Entity {
public:
	CharacterSystem character;

	double angle;

	double speed;

	double time=0;

	bool effectFlg = false;

	static constexpr double eventInterval = 0.05;
	double accumulatedTime = 0.0;

	ClosedUmbrella(const Vec2& cpos,double angle,double speed) :Entity{ U"Umbrella", RectF{Arg::center(0,-5),40,150},cpos,{0,0},1 }
		, character{ U"Characters/bitter/umbrella2.json" ,U"Characters/bitter/umbrellaMotion.txt" ,0.3,cpos,false,false },angle{angle},speed{speed}
	{
		character.character.joint->angle = angle;
		character.character.joint->color.a = 0.0;

		TextureAsset::Register(U"MagicEffect0", 0xF810_icon, 50);
		TextureAsset::Register(U"MagicEffect1", 0xF786_icon, 50);
		TextureAsset::Register(U"MagicEffect2", 0xF563_icon, 50);
		TextureAsset::Register(U"MagicEffect3", 0xF005_icon, 50);

	}

	void update()override {

		time += Scene::DeltaTime()*2;
		character.character.joint->color.a = Min(time, 1.0);

		hitBox.update();
		character.character.joint->angle = angle;
		pos += Vec2{ OffsetCircular{ {0,0},speed,angle}}*Scene::DeltaTime();

		if (effectFlg) {

			accumulatedTime += Scene::DeltaTime();

			if (eventInterval <= accumulatedTime)
			{
				DataManager::get().additiveEffect.add<MagicEffect>(Vec2{ pos }, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() });
				accumulatedTime -= eventInterval;
			}
		}

		attack(U"Player", character.character.table[U"umb"].joint.getQuad2(), 1);

		character.update(pos, false);
	}

	void lateUpdate() {


	}

	bool isActive()override {
		return time < 30;
	}

	void draw()const override {
		character.draw();
	}



};



class ChaseUmbrella :public Entity {
public:
	CharacterSystem character;

	double angle;

	double targetAngle;

	double speed;

	double time = 0;

	static constexpr double eventInterval = 0.05;
	double accumulatedTime = 0.0;

	bool rotateFlg = false;

	static constexpr double timeLim = 10.0;

	ChaseUmbrella(const Vec2& cpos,double angle, double speed) :Entity{ U"Umbrella", Circle{0,0,50},cpos,{0,0},1 }
		, character{ U"Characters/bitter/umbrella2.json" ,U"Characters/bitter/umbrellaMotion.txt" ,0.3,cpos,false,false }, angle{ angle }, speed{ speed }
	{
		character.character.joint->angle = angle;
		character.character.joint->color.a = 0.0;

		TextureAsset::Register(U"MagicEffect0", 0xF810_icon, 50);
		TextureAsset::Register(U"MagicEffect1", 0xF786_icon, 50);
		TextureAsset::Register(U"MagicEffect2", 0xF563_icon, 50);
		TextureAsset::Register(U"MagicEffect3", 0xF005_icon, 50);
	}

	void update()override {

		time += Scene::DeltaTime();
		
		character.character.joint->angle = angle;

		if (not rotateFlg) {
			pos += Vec2{ OffsetCircular{ {0,0},speed,angle} }*Scene::DeltaTime();

			accumulatedTime += Scene::DeltaTime();

			if (eventInterval <= accumulatedTime)
			{
				DataManager::get().additiveEffect.add<MagicEffect>(Vec2{ pos }, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() });
				accumulatedTime -= eventInterval;
			}
		}
		

		manager->stage->hit(&hitBox);
		hitBox.update();

		if (hitBox.touchAny()) {
			targetAngle=(manager->get(U"Player")->pos-pos).getAngle();

			constexpr double range = 15_deg;

			if (InRange(targetAngle,90_deg- range,90_deg)) {
				targetAngle = 90_deg - range;
			}
			else if (InRange(targetAngle, 90_deg, 90_deg + range)) {
				targetAngle = 90_deg + range;
			}
			else if (InRange(targetAngle, -90_deg ,-90_deg + range)) {
				targetAngle = -90_deg + range;
			}
			else if (InRange(targetAngle, -90_deg-range, -90_deg)) {
				targetAngle = -90_deg - range;
			}

			rotateFlg = true;
		}

		if (rotateFlg) {
			angle = linerMove(angle, targetAngle, 360_deg);


			accumulatedTime += Scene::DeltaTime();

			if (eventInterval <= accumulatedTime)
			{
				DataManager::get().additiveEffect.add<MagicEffect>(pos, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() }, Random(360_deg), 300);
				accumulatedTime -= eventInterval;
			}

			if (angle == targetAngle) {
				rotateFlg = false;
			}

		}

		if (timeLim-1< time)
		{
			character.character.joint->color.a = timeLim-time;
		}

		attack(U"Player", character.character.table[U"umb"].joint.getQuad2(), 1);

		character.update(pos, false);
	}

	void lateUpdate() {


	}

	bool isActive()override {
		return time < timeLim;
	}

	void draw()const override {
		character.draw();
	}

	double linerMove(double pos, double target, double speed, double dt = Scene::DeltaTime()) {

		double d = speed * dt;

		if (pos < target) {

			if (target < pos + d) {
				return target;
			}
			else {
				return pos + d;
			}
		}
		else {

			if (pos - d < target) {
				return target;
			}
			else {
				return pos - d;
			}
		}
	}

};



class EnemyUmbrella :public Entity {
public:
	CharacterSystem character;

	Entity* child;

	EnemyUmbrella(const Vec2& pos, Entity* child) :Entity{ U"Umbrella", RectF{Arg::center(0,-5),40,150},pos,{0,0},1 },child { child }
	, character{ U"Characters/bitter/umbrella2.json" ,U"Characters/bitter/umbrellaMotion.txt" ,0.3,pos,false,false }
	{
		character.addMotion(U"open");
	}

	void update()override
	{
		pos.y += child?(Scene::DeltaTime() * 200):( - Scene::DeltaTime() * 200);
		hitBox.update();

		character.update(pos, false);
	}

	void lateUpdate()override
	{
		if (child) {
			child->pos = pos + Vec2{0,40};
			child->vel = {};

			if (child->hitBox.touch(Direction::down))
			{
				child=nullptr;
				character.addMotion(U"close");
			}
			else if (not child->isActive())
			{
				child = nullptr;
				character.addMotion(U"close");
			}
		}
	}

	bool isActive()override {
		return child or -300 < pos.y;
	}

	void draw()const override
	{
		character.draw();
	}
};




class LastBoss :public Entity {
public:

	bool left = true;

	CharacterSystem character;

	std::function<void()> updateFunc;
	std::function<void()> endFunc;

	double timer;

	bool floatFlg = false;

	enum class State{kick,stand,throwUmbrella, enemyFalls, umbrellaShot, reflectionUmbrella, kompeitoGalaxyJump, kompeitoGalaxy, masterSparkPreJump, masterSparkJump, masterSparkWait, masterSpark,} type=State::kick;

	MagicCircle magicCircle;

	double accumulatedTime = 0.0;

	LastBoss(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,-5),40,150},cpos,{0,0},100 }
		, character{ U"Characters/bitter/model1.json" ,U"Characters/bitter/motion1.txt" ,0.3,cpos,false,false }
	{
		TextureAsset::Register(U"MagicEffect0", 0xF810_icon, 50);
		TextureAsset::Register(U"MagicEffect1", 0xF786_icon, 50);
		TextureAsset::Register(U"MagicEffect2", 0xF563_icon, 50);
		TextureAsset::Register(U"MagicEffect3", 0xF005_icon, 50);
	}

	bool kickFlg = false;

	bool umbrellaFlg = false;

	int32 throwUmbrellaCount = 0;

	bool isLastSpart()
	{
		return hp<= 20;
	}

	void update()override;

	void lateUpdate() {


	}

	void draw()const override {

		magicCircle.draw();

		character.draw();
	}

	void changeDirection() {
		left = (manager->get(U"Player")->pos.x < pos.x);
	}


	double linerMove(double pos, double target, double speed, double dt = Scene::DeltaTime()) {

		double d = speed * dt;

		if (pos < target) {

			if (target < pos + d) {
				return target;
			}
			else {
				return pos + d;
			}
		}
		else {

			if (pos - d < target) {
				return target;
			}
			else {
				return pos - d;
			}
		}
	}
};


