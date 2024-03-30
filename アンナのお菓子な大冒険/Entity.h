#pragma once
#include"Figure.hpp"
#include"HitBox.h"
#include"DataManager.h"
#include"Stage.h"

class EntityManager;

class Entity {
public:
	EntityManager* manager = nullptr;

	String tag;

	Vec2 pos, vel;
	int32 hp;
	HitBox hitBox;

	Entity(const String& tag,const Figure& figure, const Vec2& _pos, const Vec2& _vel,int32 _hp) :tag{ tag },pos{_pos}, vel{_vel}, hp{_hp}, hitBox{&pos, &vel, figure, &hp} {}

	virtual void update() = 0;

	virtual void lateUpdate(){}

	virtual void draw()const = 0;

	virtual void damage(int32 n,const Vec2& force={}) {
		hp -= n;
	}

	bool alive = true;
	void kill() {
		alive = false;
	}

	virtual bool isActive() {

		if (hitBox.touch(Direction::down) && hitBox.touch(Direction::up)) {
			hp = 0;
		}
		if (hitBox.touch(Direction::left) && hitBox.touch(Direction::right)) {
			hp = 0;
		}

		return 0<hp&& pos.y<2000 and alive;
	}

	void setManager(EntityManager* _manager) {
		manager = _manager;
	}

	EntityManager* getManager() {
		return manager;
	}

	bool operator<(const Entity& right) const {
		return z < right.z;
	}

	double z = 0;

	Array<Entity*> attack(StringView target, const Figure& figure, double damage, double power=200, int32 kaisuu = -1);

	//ダメージを受けたEntityのみを返す
	Array<Entity*> attackDamaged(StringView target, const Figure& figure, double damage, double power = 200, int32 kaisuu = -1);

private:


protected:
};

class EntityManager {
public:

	using PEntity = std::unique_ptr<Entity>;

	Array<PEntity>entitys;

	HashTable<String, Array<Entity*>>table;

	Stage* stage = nullptr;

	void update() {
		for (auto i : step(entitys.size())) {
			entitys[i]->update();
		}

		for (auto i : step(entitys.size())) {
			entitys[i]->lateUpdate();
		}

		entitys.remove_if([&](const PEntity& entity) {
			if (not entity->isActive()) {
				table[entity->tag].remove(entity.get());
				return true;
			}
		return false;
		});//Activeで無いものを削除

		entitys.stable_sort_by([](const PEntity& a, const PEntity& b) {return *a < *b; });//z座標でソート
	}

	void draw()const {
		entitys.each([](const PEntity& entity) {entity->draw(); });
	}


	Entity* get(StringView tag) {
		return table[tag][0];
	}

	const Array<Entity*>& getArray(StringView tag) {
		return table[tag];
	}

	void add(Entity* entity) {
		entitys << std::unique_ptr<Entity>(entity);
		entitys.back()->setManager(this);
		table[entity->tag] << entity;
	}

};

Array<Entity*> Entity::attack(StringView target, const Figure& figure, double damage, double power, int32 kaisuu) {

	Array<Entity*>list;
	for (auto& entity : manager->getArray(target)) {
		if (entity->hitBox.getFigure().intersects(figure)) {

			if (entity->pos.x < pos.x) {
				entity->damage(damage, {-power,-200});
			}
			else {
				entity->damage(damage, { power,-200 });
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

Array<Entity*> Entity::attackDamaged(StringView target, const Figure& figure, double damage, double power, int32 kaisuu) {

	Array<Entity*>list;
	for (auto& entity : manager->getArray(target)) {
		if (entity->hitBox.getFigure().intersects(figure)) {

			int32 tmpHp = entity->hp;

			if (entity->pos.x < pos.x) {
				entity->damage(damage, { -power,-200 });
			}
			else {
				entity->damage(damage, { power,-200 });
			}

			if (entity->hp < tmpHp) {
				list << entity;
			}

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
