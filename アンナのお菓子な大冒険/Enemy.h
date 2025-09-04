#pragma once
#include"Common.h"
#include"Entity.h"
#include"Player.h"
#include"CookieItem.h"
#include"CharacterSystem.h"
#include"Gimmick.h"

#include"StarEffect.h"

struct Big {};

class StrawberrySoldier:public Entity
{
public:

	bool isBig = false;

	bool left=true;

	CharacterSystem character;

	StrawberrySoldier(const Vec2& cpos);

	StrawberrySoldier(const Vec2& cpos,Big);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	void damage(int32 n, const Vec2& force, DamageType)override;
};

class CookieSoldier :public Entity
{
public:

	bool isBig = false;

	bool left = false;

	CharacterSystem character;

	double speed = 0;

	CookieSoldier(const Vec2& cpos);

	CookieSoldier(const Vec2& cpos,Big);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	void damage(int32 n, const Vec2& force, DamageType)override;
};

class Snowman:public Entity
{
public:
	bool left = false;

	bool attackMode = false;

	double attackAccumlater=0;

	bool holdSnowBall = false;

	double t = 0;

	CharacterSystem character;

	Snowman(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class ItigoSlave :public Entity
{
public:
	bool left = true;

	bool isBig = false;

	CharacterSystem character;

	static constexpr double attackTime = 0.5;
	double attackTimer = 0;

	ItigoSlave(const Vec2& cpos);

	ItigoSlave(const Vec2& cpos,Big);

	void update()override;

	void lateUpdate()override;

	void damage(int32 n, const Vec2& force, DamageType)override;

	void draw()const override;
};

class CloudEnemy :public Entity
{
public:

	CharacterSystem character;

	double accumulatedTime = 0;

	double startY = 0;

	double time = 0;

	CloudEnemy(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class Corn :public Entity
{
public:

	bool left = false;

	bool startShake = false;

	bool corn = true;

	//爆発した後、地面に触れたか。
	bool touch = false;

	CharacterSystem character;

	Corn(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class CookieMuti :public Entity
{
public:

	static constexpr double attackInterval = 0.4;
	double attackTimer=attackInterval;

	bool left = false;

	CharacterSystem character;

	CookieMuti(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class CookieKaban :public Entity
{
public:

	static constexpr double attackInterval = 0.4;
	double attackTimer = 0;

	bool left = false;

	CharacterSystem character;

	CookieKaban(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class DropCorn :public Entity
{
public:

	bool left = false;

	bool startShake = false;

	double velX;

	bool corn = true;

	//爆発した後、地面に触れたか。
	bool touch = false;

	//攻撃されたか
	bool damaged = false;

	CharacterSystem character;

	DropCorn(const Vec2& cpos, double velX);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class Zerosen :public Entity
{
public:

	CharacterSystem character;

	double accumulatedTime = 0;

	Zerosen(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;
};

class Needle :public Entity
{
public:

	int32 angle = 0;

	Needle(const Vec2& cpos,int32 angle);

	void update()override;

	void draw()const override;

	void damage(int32 n, const Vec2& force = {}, DamageType damageType = DamageType::Brakable);
};

class BigCloudEnemy :public Entity
{
public:

	CharacterSystem character;

	double accumulatedTime = 0;

	double startY = 0;

	//double time = 0;

	BigCloudEnemy(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	void damage(int32, const Vec2&, DamageType)override;

	bool isActive()override;
};
