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

	virtual void draw()const = 0;

	virtual void damage(int32 n,const Vec2& force={}) {
		hp -= n;
	}

	virtual bool isActive() {
		return 0<hp&& pos.y<1000;
	}

	virtual void end() {

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

		entitys.remove_if([&](const PEntity& entity) {
			if (not entity->isActive()) {
				entity->end();
				table[entity->tag].remove(entity.get());
				return true;
			}
		return false;
		});//Activeで無いものを削除

		//for (auto it = entitys.begin(); it != entitys.end();)
		//{
		//	// 円がクリックされたらその地点を表す要素を削除
		//	if (not (*it)->isActive())
		//	{
		//		(*it)->end();
		//		table[(*it)->tag].remove(it->get());
		//		it = entitys.erase(it);
		//	}
		//	else
		//	{
		//		++it;
		//	}
		//}

		entitys.stable_sort_by([](const PEntity& a, const PEntity& b) {return *a < *b; });//z座標でソート
	}

	void draw()const {
		entitys.each([](const PEntity& entity) {entity->draw(); });
	}


	Entity* get(const String& tag) {
		return table[tag][0];
	}

	const Array<Entity*>& getArray(const String& tag) {
		return table[tag];
	}

	void add(Entity* entity) {
		entitys << std::unique_ptr<Entity>(entity);
		entitys.back()->setManager(this);
		table[entity->tag] << entity;
	}

};
