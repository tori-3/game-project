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
	CharacterSystem character;

	//右と左の定位置
	double rx, lx;
	double centerX = 0;

	std::function<void()> f;
	std::function<void()>f2;

	bool left = false;
	double timer;

	HitBox* kenHitbox;

	Array<Entity*>summonList;//召喚したリスト

	SnowKnight(const Vec2& cpos);

	Timer attackTimer{ 0.5s };

	double accumulatedTime = 0;

	int32 yoroi = 1;

	Vec2 kenboxPos{ 0,0 };
	Vec2 kenboxVel{ 0,0 };
	int32 kenboxHp{ 10 };

	Vec2 force{};

	//剣を振ったか
	bool kenFlg = false;

	void update()override;

	void lateUpdate() override;

	void damage(int32 n, const Vec2& _force = {}) override;

	void draw()const override;
};
