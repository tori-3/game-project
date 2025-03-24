#include"LastBoss.h"


void LastBoss::update() {

	manager->stage->hit(&hitBox);

	if(not floatFlg)hitBox.physicsUpdate();
	hitBox.update();

	if (timer <= 0) {

		switch (type)
		{
		case State::kick: {

			constexpr double timeLim = 1.4;
			timer = timeLim;
			character.addMotion(U"kick1");

			changeDirection();

			updateFunc = [&]() {

				if ((not kickFlg) and timer < timeLim - 0.4)
				{
					kickFlg = true;
					left = (manager->get(U"Player")->pos.x < pos.x);

					if (left) {

						if (pos.x<rect_size * 2)
						{
							left = false;
						}
					}
					else {
						if (DataManager::get().stageSize.x-rect_size*2<pos.x)
						{
							left = true;
						}
					}
				}

				if (kickFlg)
				{
					pos.x += left ? -500 * Scene::DeltaTime() : 500 * Scene::DeltaTime();
				}

				if (timer < timeLim - 0.4)
				{
					attack(U"Player", RectF{ Arg::center(pos + (left ? Vec2{-65,-10} : Vec2{65,-10})),65,40 }, 1);
				}
			};

			endFunc = [&]() {
				type = State::stand;
				kickFlg = false;
				damageFlg = false;
			};

		}break;
		case State::stand: {


			timer = 1.0;
			character.addMotion(U"Stand");

			if (isLastSpart()) {
				switch (Random(0, 4))
				{
				case 0:type = State::enemyFalls; break;
				case 1:type = State::umbrellaShot; break;
				case 2:type = State::reflectionUmbrella; break;
				case 3:type = State::kompeitoGalaxyJump; break;
				case 4:type = State::masterSparkPreJump; break;
				}
			}
			else {
				if (damageFlg)
				{
					type = State::kick;
				}
				else {
					switch (Random(0, 4))
					{
					case 0:type = State::throwUmbrella; break;
					case 1:type = State::enemyFalls; break;
					case 2:type = State::umbrellaShot; break;
					case 3:type = State::reflectionUmbrella; break;
					}
				}
			}

			updateFunc = [=]() {
				changeDirection();
			};

			endFunc = [=]() {};

		}break;
		case State::throwUmbrella: {

			timer = 3.0;

			character.addMotion(U"Skill4");


			ClosedUmbrella* umbrella = nullptr;

			umbrella = new ClosedUmbrella{ pos,0_deg,300 };
			manager->add(umbrella);

			umbrellaFlg = false;

			updateFunc = [=]() {
				changeDirection();

				if (timer < 3.0 - 0.8) {
					if (not umbrellaFlg) {

						umbrella->speed = 800;
						umbrella->effectFlg = true;
						umbrellaFlg = true;
					}
				}
				else if (timer < 3.0 - 0.4) {
					umbrella->speed = 0;
					umbrella->angle = LinerMove(umbrella->angle, (manager->get(U"Player")->pos - umbrella->pos).getAngle(), 360_deg);
				}
			};

			endFunc = [&]() {

				if (throwUmbrellaCount<2) {
					++throwUmbrellaCount;
					type = State::throwUmbrella;
				}
				else {
					throwUmbrellaCount = 0;
					type = State::stand;
				}

			};

		}break;
		case State::enemyFalls: {
			timer = 3.0;

			character.addMotion(U"UdeAgeru");

			AudioAsset{ U"召喚" }.playOneShot();

			for (int32 i = 0; i < (isLastSpart()?5:3) ; ++i) {

				const Vec2 enemyPos = Vec2(Random(rect_size * 2.0, DataManager::get().stageSize.x - rect_size * 2.0), rect_size*2);

				Entity* soldier = nullptr;

				switch (Random(3))
				{
				case 0:soldier = new StrawberrySoldier{ enemyPos };  break;
				case 1:soldier = new CookieSoldier{ enemyPos }; break;
				case 2:soldier = new Snowman{ enemyPos }; break;
				case 3:soldier = new ItigoSlave{ enemyPos }; break;
				default:
					break;
				}

				manager->add(soldier);
				EnemyUmbrella* umbrella = new EnemyUmbrella{ enemyPos,soldier };
				manager->add(umbrella);

				DataManager::get().additiveEffect.add<ExplosionEffect>(enemyPos, 50, HSV{ 20,0.8,1 });

			}

			updateFunc = [&]()
			{
				changeDirection();
			};

			endFunc = [&]()
			{
				type = State::stand;
			};

		}break;
		case State::umbrellaShot: {
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
		case State::reflectionUmbrella: {

			character.addMotion(U"ごめんあそばせ");

			
			double timeLim = isLastSpart()?10.0:7.5;

			timer = timeLim;

			

			for (int32 i = 0; i < 3; ++i) {
				const SizeF stageSize = DataManager::get().stageSize;

				ChaseUmbrella* umb = new ChaseUmbrella{ pos,30_deg*(i-1),Random(300.0,500.0),timeLim };
				manager->add(umb);

			}

			updateFunc = [&]() {
				changeDirection();

			};

			endFunc = [&]() {
				type = State::stand;
			};

		}break;
		case State::kompeitoGalaxyJump: {
			//マスタースパーク前のジャンプ
			character.addMotion(U"Fly");

			constexpr double timeLim = 2.0;
			timer = timeLim;

			floatFlg = true;
			constexpr double eventInterval = 0.05;

			accumulatedTime = 0;

			const double stageWidth = DataManager::get().stageSize.x;

			bool playerLeft = (manager->get(U"Player")->pos.x) < (stageWidth / 2.0);

			double targetX = playerLeft ? (stageWidth - rect_size * 2) : rect_size * 2;
			double lenX = Abs(pos.x - targetX);
			double targetY = rect_size * 3;
			double lenY = Abs(pos.y - targetY);
			updateFunc = [=]() {
				changeDirection();

				pos.x = LinerMove(pos.x, targetX, lenX / timeLim);
				pos.y = LinerMove(pos.y, targetY, lenY / timeLim);

				this->accumulatedTime += Scene::DeltaTime();
				if (eventInterval <= this->accumulatedTime)
				{
					const Vec2 rFootPos = Figure{ character.character.table[U"rfootTip"].joint.getQuad() }.center();
					const Vec2 lFootPos = Figure{ character.character.table[U"lfootTip"].joint.getQuad() }.center();
					DataManager::get().additiveEffect.add<MagicEffect>(rFootPos, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() });
					DataManager::get().additiveEffect.add<MagicEffect>(lFootPos, TextureAsset{ U"MagicEffect{}"_fmt(Random(0,3)) }, HSV{ 360 * 2 * Scene::Time() });
					this->accumulatedTime -= eventInterval;
				}
			};

			endFunc = [&]() {
				type = State::kompeitoGalaxy;
			};

		}break;
		case State::kompeitoGalaxy: {
			timer = 20.0;
			character.addMotion(U"KompeitoGalaxy");

			constexpr double eventInterval = 0.1;
			accumulatedTime = 0;
			const double stageWidth = DataManager::get().stageSize.x;

			double randomX=Random(double(rect_size*1.5),stageWidth-rect_size*1.5);

			double theata = 0;

			updateFunc = [=]()mutable{

				theata += Scene::DeltaTime() * 0.5;
				pos.y = rect_size * 3+ Periodic::Sine0_1(1s, theata)*rect_size*1.5;

				if (timer < 18.1) {


					pos.x=LinerMove(pos.x, randomX, 100);

					if (pos.x == randomX) {
						changeDirection();
					}


					accumulatedTime += Scene::DeltaTime();
					if (eventInterval <= accumulatedTime)
					{
						const Vec2 umbPos = Figure{ character.character.table[U"saki"].joint.getQuad() }.center();
						DataManager::get().additiveEffect.add<ExplosionEffect>(umbPos, 35, HSV{ 60,0.8,1 });

						double angle = Math::Sin(timer * 0.5) * 360_deg;
						Vec2 konpeitoVel = OffsetCircular{ {0,0},Math::Cos(angle) * 100 + 150,angle };



						manager->add(new DanmakuKompeito{ umbPos ,konpeitoVel });

						accumulatedTime -= eventInterval;
					}
				}
			};
			endFunc = [=]() {
				type = State::stand;
				floatFlg = false;
			};


		}break;
		case State::masterSparkPreJump:{
			character.addMotion(U"Jump1");

			timer = 1.0;
			constexpr double eventInterval = 0.05;
			accumulatedTime = 0;

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
			constexpr double eventInterval = 0.05;

			accumulatedTime = 0;

			const double stageWidth = DataManager::get().stageSize.x;

			bool playerLeft = (manager->get(U"Player")->pos.x) < (stageWidth/ 2.0);

			double targetX = playerLeft ? (stageWidth - rect_size * 2) : rect_size * 2;
			double lenX = Abs(pos.x - targetX);
			double targetY = rect_size * 3;
			double lenY = Abs(pos.y - targetY);
			updateFunc = [=]() {
				changeDirection();

				pos.x = LinerMove(pos.x, targetX, lenX / timeLim);
				pos.y = LinerMove(pos.y, targetY, lenY / timeLim);

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

			AudioAsset{ U"ビーム" }.playOneShot();

			updateFunc = [=]() {
				//向きは変えない

				attack(U"Player", RectF(Arg::leftCenter = pos, 1500, 100).rotatedAt(pos, magicCircle.rad), 1.0, DamageType::UnBrakable);
				character.character.table[U"rarm"].joint.angle = left ? 180_deg -magicCircle.rad - 120_deg : magicCircle.rad - 120_deg;
			};

			endFunc = [&]() {
				magicCircle.end();
				type = State::landing;
				floatFlg = false;

			};

		}break;
		case State::landing: {

			timer = 2.0;
			character.addMotion(U"Stand");

			updateFunc = [=]() {

			};

			endFunc = [&]() {
				type = State::kompeitoGalaxyJump;

			};

		}

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

	if (0 < damageTimer) {
		damageTimer -= Scene::DeltaTime();
	}



	magicCircle.update();

	character.update(pos, left);
}
