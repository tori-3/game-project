#include"Enemy.h"
#include"LinerMove.h"

StrawberrySoldier::StrawberrySoldier(const Vec2& cpos)
	: Entity{ U"Enemy", RectF{Arg::center(0,0),70,69},cpos,{0,0},1 }
	, character{ U"Characters/itigo/itigo.json" ,U"Characters/itigo/motion.txt" ,0.3,cpos,true,false }
{
	character.addMotion(U"", true);
}

StrawberrySoldier::StrawberrySoldier(const Vec2& cpos,Big)
	: Entity{ U"Enemy", RectF{Arg::center(0,0),70*2,69*2},cpos,{0,0},5 }
	, character{ U"Characters/itigo/itigo.json" ,U"Characters/itigo/motion.txt" ,0.3*2,cpos,true,false }
	,isBig(true)
{
	character.addMotion(U"", true);
}

void StrawberrySoldier::update()
{
	manager->stage->hit(&hitBox);

	if (hitBox.touch(Direction::right))
	{
		left = true;
	}
	else if (hitBox.touch(Direction::left))
	{
		left = false;
	}

	if (hitBox.touch(Direction::down))
	{
		if (left)
		{
			if (not hitBox.leftFloor())
			{
				left = false;
			}
		}
		else
		{
			if (not hitBox.rightFloor())
			{
				left = true;
			}
		}
	}

	if (left)
	{
		vel.x = -100;
	}
	else
	{
		vel.x = 100;
	}

	hitBox.physicsUpdate();
	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);

	character.update(pos, left);
}

void StrawberrySoldier::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 0);
		manager->add(new CookieItem{ pos });
	}
}

void StrawberrySoldier::draw()const
{
	character.draw();
}

CookieSoldier::CookieSoldier(const Vec2& cpos)
	: Entity{ {U"Enemy"}, RectF{Arg::center(0,0),70,69},cpos,{0,0},1 }
	, character{ U"Characters/cookie/cookie.json",U"Characters/cookie/motion_cookie.txt",0.3,cpos,true,false }
{
	character.addMotion(U"", true);
}

CookieSoldier::CookieSoldier(const Vec2& cpos,Big)
	: Entity{ {U"Enemy"}, RectF{Arg::center(0,0),70*2,69*2},cpos,{0,0},1 }
	, character{ U"Characters/cookie/cookie.json",U"Characters/cookie/motion_cookie.txt",0.3*2,cpos,true,false }
	, isBig(true)
{
	character.addMotion(U"", true);
}

void CookieSoldier::update()
{
	double speedScale = 1;
	int32 range = 5;

	if(isBig)
	{
		speedScale = 1.5;
		range = 8;
	}

	manager->stage->hit(&hitBox);

	if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * range)
	{

		if (manager->get(U"Player")->pos.x < pos.x)
		{
			left = true;

			speed -= 600 * speedScale* Scene::DeltaTime();
		}
		else if (manager->get(U"Player")->pos.x >= pos.x)
		{
			left = false;
			speed += 600 * speedScale* Scene::DeltaTime();
		}
	}
	else {
		speed = 0;
	}

	speed = Clamp(speed, -200.0 * speedScale, 200.0 * speedScale);

	vel.x = speed;

	//プレイヤーに近すぎる場合
	if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2)
	{
		left = false;
	}

	hitBox.physicsUpdate();
	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);

	character.update(pos, left);
}

void CookieSoldier::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 50);
		manager->add(new CookieItem{ pos });
	}
}

void CookieSoldier::draw()const
{
	character.draw();
}

void CookieSoldier::damage(int32 n, const Vec2&, DamageType)
{
	if (not isBig)
	{
		Entity::damage(n);
	}
}

Snowman::Snowman(const Vec2& cpos)
	: Entity{ U"Enemy", RectF{Arg::center(0,0),70,130},cpos,{0,0},1 }
	, character{ U"Characters/yukidaruma/test.json",U"Characters/yukidaruma/motion.txt",0.5,cpos,true,false }
{
	character.addMotion(U"Walk");
}

