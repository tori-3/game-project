#include"Gimmick.h"
#include"ExplosionEffect.h"

GoalDoor::GoalDoor(const Vec2& pos)
	:Entity{ U"Door",RectF{0,0,0,0},pos,{0,0},1 }
{
	z = -100;
}

void GoalDoor::update()
{
	if (RectF{ pos,rect_size * 2 }.intersects(DataManager::get().playerPos))
	{
		timerStart = true;
		DataManager::get().table.emplace(U"Clear");
	}

	if (timerStart)
	{
		timer -= Scene::DeltaTime();
		timer = Max(timer, 0.0);
	}
}

void GoalDoor::draw()const
{
	RectF{ pos,rect_size * 2 }.draw(Palette::Black);
	double d = Sin(timer * 80_deg + 10_deg);
	TextureAsset(U"Door").resized(rect_size * 2).scaled(d, 1).draw(pos);
	TextureAsset(U"Door").resized(rect_size * 2).scaled(d, 1).mirrored().draw(pos + Vec2::UnitX() * (1 - d) * 2 * rect_size);
}



FallingRocks::FallingRocks(const Vec2& cpos)
	:Entity{ U"Rocks", RectF{ Arg::center(0,0),rect_size / 2.0 },cpos,{0,0},1 }
{

}

void FallingRocks::update()
{
	hitBox.physicsUpdate();
	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1, DamageType::UnBrakable);
}

void FallingRocks::draw()const
{
	TextureAsset{ U"PointedCorn" }.resized(rect_size).drawAt(pos);
}

bool FallingRocks::isActive()
{
	return pos.y < DataManager::get().stageSize.y + 200;
}


RollingRocks::RollingRocks(const Vec2& cpos)
	:Entity{ U"Rocks", Circle{ 0,0,rect_size },cpos,{0,0},1 }
{

}

void RollingRocks::update()
{
	manager->stage->hit(&hitBox);

	audio.play();

	pos.x -= Scene::DeltaTime() * 200;

	hitBox.physicsUpdate();
	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);
	attack(U"Enemy", hitBox.getFigure(), 1);
	attack(U"SlaversCookie", hitBox.getFigure(), 1);

	if (hitBox.touch(Direction::left))
	{
		hp = 0;
	}
}

void RollingRocks::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 100, Palette::Darkgray);
		audio.stop();
		AudioAsset{ U"ドーナツ衝突" }.playOneShot();
	}
}

void RollingRocks::draw()const
{
	TextureAsset{ U"Doughnut" }.resized(rect_size * 2).rotated(-DataManager::get().time * 120_deg).drawAt(pos);
}

bool RollingRocks::isActive()
{
	return 0 < hp && pos.y < DataManager::get().stageSize.y + 200;
}


SnowBall::SnowBall(const Vec2& pos, bool left)
	: Entity{ U"Enemy", Circle{0,0,10},pos,{0,0},1 }
	, left{ left }
{
	vel.y = -500;
}

void SnowBall::update()
{
	bool collisionFlg = false;

	manager->stage->hit(&hitBox);

	if (hitBox.touch(Direction::left) || hitBox.touch(Direction::right) || hitBox.touch(Direction::up) || hitBox.touch(Direction::down))
	{
		collisionFlg = true;
	}

	vel.x = left ? -400 : 400;

	hitBox.physicsUpdate();
	hitBox.update();

	if (manager->get(U"Player")->hitBox.intersects(hitBox))
	{
		manager->get(U"Player")->damage(1, Vec2{ left ? -100 : 100,-20 });
		collisionFlg = true;
	}

	if (collisionFlg)
	{
		hp = 0;
	}

}

void SnowBall::lateUpdate()
{
	if (not isActive())
	{
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, Palette::White);
		AudioAsset{ U"雪玉壊れる" }.playOneShot();
	}
}

void SnowBall::draw()const
{
	hitBox.Get_Box().draw(Palette::White);
}


Kompeito::Kompeito(const Vec2& cpos, double velX, double velY) :Entity{ U"Enemy", Circle{25},cpos,{0,0},1 }, velX{ velX }
{
	z = 110;
	vel.y = velY;
	if (not TextureAsset::IsRegistered(U"金平糖0"))
	{
		TextureAsset::Register(U"金平糖0", U"Characters/cloud/金平糖/konpeitou.png");
		TextureAsset::Register(U"金平糖1", U"Characters/cloud/金平糖/konpeitouB.png");
		TextureAsset::Register(U"金平糖2", U"Characters/cloud/金平糖/konpeitouR.png");
	}
}

void Kompeito::update()
{
	vel.x = velX;

	hitBox.physicsUpdate();
	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);
}

void Kompeito::lateUpdate()
{
	constexpr ColorF colorList[] = { Palette::Yellow,Palette::Blue,Palette::Red };
	if (not isActive())
	{
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, colorList[type]);
	}
}

void Kompeito::draw()const
{
	TextureAsset(U"金平糖{}"_fmt(type)).resized(100).drawAt(pos);
}

bool Kompeito::isActive()
{
	return 0 < hp && pos.y < DataManager::get().stageSize.y + 200;
}


DanmakuKompeito::DanmakuKompeito(const Vec2& cpos, const Vec2& vel, double timeLim)
	: Entity{ U"Enemy", Circle{25 * 0.8},cpos,vel,1 }
	, timer{ timeLim }
{
	z = 110;

	if (not TextureAsset::IsRegistered(U"金平糖0"))
	{
		TextureAsset::Register(U"金平糖0", U"Characters/cloud/金平糖/konpeitou.png");
		TextureAsset::Register(U"金平糖1", U"Characters/cloud/金平糖/konpeitouB.png");
		TextureAsset::Register(U"金平糖2", U"Characters/cloud/金平糖/konpeitouR.png");
	}
}

void DanmakuKompeito::update()
{
	timer -= Scene::DeltaTime();

	pos += vel * Scene::DeltaTime();
	hitBox.update();

	attack(U"Player", hitBox.getFigure(), 1);
}

void DanmakuKompeito::lateUpdate()
{
	constexpr ColorF colorList[] = { Palette::Yellow,Palette::Blue,Palette::Red };
	if (not isActive())
	{
		DataManager::get().additiveEffect.add<ExplosionEffect>(pos, 35, colorList[type]);
	}
}

void DanmakuKompeito::draw()const
{
	TextureAsset(U"金平糖{}"_fmt(type)).resized(100 * 0.8).rotated(timer * 360_deg).drawAt(pos);
}

bool DanmakuKompeito::isActive()
{
	return 0 < hp && 0 < timer;
}
