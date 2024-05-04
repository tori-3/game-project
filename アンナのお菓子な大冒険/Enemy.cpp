#include"Enemy.h"

double linerMove(double pos,double target,double speed,double dt=Scene::DeltaTime()) {

	double d = speed * dt;

	if (pos<target) {

		if (target<pos+d) {
			return target;
		}
		else {
			return pos + d;
		}
	}
	else {

		if (pos - d<target) {
			return target;
		}
		else {
			return pos - d;
		}
	}
}





void Captain::update() {


//目をプレイヤーに向ける
	character.character.table[U"me"].joint.pos = (manager->get(U"Player")->pos - pos).setLength(15);

	if (character.hasMotion(U"Muteki")) {
		character.character.table[U"me"].joint.pos = Vec2{ 0,-30 };
	}

	//if (hitBox.touch(Direction::right))
	//{
	//	left = true;
	//}
	//else if (hitBox.touch(Direction::left)) {
	//	left = false;
	//}

	if (timer <= 0) {

		type = Random(0, 3);

		if (type == 3) {
			if (5<summonList.size()) {
				type = Random(0, 2);
			}
		}

		if (type == 2) {

			double playerX = manager->get(U"Player")->pos.x;

			if (playerX <=l.x and pos.x<=l.x) {
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
				Print << left;



				//pos.x += (d.x / 3.0) * Scene::DeltaTime();
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
					manager->add(new Kompeito{ pos+Vec2{Random(-30,30),50},Random(-50.0,50.0),100});

					accumulatedTime -= eventInterval;
				}				

			};

			f2 = [=] {
				character.removeMotion(U"Gaaa");
				character.addMotion(U"Tojiru");

				//口を閉じる
				timer = 2;
				f = []()->void {};
				f2 = [] ()->void {};

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

					const Vec2 bornPos = pos + Vec2{ Random(-30,30),50};

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

	//hitBox.physicsUpdate();
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




