#pragma once
#include"Enemy.h"
#include"Boss.h"
#include"LastBoss.h"
#include"Spawner.h"

class EnemyAdder
{
public:

	EnemyAdder();

	HashTable<String, std::function<Entity* (const Vec2&)>>table;

	void update(EntityManager& manager);
};