void Snowman::update()
{

	attackMode = false;

	manager->stage->hit(&hitBox);

	Vec2 playerPos = manager->get(U"Player")->pos;
	if (Abs(playerPos.x - pos.x) < rect_size * 7 + 5 / 10.0 * Max(playerPos.y - pos.y, 0.0))
	{
		attackMode = true;
		left = playerPos.x < pos.x;
	}
	else
	{
		if (not character.character.hasMotion(U"Walk"))
		{
			character.addMotion(U"Walk");
		}

		if (hitBox.touch(Direction::right))
		{
			left = true;
		}
		else if (hitBox.touch(Direction::left))
		{
			left = false;
		}
		if (hitBox.touch(Direction::down))
		{
			if (left)
			{
				if (not hitBox.leftFloor())
				{
					left = false;
				}
			}
			else
			{
				if (not hitBox.rightFloor())
				{
					left = true;
				}
			}
		}

		if (left)
		{
			vel.x = -100;
		}
		else
		{
			vel.x = 100;
		}
	}

	hitBox.physicsUpdate();
	hitBox.update();

	if (attackMode)
	{
		constexpr double spawn = 2;
		for (attackAccumlater += Scene::DeltaTime(); spawn <= attackAccumlater; attackAccumlater -= spawn) {

			character.addMotion(U"Attack");
			holdSnowBall = true;
		}

	}
	else
	{
		attackAccumlater = 1.5;
	}

	if (holdSnowBall)
	{
		t += Scene::DeltaTime();

		if (0.6 < t)
		{
			holdSnowBall = false;
			const Mat3x2 mat = character.character.table.at(U"arm").joint.mat;
			const Vec2 ballPos = mat.transformPoint(Vec2{ 100, -30 });
			manager->add(new SnowBall{ ballPos,left });
			t = 0;
		}

	}

	attack(U"Player", hitBox.getFigure(), 1);

	character.update(pos, left);
}

void Snowman::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 180,0.2);
		manager->add(new CookieItem{ pos });
	}
}

void Snowman::draw()const
{
	character.draw();
	if (holdSnowBall)
	{
		const Mat3x2 mat = character.character.table.at(U"arm").joint.mat;
		const Vec2 ballPos = mat.transformPoint(Vec2{ 100, -30 });
		//Circle{ ballPos,10 }.draw(Palette::White);
		TextureAsset{ U"snowball" }.resized(20).drawAt(ballPos);
	}
}



ItigoSlave::ItigoSlave(const Vec2& cpos) :Entity{ U"Enemy", RectF{Arg::center(0,15),70 * 1.5,69 * 1.5 },cpos,{0,0},2 }
, character{ U"Characters/itigoSlave/itigoSlave.json" ,U"Characters/itigoSlave/motion.txt" ,0.3,cpos,true,false }
{
	character.addMotion(U"walk", true);
}

ItigoSlave::ItigoSlave(const Vec2& cpos,Big) :Entity{ U"Enemy", RectF{Arg::center(0,15),70 * 1.5*2,69 * 1.5*2 },cpos,{0,0},10 }
, character{ U"Characters/itigoSlave/itigoSlave.json" ,U"Characters/itigoSlave/motion.txt" ,0.3*2,cpos,true,false }
,isBig(true)
{
	character.addMotion(U"walk", true);
}

