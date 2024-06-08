#pragma once
#include"Common.h"
#include"Entity.h"
#include"Effect.h"
#include"CharacterSystem.h"
#include"SimpleAction.h"
#include"BGMManager.hpp"
#include"MagicEffect.h"

class ClosedUmbrella:public Entity {
public:
	CharacterSystem character;

	double angle;

	double speed;

	double time=0;

	bool effectFlg = false;

	static constexpr double eventInterval = 0.05;
	double accumulatedTime = 0.0;

	ClosedUmbrella(const Vec2& cpos,double angle,double speed) :Entity{ U"Enemy", RectF{Arg::center(0,-5),40,150},cpos,{0,0},1 }
		, character{ U"Characters/bitter/umbrella.json" ,U"Characters/bitter/umbrellaMotion.txt" ,0.3,cpos,false,false },angle{angle},speed{speed}
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
				DataManager::get().additiveEffect.add<MagicEffect>(Vec2{ pos + Vec2{15,-70} }, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() });
				accumulatedTime -= eventInterval;
			}
		}

		character.update(pos, false);
	}

	void lateUpdate() {


	}

	void draw()const override {

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

	int32 type=0;

	LastBoss(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,-5),40,150},cpos,{0,0},1 }
		, character{ U"Characters/bitter/model1.json" ,U"Characters/bitter/motion1.txt" ,0.3,cpos,false,false }
	{


	}

	bool kickFlg = false;

	bool umbrellaFlg = false;

	void update()override;

	void lateUpdate() {


	}

	void draw()const override {

		//hitBox.draw(Palette::Orange);

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


