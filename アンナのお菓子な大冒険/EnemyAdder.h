#pragma once
#include"Entity.h"

class EnemyAdder
{
public:

	EnemyAdder();

	HashTable<String, std::function<Entity* (const Vec2&)>>table;

	void update(EntityManager& manager);
};