void ItigoSlave::update()
{

	attackTimer -= Scene::DeltaTime();

	manager->stage->hit(&hitBox);
	if (not character.hasMotion(U"Muteki"))
	{
		if (hitBox.touch(Direction::right))
		{
			left = true;
		}
		else if (hitBox.touch(Direction::left))
		{
			left = false;
		}

		if (hitBox.touch(Direction::down))
		{
			if (left)
			{
				if (not hitBox.leftFloor())
				{
					left = false;
				}
			}
			else
			{
				if (not hitBox.rightFloor())
				{
					left = true;
				}
			}
		}

		if (left)
		{
			vel.x = -100;
		}
		else
		{
			vel.x = 100;
		}
	}
	hitBox.physicsUpdate();
	hitBox.update();

	if (not character.hasMotion(U"Muteki"))
	{

		attack(U"Player", hitBox.getFigure(), 1);

		if ((not character.hasMotion(U"attack")) && manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -60 : 60, 0)))
		{
			attackTimer = attackTime;
			character.addMotion(U"attack");
		}

		if (attackTimer <= 0)
		{
			if (manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -25 : 25, 0)))
			{
				manager->get(U"Player")->damage(2, Vec2{ left ? -200 : 200,-20 });
			}
		}
	}

	character.update(pos, left);
}

void ItigoSlave::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 0);
		manager->add(new CookieItem{ pos });
	}
}

void ItigoSlave::damage(int32 n, const Vec2& force, DamageType)
{
	character.removeMotion(U"attack");
	hp -= n;

	if(not isBig)
	{
		character.removeMotion(U"Muteki");
		character.addMotion(U"Muteki");
		vel.y = force.y;
		vel.x = force.x * 1.5;
	}
}

void ItigoSlave::draw()const
{
	character.draw();
}


CloudEnemy::CloudEnemy(const Vec2& cpos)
	: Entity{ U"Enemy", RectF{Arg::center(0,0),70 * 1.5,69 },cpos,{0,0},1 }
	, character{ U"Characters/cloud/cloud.json" ,U"Characters/cloud/motion.txt" ,0.5,cpos,true,false }
{
	character.addMotion(U"walk", true);
	startY = cpos.y;

	z = 110;

	time = Random(1.5, 2.5);
}

void CloudEnemy::update()
{
	hitBox.update();

	if (manager->get(U"Player")->pos.x < pos.x) {
		vel.x -= 500 * Scene::DeltaTime();
	}
	else if (manager->get(U"Player")->pos.x >= pos.x) {
		vel.x += 500 * Scene::DeltaTime();
	}

	vel.x = Clamp(vel.x, -200.0, 200.0);

	pos.x += (vel.x + Periodic::Sine1_1(time, DataManager::get().time) * 100) * Scene::DeltaTime();

	pos.y = startY + Periodic::Sine1_1(time, DataManager::get().time) * 30;

	attack(U"Player", hitBox.getFigure(), 1);

	if (Abs(manager->get(U"Player")->pos.x - pos.x) < 70 * 6) {

		accumulatedTime += Scene::DeltaTime();
		constexpr double eventInterval = 2.5;
		if (eventInterval <= accumulatedTime)
		{
			manager->add(new Kompeito{ pos,vel.x + vel.x * Random(1.0) });
			character.addMotion(U"attack");

			accumulatedTime -= eventInterval;
		}
	}

	character.update(pos, 0 < vel.x);
}

void CloudEnemy::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 310,0.3);
		manager->add(new CookieItem{ pos });
	}
}

void CloudEnemy::draw()const
{
	character.draw();
}



Corn::Corn(const Vec2& cpos) :Entity{ {U"Enemy"}, Circle{30},cpos,{0,0},1 },
character{ U"Characters/corn/corn.json",U"Characters/corn/motion.txt",0.3,cpos,true,false }
{
	character.addMotion(U"walk", true);

	z = 110;
}

