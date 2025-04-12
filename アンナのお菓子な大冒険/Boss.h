#pragma once
#include"Common.h"
#include"Entity.h"
#include"Player.h"
#include"MotionLoader.h"
#include"CookieItem.h"
#include"CharacterSystem.h"

class SnowKnight :public Entity
{
public:

	int32 attackCount = 0;

	static constexpr int32 maxHp = 3;

	CharacterSystem character;

	//右と左の定位置
	double rx, lx;
	double centerX = 0;

	std::function<void()>updateFunc;
	std::function<void()>endFunc;

	bool left = false;
	double timer;

	Array<Entity*>summonList;//召喚したリスト

	SnowKnight(const Vec2& cpos);

	Timer attackTimer{ 0.5s };

	double accumulatedTime = 0;

	//鎧の耐久回数
	int32 yoroi = 1;

	Vec2 kenboxPos{ 0,0 };
	Vec2 kenboxVel{ 0,0 };
	int32 kenboxHp{ 10 };

	Vec2 force{};

	//剣を振ったか
	bool kenFlg = false;

	void update()override;

	void lateUpdate() override;

	void damage(int32 n, const Vec2& _force, DamageType damageType) override;

	void draw()const override;
};

class SlaversCookie :public Entity
{
public:

	static constexpr int32 maxHp = 3;

	Array<Entity*>summonListItigo;//召喚したリスト
	Entity* summonSnowLeft = nullptr;
	Entity* summonSnowRight = nullptr;

	bool left = true;

	std::function<void()> updateFunc;
	std::function<void()>endFunc;

	double timer;

	CharacterSystem character;

	size_t type = 1;

	double rx, lx;

	//ダメージを既に受けたか
	bool damaged = false;

	SlaversCookie(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	void damage(int32 n, const Vec2& _force, DamageType damageType) override;

	~SlaversCookie();
};

class Captain :public Entity
{
public:

	static constexpr int32 maxHp = 30;

	double accumulatedTime = 0;

	Array<Entity*>summonList;//召喚したリスト

	bool left = true;

	double center;

	Vec2 rPos, lPos;

	std::function<void()> updateFunc;
	std::function<void()>endFunc;

	double timer;

	CharacterSystem character;

	size_t type = 1;

	Captain(const Vec2& cpos);

	void update()override;

	void lateUpdate()override;

	void draw()const override;

	void damage(int32 n, const Vec2& force,DamageType damageType)override;
};
