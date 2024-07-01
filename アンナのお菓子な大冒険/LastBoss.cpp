#include"LastBoss.h"


void LastBoss::update() {

	manager->stage->hit(&hitBox);


	if(not floatFlg)hitBox.physicsUpdate();
	hitBox.update();

	if (timer <= 0) {

		switch (type)
		{
		case State::kick: {

			timer = 1.0;
			character.addMotion(U"kick1");
			updateFunc = [&]() {
				changeDirection();

				if ((not kickFlg) and timer < 1.0 - 0.4) {
					kickFlg = true;
					vel.x = left ? -600 : 600;
					left = (manager->get(U"Player")->pos.x < pos.x);
				}

				if (timer < 0.4) {

					attack(U"Player", RectF{ Arg::center(pos + (left ? Vec2{-65,-10} : Vec2{65,-10})),65,20 }, 1);
				}
			};

			endFunc = [&]() {
				type = State::stand;
				kickFlg = false;
			};

		}break;
		case State::stand: {

			timer = 0.5;
			character.addMotion(U"Stand");

			updateFunc = [=]() {
				changeDirection();
			};

			endFunc = [&]() {
				type = RandomBool() ? State::kick : State::attack1;

				switch (Random(2,3)) {
				case 0:type = State::kick; break;
				case 1:type = State::attack1; break;
				case 2:type = State::attack2; break;
				case 3:type = State::attack3; break;
				case 4:type = State::masterSparkPreJump; break;
				}
			};

		}break;
		case State::attack1: {
			timer = 3.0;

			character.addMotion(U"Skill4");


			Array<ClosedUmbrella*>umbrellas;

			for (int32 i = 0; i < 1; ++i) {
				ClosedUmbrella* umbrella = new ClosedUmbrella{ pos + Vec2{(i - 0) * 30,0},0_deg,300 };
				manager->add(umbrella);
				umbrellas << umbrella;
			}

			umbrellaFlg = false;

			updateFunc = [&, umbs = umbrellas]() {
				changeDirection();

				if (timer < 3.0 - 0.8) {
					if (not umbrellaFlg) {

						for (auto& umb : umbs) {
							umb->speed = 800;
							umb->effectFlg = true;
							umbrellaFlg = true;
						}
					}
				}
				else if (timer < 3.0 - 0.4) {
					for (auto& umb : umbs) {
						umb->speed = 0;
						umb->angle = linerMove(umb->angle, (manager->get(U"Player")->pos - umb->pos).getAngle(), 360_deg);
					}
				}


			};

			endFunc = [&]() {
				type = State::stand;


			};

		}break;
		case State::attack2: {
			timer = 3.0;

			character.addMotion(U"UdeAgeru");

			for (int32 i = 0; i < 3; ++i) {

				const Vec2 enemyPos = Vec2(Random(rect_size * 2.0, DataManager::get().stageSize.x - rect_size * 2.0), -Random(50,150));

				StrawberrySoldier* soldier = new StrawberrySoldier{ enemyPos };
				manager->add(soldier);
				EnemyUmbrella* umbrella = new EnemyUmbrella{ enemyPos,soldier };
				manager->add(umbrella);
			}

			updateFunc = [&]() {
				changeDirection();
			};

			endFunc = [&]() {
				type = State::stand;


			};

		}break;
		case State::attack3: {
			timer = 10.0;

			character.addMotion(U"ごめんあそばせ");

			for (int32 i = 0; i < 10; ++i) {
				const SizeF stageSize = DataManager::get().stageSize;

				const Vec2 enemyPos = Vec2(stageSize.x + 400 * i, Random(rect_size * 5.0, stageSize.y - rect_size * 2.0));
				ClosedUmbrella* umb = new ClosedUmbrella{ enemyPos,-90_deg,400 };
				umb->effectFlg = true;
				manager->add(umb);

				if (RandomBool(0.3)) {
					umb = new ClosedUmbrella{ enemyPos + Vec2{100,Random(-50,50)},-90_deg,400 };
					umb->effectFlg = true;
					manager->add(umb);
				}

			}

			updateFunc = [&]() {
				changeDirection();

			};

			endFunc = [&]() {
				type = State::stand;


			};

		}break;

		case State::masterSparkPreJump:{
			character.addMotion(U"Jump1");

			timer = 1.0;
			updateFunc = [=]() {
				changeDirection();

				if (timer < 0.5) {
					this->accumulatedTime += Scene::DeltaTime();
					if (eventInterval <= this->accumulatedTime)
					{
						DataManager::get().additiveEffect.add<MagicEffect>(pos, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() }, Random(360_deg),500);
						this->accumulatedTime -= eventInterval;
					}
				}
			};
			endFunc = [=]() {
				type = State::masterSparkJump;
			};
			
		}break;
		case State::masterSparkJump: {
			//マスタースパーク前のジャンプ
			character.addMotion(U"Fly");

			constexpr double timeLim = 2.0;
			timer = timeLim;

			floatFlg = true;

			accumulatedTime = 0;

			const double stageWidth = DataManager::get().stageSize.x;

			bool playerLeft = (manager->get(U"Player")->pos.x) < (stageWidth/ 2.0);

			double targetX = playerLeft ? (stageWidth - rect_size * 2) : rect_size * 2;
			double lenX = Abs(pos.x - targetX);
			double targetY = rect_size * 3;
			double lenY = Abs(pos.y - targetY);
			updateFunc = [=]() {
				changeDirection();

				pos.x = linerMove(pos.x, targetX, lenX / timeLim);
				pos.y = linerMove(pos.y, targetY, lenY / timeLim);

				this->accumulatedTime += Scene::DeltaTime();
				if (eventInterval <= this->accumulatedTime)
				{
					const Vec2 rFootPos = Figure{ character.character.table[U"rfootTip"].joint.getQuad() }.center();
					const Vec2 lFootPos = Figure{ character.character.table[U"lfootTip"].joint.getQuad() }.center();
					DataManager::get().additiveEffect.add<MagicEffect>(rFootPos, TextureAsset{U"MagicEffect{}"_fmt(Random(0,3))}, HSV{360 * 2 * Scene::Time()});
					DataManager::get().additiveEffect.add<MagicEffect>(lFootPos, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() });
					this->accumulatedTime -= eventInterval;
				}
			};

			endFunc = [&]() {
				type = State::masterSparkWait;
			};


		}break;
		case State::masterSparkWait: {

			character.addMotion(U"MasupaPause");

			timer = 3.0;
			magicCircle.start(100);

			updateFunc = [=]() {

				if (1.5<timer) {
					changeDirection();

					const Vec2 umbPos = Figure{ character.character.table[U"saki"].joint.getQuad() }.center();

					magicCircle.rad = (manager->get(U"Player")->pos - pos).getAngle() - 90_deg;					
					magicCircle.setPos(OffsetCircular{ umbPos,200,magicCircle.rad-90_deg });
				}

				character.character.table[U"rarm"].joint.angle = left ? 180_deg-magicCircle.rad - 120_deg : magicCircle.rad - 120_deg;
			};

			endFunc = [&]() {
				type = State::masterSpark;
			};

		}break;
		case State::masterSpark: {

			character.addMotion(U"MasupaShot");

			timer = 5.0;

			//魔法陣の位置おかしい 原因不明
			const Vec2 umbPos = Figure{ character.character.table[U"saki"].joint.getQuad() }.center();

			DataManager::get().effect.add<LaserEffect>(RectF(Arg::leftCenter = umbPos, 1500, 150), magicCircle.rad, Palette::Pink);

			updateFunc = [=]() {
				//向きは変えない

				attack(U"Player", RectF(Arg::leftCenter = pos, 1500, 100).rotatedAt(pos, magicCircle.rad), 1.0);
				character.character.table[U"rarm"].joint.angle = left ? 180_deg -magicCircle.rad - 120_deg : magicCircle.rad - 120_deg;
			};

			endFunc = [&]() {
				magicCircle.end();
				type = State::stand;
				floatFlg = false;

			};

		}break;

		default:
			break;
		}






	}

	if (timer > 0)
	{
		timer -= Scene::DeltaTime();
		updateFunc();
		if (timer <= 0)
		{
			endFunc();
		}
	}

	magicCircle.update();

	character.update(pos, left);
}