void Corn::update()
{

	if (corn)
	{
		if (not startShake)
		{
			if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 5)
			{
				pos = Math::SmoothDamp(pos, manager->get(U"Player")->pos, vel, 0.1, 50);
				left = vel.x < 0;
			}
		}
		hitBox.update();

		if (startShake)
		{
			if (not character.hasMotion(U"explode"))
			{
				character.addMotion(U"change");
				DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200, HSV{ 20,1,1 });
				AudioAsset{ U"ポップコーン爆発" }.playOneShot();

				z = 0;

				if ((manager->get(U"Player")->pos - pos).length() < 70 * 2)
				{
					if (pos.x < manager->get(U"Player")->pos.x)
					{
						manager->get(U"Player")->damage(1, Vec2{ 100,-20 }, DamageType::UnBrakable);
					}
					else
					{
						manager->get(U"Player")->damage(1, Vec2{ -100,-20 }, DamageType::UnBrakable);
					}
				}

				if (pos.x < manager->get(U"Player")->pos.x)
				{
					vel = Vec2{ -600,-400 };
				}
				else
				{
					vel = Vec2{ 600,-400 };
				}

				corn = false;
			}
		}
		else if ((manager->get(U"Player")->pos - pos).length() < 70 * 1.5)
		{
			character.clearMotion();
			character.addMotion(U"explode");
			startShake = true;
		}
	}
	else {
		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::right))
		{
			left = true;
		}
		else if (hitBox.touch(Direction::left))
		{
			left = false;
		}

		if (hitBox.touch(Direction::down))
		{
			touch = true;

			if (left)
			{
				if (not hitBox.leftFloor())
				{
					left = false;
				}
			}
			else
			{
				if (not hitBox.rightFloor())
				{
					left = true;
				}
			}
		}

		//着地前に歩くと爆発で吹き飛ばないため。
		if (touch)
		{
			if (left)
			{
				vel.x = -100;
			}
			else
			{
				vel.x = 100;
			}
		}

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

	}


	character.update(pos, left);
}

void Corn::lateUpdate()
{
	if (not isActive())
	{
		if (corn)
		{
			character.clearMotion();
			startShake = true;
			hp = 1;
		}
		else
		{
			DataManager::get().effect.add<StarEffect>(pos, 50);
			manager->add(new CookieItem{ pos });
		}
	}
}

void Corn::draw()const
{
	character.draw();
}



CookieMuti::CookieMuti(const Vec2& cpos)
	: Entity{ {U"Enemy"}, RectF{Arg::center(0,0),50,100},cpos,{0,0},1 }
	, character{ U"Characters/cookieMuti/cookieMuti.json",U"Characters/cookieMuti/motion.txt",0.4,cpos,true,false }
{
}

void CookieMuti::update()
{
	attackTimer -= Scene::DeltaTime();

	manager->stage->hit(&hitBox);

	if (manager->get(U"Player")->pos.x < pos.x)
	{
		left = true;
		vel.x = -200;
	}
	else if (manager->get(U"Player")->pos.x >= pos.x)
	{
		left = false;
		vel.x = 200;
	}

	//プレイヤーに近すぎる場合
	if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2)
	{
		left = false;
	}

	hitBox.physicsUpdate();
	hitBox.update();

	if ((not character.hasMotion(U"attack")) && manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -60 : 60, 0).scaled(3, 1)))
	{
		character.removeMotion(U"walk");
		character.addMotion(U"attack");
		attackTimer = attackInterval;
	}

	if (not character.hasMotion(U"attack") && not character.hasMotion(U"walk"))
	{
		character.addMotion(U"walk");
	}

	if (attackTimer <= 0)
	{
		attackTimer = attackInterval;
		attack(U"Player", hitBox.Get_Box().movedBy(left ? -60 : 60, 0), 2, DamageType::UnBrakable);
	}
	else
	{
		attack(U"Player", hitBox.getFigure(), 1);
	}

	character.update(pos, left);
}

void CookieMuti::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 50);
		manager->add(new CookieItem{ pos });
	}
}

void CookieMuti::draw()const
{
	character.draw();
}

CookieKaban::CookieKaban(const Vec2& cpos)
	: Entity{ {U"Enemy"}, RectF{Arg::center(0,0),50,100},cpos,{0,0},1 }
	, character{ U"Characters/cookieKaban/cookieKaban.json",U"Characters/cookieKaban/motion.txt",0.4,cpos,true,false }
{
}

