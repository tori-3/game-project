#pragma once
#include"Enemy.h"

class Spawner:public Entity
{
public:

	Entity* entity;

	double t = 0;

	Spawner(const Vec2& pos, Entity* entity)
		: Entity{ U"Spawner", RectF{Arg::center(0,0),0,0},pos,{0,0},3 }
		, entity{entity}
	{}

	void update()override
	{
		t += Scene::DeltaTime();
		if (1.0 <= t)
		{
			hp = 0;
			manager->add(entity);
		}
	}

	void draw()const override
	{
		const Transformer2D transformer{ Mat3x2::Translate(0, 70*(1-t)) };
		entity->draw();
	}
};

class ItigoSpawnerEntity :public Entity
{
public:

	static constexpr double spawn = 1.0;

	double accumlater = 0;

	Array<Entity*>entityList;

	ItigoSpawnerEntity(const Vec2& pos)
		: Entity{ U"Spawner", RectF{Arg::center(0,0),0,0},pos,{0,0},3 }
	{}

	int32 spawnCount = 0;

	static constexpr double Interval = 2.0;
	double intervalTimer = 0;

	bool canSpown = true;

	void update()override
	{
		if(not canSpown)
		{
			if (entityList.size() == 0)
			{
				intervalTimer += Scene::DeltaTime();
			}

			if (Interval <= intervalTimer)
			{
				canSpown = true;
				spawnCount = 0;
			}
		}
		else if (not RectF{ Arg::center(pos),rect_size * 2,9999 }.intersects(DataManager::get().playerPos))
		{
			for (accumlater += Scene::DeltaTime(); spawn <= accumlater; accumlater -= spawn)
			{
				Entity* child = new StrawberrySoldier{ pos };
				entityList << child;
				manager->add(new Spawner{ pos,child });

				++spawnCount;
				if (spawnCount == 5)
				{
					canSpown = false;
					intervalTimer = 0;
				}
			}
		}


	}

	void lateUpdate()override
	{
		entityList.remove_if([](Entity* entity) {return not entity->isActive();});
	}

	void draw()const override {}

};
