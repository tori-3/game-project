#include"Boss.h"
#include"Enemy.h"

SnowKnight::SnowKnight(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(-20,40),70 * 1,70 * 4 - 30 },cpos,{0,0},1 }
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

					attack(U"Player", ken, 1);

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

void SnowKnight::damage(int32 n, const Vec2& _force) {

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