void CookieKaban::update()
{
	manager->stage->hit(&hitBox);

	if (manager->get(U"Player")->pos.x < pos.x)
	{
		left = true;
		vel.x = -200;
	}
	else if (manager->get(U"Player")->pos.x >= pos.x)
	{
		left = false;
		vel.x = 200;
	}

	//プレイヤーに近すぎる場合
	if (Abs(manager->get(U"Player")->pos.x - pos.x) < rect_size * 0.2)
	{
		left = false;
	}

	hitBox.physicsUpdate();
	hitBox.update();

	if ((not character.hasMotion(U"attack")) && manager->get(U"Player")->hitBox.Get_Box().intersects(hitBox.Get_Box().movedBy(left ? -20 : 20, 0).scaled(3, 1)))
	{
		character.removeMotion(U"walk");
		character.addMotion(U"attack");
		attackTimer = attackInterval;
	}

	if (not character.hasMotion(U"attack") && not character.hasMotion(U"walk"))
	{
		character.addMotion(U"walk");
	}

	if (attackTimer <= 0)
	{
		attackTimer = attackInterval;
		attack(U"Player", hitBox.Get_Box(), 2, DamageType::UnBrakable);
	}
	else
	{
		attack(U"Player", hitBox.Get_Box(), 1);
	}


	character.update(pos, left);
}

void CookieKaban::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 50);
		manager->add(new CookieItem{ pos });
	}
}

void CookieKaban::draw()const
{
	character.draw();
}



DropCorn::DropCorn(const Vec2& cpos, double velX)
	: Entity{ {U"Enemy"}, Circle{30},cpos,{0,0},1 }
	, character{ U"Characters/corn/corn.json",U"Characters/corn/motion.txt",0.3,cpos,true,false }, velX{ velX }
{
	++count;

	if(50<count)
	{
		kill();
	}
}

DropCorn::~DropCorn()
{
	--count;
}

void DropCorn::update()
{
	if (corn)
	{
		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::down) || damaged)
		{
			character.addMotion(U"change");
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200, HSV{ 20,1,1 });
			AudioAsset{ U"ポップコーン爆発" }.playOneShot();


			attack(U"Player", Circle{ pos,70 * 2 }, 1);

			if (pos.x < manager->get(U"Player")->pos.x)
			{
				vel = Vec2{ -600,-400 };

			}
			else
			{
				vel = Vec2{ 600,-400 };
			}
			corn = false;
		}

		vel.x = velX;
		hitBox.physicsUpdate();
		hitBox.update();
	}
	else
	{
		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::right))
		{
			left = true;
		}
		else if (hitBox.touch(Direction::left))
		{
			left = false;
		}

		if (hitBox.touch(Direction::down))
		{
			touch = true;

			if (left) {
				if (not hitBox.leftFloor())
				{
					left = false;
				}
			}
			else {
				if (not hitBox.rightFloor())
				{
					left = true;
				}
			}
		}

		//着地前に歩くと爆発で吹き飛ばないため。
		if (touch) {
			if (left) {
				vel.x = -100;
			}
			else {
				vel.x = 100;
			}
		}

		hitBox.physicsUpdate();
		hitBox.update();

		attack(U"Player", hitBox.getFigure(), 1);

	}


	character.update(pos, left);
}

void DropCorn::lateUpdate()
{
	if (not isActive())
	{
		if (corn)
		{
			damaged = true;
			hp = 1;
		}
		else
		{
			DataManager::get().effect.add<StarEffect>(pos, 50,0.3);
		}
	}
}

void DropCorn::draw()const
{
	character.draw();
}

int32 DropCorn::count = 0;


Zerosen::Zerosen(const Vec2& cpos)
	: Entity{ U"Enemy", RectF{Arg::center(0,-15),70 * 1.5,50 },cpos,{0,0},1 }
	, character{ U"Characters/zerosen/model.json" ,U"Characters/zerosen/motion.txt" ,1,cpos,false,false }
{
	character.addMotion(U"susumu");
	z = 110;
}

