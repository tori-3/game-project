#pragma once
#include"Enemy.h"

class Spawner:public Entity {
public:

	Entity* entity;

	double t = 0;

	Spawner(const Vec2& pos, Entity* entity)
		: Entity{ U"Spawner", RectF{Arg::center(0,0),0,0},pos,{0,0},3 },entity{entity}
	{

	}

	void update()override {
		t += Scene::DeltaTime();
		if (1.0 <= t) {
			hp = 0;
			manager->add(entity);
		}
	}

	void draw()const override {
		const Transformer2D transformer{ Mat3x2::Translate(0, 70*(1-t)) };
		entity->draw();
	}
};
