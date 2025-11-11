#pragma once
#include"Common.h"
#include"Entity.h"
#include"ExplosionEffect.h"
#include"CharacterSystem.h"
#include"SimpleAction.h"
#include"BGMManager.hpp"
#include"MagicEffect.h"
#include"MagicCircle.h"
#include"LaserEffect.h"
#include"Enemy.h"

#include"LinerMove.h"

#include"MyRandom.h"

class ClosedUmbrella:public Entity
{
public:
	CharacterSystem character;

	double angle;

	double speed;

	double time=0;

	bool effectFlg = false;

	static constexpr double eventInterval = 0.05;
	double accumulatedTime = 0.0;

	bool audioStartFlg = false;

	ClosedUmbrella(const Vec2& cpos, double angle, double speed);

	void update()override;

	void draw()const override;

	bool isActive()override;
};

class ChaseUmbrella :public Entity
{
public:
	CharacterSystem character;

	double angle;

	double targetAngle = 0;

	double speed;

	double time = 0;

	static constexpr double eventInterval = 0.05;
	double accumulatedTime = 0.0;

	bool rotateFlg = false;

	double timeLim;

	ChaseUmbrella(const Vec2& cpos, double angle, double speed, double time);

	void update()override;

	bool isActive()override;

	void draw()const override;
};

class EnemyUmbrella :public Entity {
public:
	CharacterSystem character;

	Entity* child;

	EnemyUmbrella(const Vec2& pos, Entity* child);

	void update()override;

	void lateUpdate()override;

	bool isActive()override;

	void draw()const override;
};

class LastBoss :public Entity
{
public:

	static constexpr int32 maxHp = 50;

	bool left = true;

	CharacterSystem character;

	std::function<void()> updateFunc;
	std::function<void()> endFunc;

	double timer = 0;

	bool floatFlg = false;

	enum class State{kick,stand,throwUmbrella, enemyFalls, umbrellaShot, reflectionUmbrella, kompeitoGalaxyJump, kompeitoGalaxy, masterSparkPreJump, masterSparkJump, masterSparkWait, masterSpark, landing} type=State::stand;

	MagicCircle magicCircle;

	double accumulatedTime = 0.0;

	bool damageFlg = false;

	static constexpr double mutekiTime = 1.5;
	double damageTimer = 0;

	bool kickFlg = false;

	bool umbrellaFlg = false;

	int32 throwUmbrellaCount = 0;

	bool playLastBGM = false;

	MyRandom rand1{ 3 };
	MyRandom rand2{ 4 };

	LastBoss(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	void damage(int32 n, const Vec2&, DamageType)override;

	bool isLastSpart()
	{
		return hp <= 25;
	}

	void changeDirection()
	{
		left = (manager->get(U"Player")->pos.x < pos.x);
	}
};