void Zerosen::update()
{

	if (manager->get(U"Player")->pos.x < pos.x)
	{
		vel.x -= 500 * Scene::DeltaTime();
	}
	else if (manager->get(U"Player")->pos.x >= pos.x)
	{
		vel.x += 500 * Scene::DeltaTime();
	}

	vel.x = Clamp(vel.x, -400.0, 400.0);

	pos.x += vel.x * Scene::DeltaTime();



	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);

	if (Abs(manager->get(U"Player")->pos.x - pos.x) < 70 * 6)
	{
		accumulatedTime += Scene::DeltaTime();
		constexpr double eventInterval = 2.5;
		if (eventInterval <= accumulatedTime)
		{
			manager->add(new DropCorn{ pos,vel.x + vel.x * Random(1.0) });

			accumulatedTime -= eventInterval;
		}
	}

	character.update(pos, 0 < vel.x);
}

void Zerosen::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 50);
		manager->add(new CookieItem{ pos });
	}
}

void Zerosen::draw()const
{
	character.draw();
}

Needle::Needle(const Vec2& pos, int32 angle)
	:Entity{ U"Rocks", RectF::Empty(),pos,{0,0},1}
	,angle{angle}
{ }

void Needle::update()
{
	Vec2 rectPos = pos;

	switch (angle)
	{
	case 0:rectPos.y -= rect_size / 4.0; break;
	case 1:rectPos.x += rect_size / 4.0; break;
	case 2:rectPos.y += rect_size / 4.0; break;
	case 3:rectPos.x -= rect_size / 4.0; break;
	}

	attack(U"Player", RectF{ Arg::center(rectPos),rect_size / 2.0 }, 1);
}

void Needle::draw()const
{
	TextureAsset{ U"PointedCorn" }.resized(rect_size).rotated(angle * 90_deg).drawAt(pos);
}

void Needle::damage(int32, const Vec2&, DamageType)
{

}

BigCloudEnemy::BigCloudEnemy(const Vec2& cpos)
	: Entity{ U"Gimmick", RectF::Empty(),cpos,{0,0},1}
	, character{ U"Characters/cloud/cloud.json" ,U"Characters/cloud/motion.txt" ,5,cpos,true,false }
{
	character.addMotion(U"walk", true);
	startY = cpos.y;
}

void BigCloudEnemy::update()
{
	hitBox.update();

	if (manager->get(U"Player")->pos.x < pos.x) {
		vel.x -= 700 * Scene::DeltaTime();
	}
	else if (manager->get(U"Player")->pos.x >= pos.x) {
		vel.x += 700 * Scene::DeltaTime();
	}

	vel.x = Clamp(vel.x, -300.0, 300.0);

	pos.x += (vel.x + Periodic::Sine1_1(2, DataManager::get().time) * 100) * Scene::DeltaTime();

	pos.y = startY + Periodic::Sine1_1(2, DataManager::get().time) * 30;


	accumulatedTime += Scene::DeltaTime();

	constexpr double eventInterval = 20;
	if (eventInterval <= accumulatedTime)
	{
		character.addMotion(U"attack");

		accumulatedTime -= eventInterval;
	}

	character.update(pos, 0 < vel.x);
}

void BigCloudEnemy::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 0);
		manager->add(new CookieItem{ pos });
	}
}

void BigCloudEnemy::draw()const
{
	ScopedColorMul2D mul{ ColorF{1,0.85} };

	character.draw();
}

void BigCloudEnemy::damage(int32, const Vec2&, DamageType)
{}


StrawberrySoldierTower::StrawberrySoldierTower(const Vec2& cpos, bool root)
	: Entity{ U"Enemy", RectF{Arg::center(0,0),70,69},cpos,{0,0},1 }
	, character{ U"Characters/itigo/itigo.json" ,U"Characters/itigo/motion.txt" ,0.3,cpos,true,false }
	,bornFlg{not root}
{
	character.addMotion(U"", true);
}

