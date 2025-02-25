#pragma once
#include"Common.h"
#include"Entity.h"
#include"Effect.h"
#include"CharacterSystem.h"
#include"SimpleAction.h"
#include"BGMManager.hpp"

#include"MyInput.h"

class Hadouken:public Entity {
public:

	double angle = 0_deg;

	double timer = 0.4;

	CharacterSystem character;

	Hadouken(const Vec2& _pos, double angle) :Entity {U"PlayerAttack", RectF{Arg::center(0,0),50,50},_pos,{0,0},1}, angle{angle}
		, character{ U"Characters/Hadouken/Hadouken.json",U"Characters/Hadouken/motion.txt",0.25,_pos,false }
	{
		character.addMotion(U"");
		z = 100;
	}

	void update()override {

		timer -= Scene::DeltaTime();

		bool collisionFlg = false;

		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::left) || hitBox.touch(Direction::right) || hitBox.touch(Direction::up) || hitBox.touch(Direction::down)) {
			collisionFlg = true;
		}

		if (timer<=0) {
			hp = 0;
		}

		pos += OffsetCircular{ {0,0},Scene::DeltaTime() * 700,angle };

		for (auto& entity : manager->getArray(U"Enemy")) {
				if (entity->hitBox.intersects(hitBox)) {
					entity->damage(1, OffsetCircular{ {0,0},700,angle });
					collisionFlg = true;

					break;
				}
		}

		if (collisionFlg) {
			hp = 0;
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos,35,HSV{ -20,0.8,1 });
			AudioAsset{ U"パンチヒット" }.playOneShot();
		}

		hitBox.update();

		character.character.joint->angle = angle-90_deg;
		character.update(pos, false);
	}

	void draw()const override {
		character.draw();
	}
};

class Player :public Entity
{
public:
	double acc1 = 0;

	bool jump = false;//描画用のジャンプフラグ

	Timer damageTimer{ 1s };

	bool rushMode = false;

	bool left = false;

	int32 itemCount = 0;

	Vec2 force{};

	CharacterSystem character;

	ActionManager actMan{};

	double speed = 400;

	static constexpr RectF defaultBody{ Arg::center(0,0),40,130 };

	double crushedTimer = 0;


	Vec2 lastTouchPos{};

	MyInput jumpKey;
	MyInput rightKey;
	MyInput leftKey;
	MyInput downKey;
	MyInput attackKey;

	void setDataP(GameData* data) {
		jumpKey.setKey(data->jumpKey);
		rightKey.setKey(data->rightKey);
		leftKey.setKey(data->leftKey);
		downKey.setKey(data->downKey);
		attackKey.setKey(data->attackKey);
	}

	Player(const Vec2& cpos);

	bool canSummer = true;

	bool punch = false;

	bool walk=false;

	void update()override;

	bool die = false;

	bool summerHited = false;

	void draw()const override;

	void damage(int32 n, const Vec2& force = {}, DamageType damageType = DamageType::Brakable)override;

	bool isActive()override {
		return true;
	}

	void stopRush() {
		rushMode = false;
	}

	void KeyUpdate(bool jump,bool down,bool left, bool right,bool attack)
	{
		jumpKey.update(jump);
		downKey.update(down);
		leftKey.update(left);
		rightKey.update(right);
		attackKey.update(attack);
	}


};
