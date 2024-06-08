#include"LastBoss.h"


void LastBoss::update() {

	manager->stage->hit(&hitBox);


	hitBox.physicsUpdate();
	hitBox.update();

	left = (manager->get(U"Player")->pos.x < pos.x);


	if (timer <= 0) {

		switch (type)
		{
		case 0: {

			timer = 1.0;
			character.addMotion(U"kick1");
			updateFunc = [&]() {

				if ((not kickFlg) and timer < 1.0 - 0.4) {
					kickFlg = true;
					vel.x = left ? -600 : 600;
				}

				if (timer < 0.4) {

					attack(U"Player", RectF{ Arg::center(pos + (left ? Vec2{-65,-10} : Vec2{65,-10})),65,20 }, 1);
				}
			};

			endFunc = [&]() {
				type = 1;
				kickFlg = false;
			};

		}break;
		case 1: {

			timer = 0.5;
			character.addMotion(U"Stand");

			updateFunc = []() {

			};

			endFunc = [&]() {
				type = RandomBool() ? 0 : 2;


			};

		}break;
		case 2: {
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
				type = 1;


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

	character.update(pos, left);
}