void StrawberrySoldierTower::update()
{
	if (not bornFlg)
	{
		bornFlg = true;

		StrawberrySoldierTower* tale = this;
		for (int32 i = 0; i < 7; ++i)
		{
			StrawberrySoldierTower* p = new StrawberrySoldierTower(pos + Vec2{ 0,-65 * (1 + i) });
			p->parent = tale;
			manager->add(p);
			tale = p;
		}
	}

	if(parent)
	{
		pos = parent->pos + Vec2{ 0,-65 };
		left = parent->left;
	}
	else
	{
		manager->stage->hit(&hitBox);

		if (hitBox.touch(Direction::right))
		{
			left = true;
		}
		else if (hitBox.touch(Direction::left))
		{
			left = false;
		}

		if (hitBox.touch(Direction::down))
		{
			if (left)
			{
				if (not hitBox.leftFloor())
				{
					left = false;
				}
			}
			else
			{
				if (not hitBox.rightFloor())
				{
					left = true;
				}
			}
		}

		if (left)
		{
			vel.x = -100;
		}
		else
		{
			vel.x = 100;
		}

		if(stop)
		{
			vel.x = 0;

			if(hitBox.touch(Direction::down))
			{
				stop = false;
			}
		}

		hitBox.physicsUpdate();
	}

	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);

	character.update(pos, left);
}

void StrawberrySoldierTower::lateUpdate()
{
	if (parent)
	{
		if (not parent->isActive())
		{
			stop = true;
			vel.y = -300;
			parent = nullptr;
		}
	}

	if (not isActive())
	{
		DataManager::get().effect.add<StarEffect>(pos, 0);
		manager->add(new CookieItem{ pos });
	}
}

void StrawberrySoldierTower::draw()const
{
	character.draw();
}

void StrawberrySoldierTower::damage(int32 n, const Vec2&, DamageType)
{
	Entity::damage(n);
}


BigCorn::BigCorn(const Vec2& cpos) :Entity{ {U"Gimmick"}, Circle{0},cpos,{0,0},1 },
character{ U"Characters/corn/corn.json",U"Characters/corn/motion.txt",0.3*10,cpos,true,false }
{
	character.addMotion(U"bigExplode");

	//z = 110;
}

BigCorn::~BigCorn()
{
	AudioAsset{ U"風が吹く2" }.stop(0.5s);
}

void BigCorn::update()
{
	if(DataManager::get().blast)
	{
		AudioAsset{ U"風が吹く2" }.play();

		accumulateTime += Scene::DeltaTime();

		if(0.15<accumulateTime)
		{
			accumulateTime -= 0.15;

			const Vec2 pos = Vec2{ DataManager::get().playerPos.x - 400,Random(Scene::Height())};

			DataManager::get().effect.add([pos=pos](double t)mutable
			{
				Line{ pos - Vec2{150,0},pos }.draw(LineStyle::RoundCap,4,ColorF(0.95,0.5));
				pos.x += Scene::DeltaTime() * 1200;
				return (t < 3.0);
			});
		}

		if (DataManager::get().stageSize.x - 10 * rect_size < DataManager::get().playerPos.x)
		{
			DataManager::get().blast = false;
			kill();
		}

	}
	else
	{
		if (not character.hasMotion(U"bigExplode"))
		{
			character.addMotion(U"change");
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200 * 3, HSV{ 20,1,1 });

			DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 200 * 2);
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos + Vec2{ 50,50 }*2, 100 * 2);
			DataManager::get().additiveEffect.add<ExplosionEffect>(pos - Vec2{ 50,50 }*2, 100 * 2);
			AudioAsset{ U"爆発4" }.playOneShot();
			DataManager::get().blast = true;
		}

		character.update(pos, true);
	}
}

void BigCorn::draw()const
{
	if(not DataManager::get().blast)
	{
		character.draw();
	}
}
