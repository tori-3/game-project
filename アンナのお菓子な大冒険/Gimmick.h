#pragma once
#include"Common.h"
#include"Entity.h"

class GoalDoor :public Entity
{
public:

	bool timerStart = false;
	double timer = 1;

	GoalDoor(const Vec2& pos);

	void update()override;

	void draw()const override;
};

class FallingRocks :public Entity
{
public:
	FallingRocks(const Vec2& cpos);

	void update()override;

	void draw()const override;

	bool isActive()override;
};

class RollingRocks :public Entity
{
public:

	Audio audio = AudioAsset{ U"ドーナツ" };

	const double audioLength = audio.lengthSec();

	Stopwatch time;

	RollingRocks(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	bool isActive()override;

	double volume()const
	{
		return Clamp((2 - Abs(DataManager::get().playerPos.x - pos.x)/ (rect_size*10)), 0.0, 1.0);
	}
};

class SnowBall :public Entity
{
public:
	bool left;

	double time = 0;

	SnowBall(const Vec2& pos, bool left);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class Kompeito :public Entity
{
public:
	double velX;

	size_t type = Random(2);

	Kompeito(const Vec2& cpos, double velX, double velY = -300);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	bool isActive()override;
};

class DanmakuKompeito :public Entity
{
public:
	size_t type = Random(2);

	double timer;

	DanmakuKompeito(const Vec2& cpos, const Vec2& vel, double timeLim = 20);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	bool isActive()override;
};

class BigElevator:public Entity
{
public:

	Size stageSize;

	BigElevator(const Size& stageSize)
		:Entity{ U"BigElevator",RectF{0,0,0,0},pos,{0,0},1 }, stageSize{ stageSize }
	{
		DataManager::get().isElevatorStage = true;
		DataManager::get().elevatorPosY = (stageSize.y - 1) * rect_size;

		z = 100;
	}

	void update()override
	{
		DataManager::get().elevatorPosY = Max<double>(DataManager::get().elevatorPosY - Scene::DeltaTime() * 100, rect_size * 11);
	}

	void lateUpdate()override
	{

	}

	void draw()const override
	{
		TextureAsset{ U"船体" }.draw(-rect_size, DataManager::get().elevatorPosY);
	}

	bool isActive()override
	{
		return true;
	}
};

class BigRollingRocks :public Entity
{
public:

	Audio audio = AudioAsset{ U"ドーナツ" };

	const double audioLength = audio.lengthSec();

	Stopwatch time;

	BigRollingRocks(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	bool isActive()override;

	double volume()const
	{
		return Clamp((2 - Abs(DataManager::get().playerPos.x - pos.x) / (rect_size * 10)), 0.0, 1.0);
	}
};
