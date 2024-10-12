#include"Boss.h"
#include"Enemy.h"
#include"LinerMove.h"
#include"TalkManager.h"
#include"FadeUpEffect.h"
#include"Player.h"

SnowKnight::SnowKnight(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(-20,40),70 * 1,70 * 4 - 30 },cpos,{0,0},maxHp }
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


void SnowKnight::update() {

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

			left = manager->get(U"Player")->pos.x < pos.x;

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

					attack(U"Player", ken, 1,DamageType::UnBrakable);

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
				else if (3.5 < t and t <= 3.6) {
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
					Vec2 bornPos{ centerX + Random(-400,400),200 };



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

void SnowKnight::lateUpdate() {

	DataManager::get().bossHPRate = hp / double(maxHp);

	summonList.remove_if([](Entity* entity) {return not entity->isActive(); });

	if (not isActive()) {
		DataManager::get().table.emplace(U"Clear");
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200);
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos + Vec2{ 50,50 }, 100);
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos - Vec2{ 50,50 }, 100);
	}
}

void SnowKnight::damage(int32 n, const Vec2& _force, DamageType damageType) {

	if (not character.hasMotion(U"Muteki") and not character.hasMotion(U"YoroiMuteki")) {

		if (yoroi) {
			//5は突進のダメージ
			if (5 <= n) {
				yoroi -= 1;
			}
			else {

				String text;

				if (attackCount < 3)
				{
					text = U"むてき！";
				}
				else if (attackCount < 4)
				{
					text = U"むてきだってば！";
				}
				else if (attackCount < 5)
				{
					text = U"よろいさいきょう！";
				}
				else if (attackCount < 6)
				{
					text = U"きかないね！";
				}
				else if (attackCount < 7)
				{
					text = U"とっしんしてみな！";
				}
				else
				{
					if (IsOdd(attackCount))
					{
						if (10 <= dynamic_cast<Player*>(manager->get(U"Player"))->itemCount)
						{
							text = U"Enter長押し！";
						}
						else
						{
							text = U"クッキーを取ろう！";
						}
					}
					else
					{
						text = U"むてき！";
					}
				}


				DataManager::get().effect.add<FadeUpEffect>(pos,text, FontAsset(U"TitleFont"),Palette::Blue);

				++attackCount;
			}




			if (yoroi <= 0) {
				character.addMotion(U"Nugeru");
			}
			character.addMotion(U"YoroiMuteki");
			force = _force;
			vel.y = force.y;
			vel.x = force.x * 1.5;

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
	else if (character.hasMotion(U"YoroiMuteki") and 5 <= n) {
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

void SnowKnight::draw()const {
	character.draw();
}


SlaversCookie::SlaversCookie(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,0),50,100},cpos,{0,0},maxHp }
, character{ U"Characters/cookieDoreisho/model.json" ,U"Characters/cookieDoreisho/motion.txt" ,0.4,cpos,true,false }
{
	f = []() {};
	f2 = []() {};
	timer = 3;

	rx = pos.x + rect_size * 6.5;
	lx = pos.x - rect_size * 6.5;

}

void SlaversCookie::update() {

	manager->stage->hit(&hitBox);

	if (rx <= pos.x)
	{
		left = true;
	}
	else if (pos.x <= lx) {
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

			if (summonListItigo.size() < 10) {
				for (auto i : step(2)) {
					Vec2 bornPos{};

					if (i == 0) {
						bornPos = { rect_size * 1.5,pos.y + 200 };
					}
					else {
						bornPos = { DataManager::get().stageSize.x - rect_size * 1.5,pos.y + 200 };
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

				Vec2 bornPos{ pos.x + Random(-100,100),pos.y + 200 };
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

void SlaversCookie::lateUpdate() {

	DataManager::get().bossHPRate = hp / double(maxHp);

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

void SlaversCookie::draw()const {
	character.draw();
}




Captain::Captain(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,-30),230,100},cpos,{0,0},maxHp }
, character{ U"Characters/sentyo/model.json" ,U"Characters/sentyo/motion.txt" ,1,cpos,true,false }
{
	r = { pos.x + 400,pos.y };
	l = { pos.x - 400,pos.y };
	center = pos.x;

	f = []() {};
	f2 = []() {};
	timer = 3;
	character.addMotion(U"Mokumoku", true);
}


void Captain::update() {


	//目をプレイヤーに向ける
	character.character.table[U"me"].joint.pos = (manager->get(U"Player")->pos - pos).setLength(15);

	if (character.hasMotion(U"Muteki")) {
		character.character.table[U"me"].joint.pos = Vec2{ 0,-30 };
	}

	if (timer <= 0) {

		type = Random(0, 3);

		if (type == 3) {
			if (5 < summonList.size()) {
				type = Random(0, 2);
			}
		}

		if (type == 2) {

			double playerX = manager->get(U"Player")->pos.x;

			if (playerX <= l.x and pos.x <= l.x) {
				type = Random(0, 1);
			}
			else if (r.x <= playerX and r.x <= pos.x) {
				type = Random(0, 1);
			}
		}

		switch (type)
		{
		case 0: {
			double timeLim = 2.5;
			timer = timeLim;
			Vec2 d;

			bool targetIsRight = pos.x < center;

			if (Abs(center - pos.x) < 200) {
				targetIsRight = RandomBool();
			}

			if (targetIsRight)
			{
				d = r - pos;
				left = false;
			}
			else
			{
				d = l - pos;
				left = true;
			}

			if (left) {
				character.addMotion(U"YureruLeft");
			}
			else {
				character.addMotion(U"YureruRight");
			}

			double y = pos.y, x = pos.x;

			Duration time = 1.5s;
			double A = 30;
			if (RandomBool()) {
				time = timeLim * 2s;
				A = 500 + l.y - y;
			}

			f = [=]
			{
				pos.x = Periodic::Sine1_1(timeLim * 4, timeLim - timer) * d.x + x;
				pos.y = (d.y / timeLim) * (timeLim - timer) + y + Periodic::Sine1_1(time, timer) * A;

				if (timer < 0.2) {
					character.addMotion(U"YureruCenter");
				}
			};
			f2 = [=]
			{
				vel.x = 0;
				left != left;
			};
		}
			  break;
		case 1: {

			//プレイヤーを追従
			timer = 3;

			f = [&] {

				pos.x = linerMove(pos.x, manager->get(U"Player")->pos.x, 500);

			};

			f2 = [&] {

				//口を開けてゆっくり追いかける
				timer = 1.0;
				character.addMotion(U"Gaaa");

				f = [&] {
					pos.x = linerMove(pos.x, manager->get(U"Player")->pos.x, 500 * timer / 1.0);
				};

				f2 = [&] {

					//落下
					double timeLim = 0.2;
					timer = timeLim;

					double dy = (r.y + 450) - pos.y;
					double y = pos.y;

					f = [=] {

						pos.y = y + dy * (timeLim - timer) / timeLim;
					};

					f2 = [&] {
						DataManager::get().table.emplace(U"ShakeCamera");

						//待つ
						timer = 2.0;

						character.addMotion(U"Tojiru");

						f = [&] {

						};

						f2 = [&] {
							//上る
							timer = 2.0;

							f = [&] {
								pos.y = Math::SmoothDamp(pos.y, r.y, vel.y, 0.5);
							};

							f2 = [&] {


							};
						};
					};

				};
			};






		}break;
		case 2: {

			//金平糖を吐き出す

			constexpr double speed = 200;

			accumulatedTime = 0.0;
			character.addMotion(U"Gaaa");

			double targetX = 0;

			if (manager->get(U"Player")->pos.x < pos.x) {
				targetX = l.x;
			}
			else {
				targetX = r.x;
			}

			timer = Abs(targetX - pos.x) / speed;

			f = [=] {

				pos.x = linerMove(pos.x, targetX, speed);

				accumulatedTime += Scene::DeltaTime();
				constexpr double eventInterval = 0.1;
				if (eventInterval <= accumulatedTime)
				{
					manager->add(new Kompeito{ pos + Vec2{Random(-30,30),50},Random(-50.0,50.0),100 });

					accumulatedTime -= eventInterval;
				}

			};

			f2 = [=] {
				character.removeMotion(U"Gaaa");
				character.addMotion(U"Tojiru");

				//口を閉じる
				timer = 2;
				f = []()->void {};
				f2 = []()->void {};

			};

		}break;
		case 3: {

			//敵を吐き出す

			Vec2 d;

			accumulatedTime = 0.4;
			character.addMotion(U"Gaaa");

			double timeLim = 3;

			timer = timeLim;


			Duration time = 1.5s;
			double y = pos.y, x = pos.x;
			double A = 30;


			bool targetIsRight = pos.x < center;

			if (Abs(center - pos.x) < 200) {
				targetIsRight = RandomBool();
			}

			if (targetIsRight)
			{
				d = r - pos;
				left = false;
			}
			else
			{
				d = l - pos;
				left = true;
			}

			f = [=] {

				pos.x = Periodic::Sine1_1(timeLim * 4, timeLim - timer) * d.x + x;
				pos.y = (d.y / timeLim) * (timeLim - timer) + y + Periodic::Sine1_1(time, timer) * A;

				accumulatedTime += Scene::DeltaTime();
				constexpr double eventInterval = 0.5;
				if (eventInterval <= accumulatedTime)
				{
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
						[&](Vec2 pos) {
							ItigoSlave* ptr = new ItigoSlave{ pos };
							ptr->left = RandomBool();
							ptr->vel.y = -500;
							return ptr;
						},
						[&](Vec2 pos) {
							CloudEnemy* ptr = new CloudEnemy{ pos };
							ptr->vel.y = -500;
							return ptr;
						},
					};

					const Vec2 bornPos = pos + Vec2{ Random(-30,30),50 };

					Entity* entity = options[Random(4)](bornPos);

					summonList << entity;

					manager->add(entity);

					DataManager::get().additiveEffect.add<ExplosionEffect>(bornPos, 60, Palette::Yellowgreen);

					accumulatedTime -= eventInterval;
				}

			};

			f2 = [=] {
				character.addMotion(U"Tojiru");

				//口を閉じる
				timer = 2;
				f = []()->void {};
				f2 = []()->void {};

			};


		}break;
		default:
			break;
		}
	}

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

	attack(U"Player", hitBox.getFigure(), 1);

	character.update(pos, true);

}

void Captain::lateUpdate() {

	DataManager::get().bossHPRate = hp / double(maxHp);

	summonList.remove_if([](Entity* entity) {return not entity->isActive(); });

	if (not isActive()) {
		DataManager::get().table.emplace(U"Clear");
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200);
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos + Vec2{ 50,50 }, 100);
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos - Vec2{ 50,50 }, 100);
	}
}

void Captain::draw()const{
	character.draw();
}

void Captain::damage(int32 n, const Vec2& force, DamageType damageType)
{
	if (not character.hasMotion(U"Muteki")) {
		hp -= n;
		character.addMotion(U"Muteki");
	}
}
