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

	RollingRocks(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	bool isActive()override;
};

class SnowBall :public Entity
{
public:
	bool left;

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
