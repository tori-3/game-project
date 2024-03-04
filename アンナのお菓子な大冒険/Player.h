﻿#pragma once
#include"Entity.h"
#include"Effect.h"
#include"CharacterSystem.h"
#include"SimpleAction.h"

class Hadouken:public Entity {
public:

	double angle = 0_deg;

	Timer timer{ 0.4s,StartImmediately::Yes };

	CharacterSystem character;

	Hadouken(const Vec2& _pos, double angle) :Entity {U"PlayerAttack", RectF{Arg::center(0,0),50,50},_pos,{0,0},1}, angle{angle}
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
	Timer backTimer{ 0.2s };

	CharacterSystem character;

	ActionManager actMan{};

	double speed = 400;

	static constexpr RectF defaultBody{ Arg::center(0,0),40,130 };

	GameData* data=nullptr;

	void setDataP(GameData* _data) {
		data = _data;
	}

	Player(const Vec2& cpos) : 
		character{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.25,cpos,false },
		Entity{ U"Player",defaultBody ,cpos,{0,0},5}
	{
		
		actMan.add(U"Walk", {
			.startCondition = [&]() {
				return hitBox.touch(Direction::down) and (not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Landing",U"Shagamu",U"Sliding",U"Punch",U"Summer",U"HeadDropLanding",U"HeadDrop",U"Damage")) and (data->leftKey.pressed() or data->rightKey.pressed());
			},
			.start = [&]() {
				AudioAsset{U"足音"}.play();
				character.addMotion(U"Walk",true);
			},
			.update = [&](double t) {
				return not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Landing",U"Shagamu",U"Sliding",U"Punch",U"HeadDrop") and (data->leftKey.pressed() or data->rightKey.pressed());
			},
			.end = [&]() {
				AudioAsset{U"足音"}.stop(0.5s);
				character.removeMotion(U"Walk");
			}
		});

		actMan.add(U"PreJump", {
			.startCondition = [&]() {
				return hitBox.touch(Direction::down) and data->jumpKey.down() and not actMan.hasActive(U"Sliding",U"Summer",U"Damage",U"Jump");
			},
			.start = [&]() {
				AudioAsset{U"ジャンプ"}.playOneShot();
				if(not actMan.hasActive(U"Rush"))character.addMotion(U"PreJump");
			},
			.update = [&](double t) {
				return t<(1/30.0);				
			},
			.end = [&]() {
				actMan.start(U"Jump");
			}
		});

		actMan.add(U"Jump", {
			.start = [&]() {
				if (not actMan.hasActive(U"Rush"))character.addMotion(U"Jump");
				vel.y = -700;
			},
			.update = [&](double t) {
				return not (0 < vel.y);
			},
			.end = [&]() {
				character.removeMotion(U"Jump");
			} 
		});

		actMan.add(U"Rush", {
			.startCondition = [&]() {

				return (2s<data->attackKey.pressedDuration()) && 10 <= itemCount and not actMan.hasActive(U"Sliding",U"Damage") and hitBox.touch(Direction::down);
			},
			.start = [&]() {
				itemCount = 0;

				speed = 1000;
				character.addMotion(U"Tosshin",true);
				AudioAsset{ U"突進足音" }.play();
				AudioAsset{ U"風" }.play();
			},
			.update = [&](double t) {

				if (hitBox.touch(Direction::down)) {
					AudioAsset{ U"突進足音" }.play();
				}
				else {
					AudioAsset{ U"突進足音" }.stop(0.1s);
				}


				if (attackDamaged(U"Enemy",  character.character.table.at(U"Hitbox").joint.getQuad(), 5, 1500)) {
					AudioAsset{ U"突進衝突" }.playOneShot();
				}
				if (hitBox.touch(Direction::left)) {
					force= Vec2{ 300,-400 };
					actMan.start(U"Damage");

					return false;
				}
				else if (hitBox.touch(Direction::right)) {
					force = Vec2{ -300,-400 };
					actMan.start(U"Damage");
					return false;
				}
				else {
					return true;
				}

				//return not (hitBox.touch(Direction::left) || hitBox.touch(Direction::right));
			},
			.end = [&]() {
				character.removeMotion(U"Tosshin");
				actMan.start(U"HeadDropMuteki");
				AudioAsset{ U"突進足音" }.stop(0.1s);
				AudioAsset{ U"風" }.stop(0.1s);
				AudioAsset{ U"突進衝突" }.playOneShot();
				speed = 400;
			}
		});

		actMan.add(U"Falling", {
			.startCondition = [&]() {
				return 0<vel.y and not hitBox.touch(Direction::down) and not actMan.hasActive(U"Summer",U"HeadDrop",U"Damage",U"Rush");
			},
			.start = [&]() {
				character.addMotion(U"Falling");
			},
			.update = [&](double t) {
				return 0 < vel.y and not hitBox.touch(Direction::down) and not actMan.hasActive(U"HeadDrop");
			},
			.end = [&]() {
				character.removeMotion(U"Falling");
				if (not data->downKey.pressed()) {
					actMan.start(U"Landing");
				}
			}
		});

		actMan.add(U"Landing", {
			.start = [&]() {
				character.addMotion(U"Landing");
				AudioAsset{ U"着地" }.playOneShot();
			},
			.update = [&](double t) {
				if (t < 0.05) {
					speed = 200;
				}
				else {
					speed = 400;
				}
				return character.hasMotion(U"Landing") and not data->jumpKey.down() and not data->downKey.pressed() and not actMan.hasActive(U"Rush");
			},
			.end = [&]() {
				speed = 400;
				character.removeMotion(U"Landing");
			}
		});


		actMan.add(U"Standing", {
			.startCondition = [&]() {
				return not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Walk",U"Landing",U"Shagamu",U"Sliding",U"Punch",U"Summer",U"HeadDropLanding",U"Damage") and hitBox.touch(Direction::down);
			},
			.start = [&]() {
				character.addMotion(U"Standing");
			},
			.update = [&](double t) {
				return not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Walk",U"Landing",U"Shagamu",U"Punch",U"HeadDropLanding");
			},
			.end = [&]() {
				character.removeMotion(U"Standing");
			}
		});

		actMan.add(U"Shagamu", {
			.startCondition = [&]() {
				return data->downKey.pressed() and hitBox.touch(Direction::down) and not actMan.hasActive(U"Sliding",U"Summer",U"Damage",U"Landing",U"HeadDropLanding",U"HeadDrop");
			},
			.start = [&]() {
				character.addMotion(U"Shagamu");
				hitBox.setFigure(RectF{ Arg::center(0,defaultBody.h /4.0),defaultBody.w,defaultBody.h /2.0 });
			},
			.update = [&](double t) {
				speed = 0;
				return data->downKey.pressed() and not data->jumpKey.pressed() and not data->attackKey.pressed();
			},
			.end = [&]() {
				character.removeMotion(U"Shagamu");
				speed = 400;

				hitBox.setFigure(defaultBody);

				if (data->attackKey.pressed() and hitBox.touch(Direction::down)) {
					actMan.start(U"Sliding");
				}
			}
		});

		//次回スライディングが壁にぶつかったときに止まるようにする。
		actMan.add(U"Sliding", {
			.start = [&]() {
				character.addMotion(U"Sliding");
				hitBox.setFigure(RectF{ Arg::center(0,130 / 4.0),60,130 / 2.0 });
				AudioAsset{ U"スライディング" }.playOneShot();
			},
			.update = [&](double t) {

				attack(U"Enemy", character.character.table.at(U"Hitbox").joint.getQuad(), 1, 1000);

				speed = 600 * (1 - t / 0.8);

				if (t < 0.8) {
					return true;
				}
				else {
					return false;
				}
			},
			.end = [&]() {
				speed = 400;
				character.removeMotion(U"Sliding");
				hitBox.setFigure(RectF{ Arg::center(0,0),60,130 });
			}
		});

		actMan.add(U"Punch", {
			.startCondition = [&]() {
				return data->attackKey.down() and actMan.hasActive(U"Standing",U"Walk",U"Summer") and hitBox.touch(Direction::down);
			},
			.start = [&]() {
				character.addMotion(U"Punch");
				punch = false;
				speed = 70;
				AudioAsset{ U"パンチ" }.playOneShot();
			},
			.update = [&](double t) {

				if (not punch) {
					if (0.07<t) {
						//発射
						punch = true;
						manager->add(new Hadouken{ pos,left?-90_deg:90_deg });
					}
					return true;
				}
				return character.hasMotion(U"Punch") and not data->jumpKey.down();
			},
			.end = [&]() {
				character.removeMotion(U"Punch");
				speed = 400;
			}
		});

		actMan.add(U"Summer", {
			.startCondition = [&]() {
				return data->attackKey.down()and not hitBox.touch(Direction::down) and not actMan.hasActive(U"Punch",U"Shagamu",U"Sliding",U"Damage",U"Rush") and canSummer;
			},
			.start = [&]() {
				AudioAsset{U"サマーソルト"}.playOneShot();
				character.addMotion(U"Summer");
				vel.y = -500.0;
			},
			.update = [&](double t) {
				if (attack(U"Enemy",Circle{ pos,70 },1,200)) {
					AudioAsset{ U"サマーソルトヒット" }.playOneShot();
				}
				return character.hasMotion(U"Summer");
			},
			.end = [&]() {
				canSummer = false;
				character.removeMotion(U"Summer");
			}
		});

		actMan.add(U"HeadDrop", {
			.startCondition = [&]() {
				return data->downKey.down() and not actMan.hasActive(U"Summer",U"Damage") and not hitBox.touch(Direction::down);
			},
			.start = [&]() {
				character.addMotion(U"HeadDrop");
				speed = 0;
			},
			.update = [&](double t) {

				for (auto enemy: attack(U"Enemy", hitBox.Get_Box(), 1, 200)) {
					if (enemy->isActive()) {
						actMan.start(U"Jump");
						actMan.start(U"HeadDropMuteki");
						Motion m;
						m.add(new RotateTo{ U"body",40_deg,0 });
						character.character.addMotion(U"Rotate", m);
						speed = 400;
						return false;
					}
				}

				if (t < 0.7) {
					vel.y = 0.0;
				}
				else {
					vel.y = 1200.0;
				}
				return not hitBox.touch(Direction::down);
			},
			.end = [&]() {
				if(hitBox.touch(Direction::down))actMan.start(U"HeadDropLanding");
				character.removeMotion(U"HeadDrop");

				AudioAsset{ U"ヘッドドロップ" }.playOneShot();
			}
		});

		//一瞬無敵　HeadDrop以外にも使う
		actMan.add(U"HeadDropMuteki",
			{
				.update = [&](double t) {
					return t < 0.3;
				}
			}
		);

		actMan.add(U"HeadDropLanding", {
			.start = [&]() {
				character.addMotion(U"HeadDropLanding");
			},
			.update = [&](double t) {
				return character.hasMotion(U"HeadDropLanding");
			},
			.end = [&]() {
				character.removeMotion(U"HeadDropLanding");
				speed = 400;
			}
		});

		actMan.add(U"Damage",{
			.start = [&]() {
				vel = force;
				actMan.start(U"Muteki");
				character.addMotion(U"Knockback");
			},
			.update = [&](double t) {
				vel.x = force.x;

					if (0 <= force.x) {
						if (hitBox.touch(Direction::right)) {
							return false;
						}
					}
					else {
						if (hitBox.touch(Direction::left)) {
							return false;
						}
					}

					if (0 < force.y) {
						if (hitBox.touch(Direction::down)) {
							return false;
						}
					}
					else {
						if (hitBox.touch(Direction::up)) {
							return false;
						}
					}
				return vel.y<0;
			},
			.end = [&]() {
				character.removeMotion(U"Knockback");
				//character.removeMotion(U"HeadDropLanding");
			}
		});

		actMan.add(U"Muteki", {
			.start = [&]() {
				character.addMotion(U"Muteki");
			},
			.update = [&](double t) {
				return character.hasMotion(U"Muteki");
			},
			.end = [&]() {
				character.removeMotion(U"Muteki");
			}
		});






	}

	bool canSummer = true;

	bool punch = false;

	bool walk=false;

	void update()override {

		manager->stage->hit(&hitBox);

		actMan.update();

		if (actMan.hasActive(U"Sliding", U"Rush")) {

			if (left) {
				if (not hitBox.touch(Direction::left))vel.x = -speed;
			}
			else {
				if (not hitBox.touch(Direction::right))vel.x = speed;
			}
		}
		else if (actMan.hasActive(U"Damage")) {
			//何もできない
		}
		else if (actMan.hasActive(U"Summer", U"Mirror{}"_fmt(character.mirrorCount - 1))) {
			if (data->leftKey.pressed()) {
				if (not hitBox.touch(Direction::left))vel.x = -speed;
			}//左
			else if (data->rightKey.pressed()) {
				if (not hitBox.touch(Direction::right))vel.x = speed;
			}//右
		}
		else{
			if (data->leftKey.pressed()) {
				if (not hitBox.touch(Direction::left))vel.x = -speed;
				left = true;
			}//左
			else if (data->rightKey.pressed()) {
				if (not hitBox.touch(Direction::right))vel.x = speed;
				left = false;
			}//右
		}

		if (hitBox.touch(Direction::down)) {
			canSummer = true;
		}

		hitBox.physicsUpdate();
		hitBox.update();

		for (auto& entity : manager->getArray(U"Item")) {
			if (entity->hitBox.intersects(hitBox)) {
				itemCount++;
				entity->damage(1);
				AudioAsset{ U"食べる" }.playOneShot();
			}
		}

		ClearPrint();
		actMan.debugPrint();
		Print << hitBox.touch(Direction::down);

		character.update(pos, left);
		character.character.touchGround(hitBox.Get_Box().boundingRect().bottomY());

		Print << hitBox.touch(Direction::down);
		Print << hitBox.touch(Direction::up);
		Print << hitBox.touch(Direction::left);
		Print << hitBox.touch(Direction::right);

		if ((hitBox.touch(Direction::down) && hitBox.touch(Direction::up)) or (hitBox.touch(Direction::left) && hitBox.touch(Direction::right))) {
			damage(1);
		}

		tmp = character.character.table.at(U"Hitbox").joint.getQuad();
	}

	Quad tmp;

	void draw()const override {

		Circle{ pos,70 }.draw(Palette::Orange);

		tmp.draw(Palette::Red);
		hitBox.draw(ColorF{Palette::Blue,0.5});
		character.draw();


	}

	void damage(int32 n, const Vec2& force = {})override {

		if (not actMan.hasActive(U"Rush",U"Muteki",U"HeadDrop",U"HeadDropMuteki",U"Summer")) {

			this->force = force;

			actMan.start(U"Damage");

			hp -= n;

			AudioAsset{ U"アンナダメージ" }.playOneShot();
		}
	}

	bool isActive()override {
		return true;
	}

	void stopRush() {
		rushMode = false;
	}
};

