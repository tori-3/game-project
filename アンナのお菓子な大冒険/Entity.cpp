#include"Entity.h"


void Entity::lateUpdate() {}

void Entity::damage(int32 n, const Vec2&, DamageType damageType)
{
	hp -= n;
}

bool Entity::isActive()
{

	if (hitBox.touch(Direction::down) && hitBox.touch(Direction::up)) {
		hp = 0;
	}
	if (hitBox.touch(Direction::left) && hitBox.touch(Direction::right)) {
		hp = 0;
	}

	return 0 < hp && pos.y < 2000 and alive;
}

Array<Entity*> Entity::attack(StringView target, const Figure& figure, int32 damage, double power, DamageType damageType, int32 kaisuu)
{
	Array<Entity*>list;
	for (auto& entity : manager->getArray(target)) {
		if (entity->hitBox.getFigure().intersects(figure)) {

			if (entity->pos.x < pos.x) {
				entity->damage(damage, { -power,-200 }, damageType);
			}
			else {
				entity->damage(damage, { power,-200 }, damageType);
			}

			list << entity;
			if (kaisuu == 0) {
				break;
			}
			else {
				kaisuu -= 1;
			}
		}
	}
	return list;
}

Array<Entity*> Entity::attackDamaged(StringView target, const Figure& figure, double damage, double power, int32 kaisuu)
{
	Array<Entity*>list;
	for (auto& entity : manager->getArray(target))
	{
		if (entity->hitBox.getFigure().intersects(figure))
		{

			int32 tmpHp = entity->hp;

			if (entity->pos.x < pos.x)
			{
				entity->damage(damage, { -power,-200 });
			}
			else
			{
				entity->damage(damage, { power,-200 });
			}

			if (entity->hp < tmpHp)
			{
				list << entity;
			}

			if (kaisuu == 0)
			{
				break;
			}
			else
			{
				kaisuu -= 1;
			}
		}
	}
	return list;
}

void EntityManager::update()
{
	for (auto i : step_backward(entitys.size()))
	{
		entitys[i]->update();
	}

	for (auto i : step_backward(entitys.size()))
	{
		entitys[i]->lateUpdate();
	}

	entitys.remove_if([&](const PEntity& entity)
	{
	if (not entity->isActive())
	{
		table[entity->tag].remove(entity.get());
		return true;
	}
	return false;
	});//Activeで無いものを削除

	entitys.stable_sort_by([](const PEntity& a, const PEntity& b) {return *a < *b; });//z座標でソート
}
