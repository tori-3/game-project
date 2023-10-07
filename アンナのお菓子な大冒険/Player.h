#pragma once
#include"Entity.h"
#include"Effect.h"
#include"CharacterSystem.h"

class Hadouken:public Entity {
public:

	double angle = 0_deg;

	Timer timer{ 0.4s,StartImmediately::Yes };

	CharacterSystem character;

	Hadouken(const Vec2& _pos, double angle) :Entity {U"PlayerAttack", RectF{Arg::center(_pos),50,50},_pos,{0,0},1}, angle{angle}
		, character{ U"CharacterImages/Hadouken/Hadouken.json",U"CharacterImages/Hadouken/motion.txt",0.25,_pos,false }
	{
		character.addMotion(U"");
	}

	void update()override {
		bool collisionFlg = false;

		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::left) || hitBox.touch(Direction::right) || hitBox.touch(Direction::up) || hitBox.touch(Direction::down)) {
			collisionFlg = true;
		}

		if (timer<=0s) {
			hp = 0;
		}

		pos += OffsetCircular{ {0,0},Scene::DeltaTime() * 700,angle };

		for (auto& entity : manager->getArray(U"Enemy")) {
				if (entity->hitBox.intersects(hitBox)) {
					entity->damage(100, OffsetCircular{ {0,0},Scene::DeltaTime() * 1000,angle });
					collisionFlg = true;
					break;
				}
		}

		if (collisionFlg) {
			hp = 0;
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos,35,HSV{ -20,0.8,1 });
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
	Timer backTimer{ 0.2s };

	CharacterSystem character;

	Player(const Vec2& cpos) : Entity{ U"Player", RectF{Arg::center(0,0),60,130},cpos,{0,0},3}
		,character{ U"CharacterImages/annna/annna.json",U"CharacterImages/annna/motion.txt",0.25,cpos,false }
	{

	}

	bool walk=false;

	void update()override {

		bool tmpWalk = walk;
		walk = false;

		bool tmpRush = rushMode;

		bool tmpJump = jump;

		manager->stage->hit(&hitBox);

		if (jump && hitBox.touch(Direction::down))jump = false;

		if (KeyW.down()) {
			if (hitBox.touch(Direction::down))
			{
				vel.y = -700.0; jump = true;
				character.removeMotion(U"Walk");
				character.addMotion(U"Jump", false);
			}
		}//ジャンプ

		if (KeyA.pressed()) {
			if (not hitBox.touch(Direction::left))vel.x = -400;
			left = true;
			walk = true;
		}//左
		else if (KeyD.pressed()) {
			if (not hitBox.touch(Direction::right))vel.x = 400;
			left = false;
			walk = true;
		}//右

		if (MouseR.pressed()&&10<=itemCount) {
			itemCount = 0;
			rushMode = true;
			//本当はダッシュ
			character.addMotion(U"Walk");
		}
		//突進のとき
		if (rushMode) {

			if (hitBox.touch(Direction::left) || hitBox.touch(Direction::right)) {
				rushMode = false;
			}

			if (left) {
				vel.x = -1000;
			}
			else {
				vel.x = 1000;
			}

			for (auto& entity : manager->getArray(U"Enemy")) {
				if (entity->hitBox.intersects(hitBox)) {
					entity->damage(100);
				}
			}
		}

		if (KeyD.down()) {
			character.addMotion(U"Walk",true);
		}

		//if (jump==false&& tmpJump==true) {
		//	if (walk) {
		//		character.addMotion(U"Walk");
		//	}
		//	else {
		//		character.addMotion(U"Standing");
		//	}
		//}

		//if (not jump&&walk != tmpWalk) {
		//	if (walk) {
		//		character.addMotion(U"Walk");
		//	}
		//	else {
		//		character.addMotion(U"Standing");
		//	}
		//}


		if (MouseL.down()) {
			manager->add(new Hadouken{ pos,(Cursor::PosF() - pos).getAngle() });
		}

		if (backTimer.isRunning() && 0s < backTimer) {
			vel += force;
		}



		hitBox.physicsUpdate();
		hitBox.update();



		if (hitBox.touch(Direction::down) && hitBox.touch(Direction::up) && hitBox.touch(Direction::left) && hitBox.touch(Direction::right)) {
			hp = 0;
		}

		for (auto& entity : manager->getArray(U"Item")) {
				if (entity->hitBox.intersects(hitBox)) {
					itemCount++;
					entity->damage(1);
				}
		}



		//if (not rushMode) {

		//	//モーション制御
		//	if ((not tmpWalk) && walk) {
		//		character.addMotion(U"Walk");
		//	}

		//	if (tmpWalk && (not walk)) {
		//		character.removeMotion(U"Walk");
		//	}
		//}

		//if ((not rushMode) && tmpRush) {
		//	if (walk) {
		//		character.addMotion(U"Walk");
		//	}
		//	else {
		//		character.removeMotion(U"Walk");
		//	}
		//}

		Print << character.character.joint->size;
		character.update(pos, left);
		character.character.touchGround(hitBox.Get_Box().boundingRect().bottomY());
		Print << character.character.joint->size;
	}

	void draw()const override {
		character.draw();
	}

	void damage(int32 n, const Vec2& force = {})override {

		if (not rushMode) {
			if ((damageTimer.isRunning() && 0s < damageTimer)) {
				return;
			}
			else {
				hp -= n; damageTimer.restart();
				this->force = force;
				backTimer.restart();
				vel += force;

				character.addMotion(U"Damage");
			}
		}
	}

	bool isActive()override {
		return true;
	}
};

