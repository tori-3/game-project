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

	void update()override
	{
		if (entityList.size() < 5)
		{
			if (not RectF{ Arg::center(pos),rect_size * 2,9999 }.intersects(DataManager::get().playerPos))
			{
				for (accumlater += Scene::DeltaTime(); spawn <= accumlater; accumlater -= spawn)
				{
					Entity* child = new StrawberrySoldier{ pos };
					entityList << child;
					manager->add(new Spawner{ pos,child });
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
