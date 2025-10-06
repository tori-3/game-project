#include "Player.h"
#include "TalkManager.h"
#include "ControllerManager.h"

Player::Player(const Vec2& cpos) :
	character{ U"Characters/annna/annna.json",U"Characters/annna/motion.txt",0.25,cpos,false },
	Entity{ U"Player",defaultBody ,cpos,{0,0},DataManager::get().maxHP }
{
	hitBox.physics.isPlayer = true;

	//最前面に持ってくるため
	z = 100;

	actMan.add(U"Walk", {
		.startCondition = [&]() {
			return hitBox.touch(Direction::down) and (not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Landing",U"Shagamu",U"Sliding",U"Punch",U"Summer",U"HeadDropLanding",U"HeadDrop",U"Damage",U"Dead",U"Clear",U"Stop")) and (leftKey.pressed() || rightKey.pressed()||ControllerManager::get().RightPressed()|| ControllerManager::get().LeftPressed());
		},
		.start = [&]() {
			AudioAsset{U"足音"}.play();
			character.addMotion(U"Walk",true);
		},
		.update = [&](double t) {
			return not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Landing",U"Shagamu",U"Sliding",U"Punch",U"HeadDrop") and (leftKey.pressed() || rightKey.pressed() || ControllerManager::get().RightPressed() || ControllerManager::get().LeftPressed());
		},
		.end = [&]() {
			AudioAsset{U"足音"}.stop(0.5s);
			character.removeMotion(U"Walk");
		}
	});

	actMan.add(U"PreJump", {
		.startCondition = [&]() {
			return hitBox.touch(Direction::down) and jumpKey.down() and not actMan.hasActive(U"Sliding",U"Summer",U"Damage",U"Jump",U"Punch",U"Dead",U"Clear");
		},
		.start = [&]() {
			AudioAsset{U"ジャンプ"}.playOneShot();
			if (not actMan.hasActive(U"Rush"))character.addMotion(U"PreJump");
		},
		.update = [&](double t) {
			return t < (1 / 30.0);
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

	actMan.add(U"Tame", {
		.startCondition = [&]() {
			return (0.2s < attackKey.pressedDuration()) && 10 <= itemCount;
		},
		.start = [&]() {

		},
		.update = [&](double t) {

			DataManager::get().tame = Min(t / 0.8, 1.0);

			if (0.8 <= t && (not attackKey.pressed())) {
				if (not actMan.hasActive(U"Sliding", U"Damage", U"Dead",U"Clear") and hitBox.touch(Direction::down)) {
					actMan.start(U"Rush");
				}
				return false;
			}
			else {
				return attackKey.pressed();
			}
		},
		.end = [&]() {
			DataManager::get().tame = 0;
		}
	});

	actMan.add(U"Rush", {
		.start = [&]() {
			itemCount = 0;
			hitBox.physics.rush = true;
			speed = 1000;
			character.addMotion(U"Tosshin",true);
			AudioAsset{ U"突進足音" }.play();
			AudioAsset{ U"風" }.play();

			//前のフレームのtouchが残っているためここで当たり判定を取っておく
			manager->stage->hit(&hitBox);
		},
		.update = [&](double t) {

			speed = 1000;

			if (hitBox.touch(Direction::down))
			{
				AudioAsset{ U"突進足音" }.play();
			}
			else
			{
				AudioAsset{ U"突進足音" }.stop(0.1s);
			}

			if (attackDamaged(U"Enemy",  character.character.table.at(U"Hitbox").joint.getQuad(), 5, 1500))
			{
				AudioAsset{ U"突進衝突" }.playOneShot();
			}

			if (hitBox.touch(Direction::left))
			{

				//すり抜け防止
				manager->stage->hit(&hitBox);

				force = Vec2{ 300,-400 };
				actMan.start(U"Damage");

				return false;
			}
			else if (hitBox.touch(Direction::right))
			{
				//すり抜け防止
				manager->stage->hit(&hitBox);

				force = Vec2{ -300,-400 };
				actMan.start(U"Damage");
				return false;
			}

			return true;
		},
		.end = [&]() {
			character.removeMotion(U"Tosshin");
			actMan.start(U"HeadDropMuteki");
			AudioAsset{ U"突進足音" }.stop(0.1s);
			AudioAsset{ U"風" }.stop(0.1s);
			AudioAsset{ U"突進衝突" }.playOneShot();
			ControllerManager::get().setVibration(0.7);

			speed = 400;
			hitBox.physics.rush = false;
		}
	});

	actMan.add(U"Falling", {
		.startCondition = [&]() {
			return 0 < vel.y and not hitBox.touch(Direction::down) and not actMan.hasActive(U"Summer",U"HeadDrop",U"Damage",U"Rush",U"Dead",U"Clear");
		},
		.start = [&]() {
			character.addMotion(U"Falling");
		},
		.update = [&](double t) {
			return 0 < vel.y and not hitBox.touch(Direction::down) and not actMan.hasActive(U"HeadDrop");
		},
		.end = [&]() {
			character.removeMotion(U"Falling");
			if (not (downKey.pressed()|| ControllerManager::get().DownPressed()) && not actMan.hasActive(U"Dead",U"Clear"))
			{
				actMan.start(U"Landing");
			}
		}
	});

	actMan.add(U"Landing", {
		.start = [&]() {
		character.addMotion(U"Standing");
		AudioAsset{ U"着地" }.playOneShot();
	},
	.update = [&](double t) {
		speed = 250;

		return not jumpKey.down() and not (downKey.pressed()|| ControllerManager::get().DownPressed()) and not actMan.hasActive(U"Rush") and t < 0.1;
	},
	.end = [&]() {
		speed = 400;
		character.removeMotion(U"Standing");
	}
	});

	actMan.add(U"Standing", {
		.startCondition = [&]() {
			return not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Walk",U"Landing",U"Shagamu",U"Sliding",U"Punch",U"Summer",U"HeadDropLanding",U"Damage",U"Dead",U"Clear") and hitBox.touch(Direction::down);
		},
		.start = [&]() {
			character.addMotion(U"Standing");
		},
		.update = [&](double t) {
			return not actMan.hasActive(U"Jump",U"PreJump",U"Rush",U"Falling",U"Walk",U"Landing",U"Shagamu",U"Punch",U"HeadDropLanding",U"Damage");
		},
		.end = [&]() {
			character.removeMotion(U"Standing");
		}
	});

	actMan.add(U"Shagamu", {
		.startCondition = [&]() {
			return (downKey.pressed()|| ControllerManager::get().DownPressed()) and (not jumpKey.pressed()) and hitBox.touch(Direction::down) and not actMan.hasActive(U"Sliding",U"Summer",U"Damage",U"Landing",U"HeadDropLanding",U"HeadDrop",U"Punch",U"Jump",U"PreJump",U"Falling",U"Dead",U"Clear",U"Rush",U"Stop");
		},
		.start = [&]() {
			character.addMotion(U"Shagamu");
			
			hitBox.setFigure(RectF{ Arg::center= Vec2(0,defaultBody.h/4.0),defaultBody.w,defaultBody.h / 2.0 });
		},
		.update = [&](double t) {
			speed = 0;
			return (downKey.pressed()|| ControllerManager::get().DownPressed()) and not jumpKey.down() and not attackKey.pressed() && not actMan.hasActive(U"Falling");
		},
		.end = [&]() {
			character.removeMotion(U"Shagamu");
			speed = 400;


			hitBox.setFigure(defaultBody);

			if (attackKey.pressed() and hitBox.touch(Direction::down) and not actMan.hasActive(U"PreJump",U"Punch")) {
				actMan.start(U"Sliding");
			}
		}
	});

	actMan.add(U"Sliding", {
		.start = [&]() {
			character.addMotion(U"Sliding");
			hitBox.setFigure(RectF{ Arg::center(0,defaultBody.h / 4),defaultBody.w,defaultBody.h / 2 });
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

			if (not actMan.conditonStart(U"Shagamu")) {
				hitBox.setFigure(defaultBody);
			}

		}
	});

	actMan.add(U"Punch", {
		.startCondition = [&]() {
			return attackKey.down() and actMan.hasActive(U"Standing",U"Walk") and (not actMan.hasActive(U"Summer",U"PreJump",U"Jump",U"Shagamu",U"Dead",U"Clear")) and hitBox.touch(Direction::down);
		},
		.start = [&]() {
			character.addMotion(U"Punch");
			punch = false;
			speed = 70;
			AudioAsset{ U"パンチ" }.playOneShot();
		},
		.update = [&](double t) {

			if (not punch) {
				if (0.07 < t) {
					//発射
					punch = true;
					manager->add(new Hadouken{ pos,left ? -90_deg : 90_deg });
				}
				return true;
			}
			return character.hasMotion(U"Punch") and not jumpKey.down();
		},
		.end = [&]() {
			character.removeMotion(U"Punch");
			speed = 400;
		}
	});

	actMan.add(U"Summer", {
		.startCondition = [&]() {
			return attackKey.down() and not hitBox.touch(Direction::down) and not actMan.hasActive(U"Punch",U"Shagamu",U"Sliding",U"Damage",U"Rush",U"HeadDrop",U"Dead",U"Clear") and canSummer;
		},
		.start = [&]() {
			AudioAsset{U"サマーソルト"}.playOneShot();
			character.addMotion(U"Summer");
			summerHited = false;
			vel.y = -500.0;
		},
		.update = [&](double t) {
			if (attack(U"Enemy",Circle{ pos,70 },1,200) and (not summerHited)) {
				AudioAsset{ U"サマーソルトヒット" }.playOneShot();
				summerHited = true;
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
			return (downKey.down()|| ControllerManager::get().DownDown()) and not actMan.hasActive(U"Summer",U"Damage",U"Dead",U"Clear",U"Rush") and not hitBox.touch(Direction::down);
		},
		.start = [&]() {
			character.addMotion(U"HeadDrop");
			speed = 0;
		},
		.update = [&](double t) {

			for (auto enemy : attack(U"Enemy", hitBox.Get_Box(), 2, 200)) {
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
			if (hitBox.touch(Direction::down))actMan.start(U"HeadDropLanding");
			character.removeMotion(U"HeadDrop");

			ControllerManager::get().setVibration(0.15);

			AudioAsset{ U"ヘッドドロップ" }.playOneShot();
		}
	});

	//一瞬無敵　HeadDrop以外にも使う
	actMan.add(U"HeadDropMuteki", {
			.update = [&](double t) {
				return t < 0.3;
			}
	});

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

	actMan.add(U"Damage", {
		.start = [&]() {
			vel = force;
			actMan.start(U"Muteki");
			character.addMotion(U"Knockback");
		},
		.update = [&](double t) {

			vel.x = force.x;

			if (0 <= force.x)
			{
				if (hitBox.touch(Direction::right))
				{
					return false;
				}
			}
			else
			{
				if (hitBox.touch(Direction::left))
				{
					return false;
				}
			}

			if (0 < force.y)
			{
				if (hitBox.touch(Direction::down))
				{
					return false;
				}
			}
			else
			{
				if (hitBox.touch(Direction::up))
				{
					return false;
				}
			}
			return vel.y < 0;
		},
		.end = [&]() {
			character.removeMotion(U"Knockback");
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

	actMan.add(U"Dead", {
		.startCondition = [&]() {
			return hp <= 0;
		},
		.start = [&]() {
			BGMManager::get().play(U"ゲームオーバー");

			actMan.cancelAll({U"Dead"});
			DataManager::get().table.emplace(U"IrisOut");
			character.clearMotion();
			character.addMotion(U"Cry");
		},
		.update = [&](double t) {
			return t < 4.0;
		},
		.end = [&]() {
			DataManager::get().playerAlive = false;
		}
	});

	actMan.add(U"Clear", {
		.startCondition = [&]() {
			return DataManager::get().table.contains(U"Clear");
		},
		.start = [&]() {
			actMan.cancelAll({U"Clear"});
			DataManager::get().table.emplace(U"IrisOut");

			character.clearMotion();

			character.addMotion(U"Yorokobu");

			BGMManager::get().play(U"ステージクリア");

		},
		.update = [&](double t) {
			return t < 4.0;
		},
		.end = [&]() {
			DataManager::get().playerAlive = false;
		}
	});

	//落下後のワープで一瞬止まる
	actMan.add(U"Stop", {
		.update = [&](double t) {
			return t <= 0.7;
		},
	});
}

void Player::update()
{
	manager->stage->hit(&hitBox);

	actMan.update();

	if(not actMan.hasActive(U"HeadDrop"))
	{
		vel.y = Min(vel.y, 800.0);
	}

	if (actMan.hasActive(U"Sliding", U"Rush"))
	{
		if (left)
		{
			if (not hitBox.touch(Direction::left))vel.x = -speed;
		}
		else
		{
			if (not hitBox.touch(Direction::right))vel.x = speed;
		}
	}
	else if (actMan.hasActive(U"Damage", U"Dead", U"Clear", U"HeadDrop",U"Stop"))
	{
		//何もできない
	}
	else if (actMan.hasActive(U"Summer", U"HeadDropLanding", U"Mirror{}"_fmt(character.mirrorCount - 1))) {
		if (leftKey.pressed() || ControllerManager::get().LeftPressed())
		{
			if (not hitBox.touch(Direction::left))vel.x = -speed;
		}//左
		else if (rightKey.pressed() || ControllerManager::get().RightPressed())
		{
			if (not hitBox.touch(Direction::right))vel.x = speed;
		}//右
	}
	else {
		if (leftKey.pressed() || ControllerManager::get().LeftPressed())
		{
			if (not hitBox.touch(Direction::left))vel.x = -speed;
			left = true;
		}//左
		else if (rightKey.pressed() || ControllerManager::get().RightPressed())
		{
			if (not hitBox.touch(Direction::right))vel.x = speed;
			left = false;
		}//右
	}

	if (hitBox.touch(Direction::down))
	{
		canSummer = true;
	}

	hitBox.physicsUpdate();
	hitBox.update();

	for (auto& entity : manager->getArray(U"Item"))
	{
		if (entity->hitBox.intersects(hitBox))
		{
			itemCount++;
			entity->damage(1);
			AudioAsset{ U"食べる" }.playOneShot();
		}
	}

	character.update(pos, left);
	character.character.touchGround(hitBox.Get_Box().boundingRect().bottomY());

	if (hitBox.canRespawnOn() && hitBox.rightFloor() && hitBox.leftFloor())
	{
		lastTouchPos = pos;
	}

	const bool horizontalCrushed = hitBox.touch(Direction::left) and hitBox.touch(Direction::right);
	const bool verticalCrushed = hitBox.touch(Direction::down) and hitBox.touch(Direction::up);

	if (horizontalCrushed or verticalCrushed)
	{
		crushedTimer += Scene::DeltaTime();
	}
	else
	{
		crushedTimer = 0;
	}

	const bool isDrop = DataManager::get().stageSize.y + 200 < pos.y;
	const bool isCrushed = 0.2 < crushedTimer;

	//落下した or 潰された
	if (isDrop or isCrushed)
	{
		vel = Vec2{};
		force = Vec2{};
		pos = lastTouchPos;
		actMan.cancelAll();
		//脱法ダメージ
		actMan.start(U"Damage");
		DataManager::get().table.insert(U"Damage");

		hp -= 1;

		crushedTimer = 0;

		actMan.start(U"Stop");

		//当たり判定を同期させるため？
		//これがないとなんかバグる
		hitBox.update();
	}
}

void Player::draw()const
{
	character.draw();
}

void Player::damage(int32 n, const Vec2& force, DamageType damageType)
{
	if (0 < hp)
	{
		if (not actMan.hasActive(U"Rush", U"Muteki", U"Summer", U"Dead", U"Clear"))
		{
			if (not actMan.hasActive(U"HeadDrop", U"HeadDropMuteki") or damageType == DamageType::UnBrakable)
			{

				this->force = force;

				actMan.start(U"Damage");
				DataManager::get().table.insert(U"Damage");

				hp -= n;

				ControllerManager::get().setVibration(0.3);

				AudioAsset{ U"アンナダメージ" }.playOneShot();
			}
		}
	}
}

