#pragma once
#include"Figure.hpp"
#include"HitBox.h"
#include"DataManager.h"
#include"Stage.h"

class EntityManager;

enum class DamageType { Brakable, UnBrakable };

class Entity
{
public:
	EntityManager* manager = nullptr;

	String tag;

	Vec2 pos, vel;
	int32 hp;
	HitBox hitBox;
	bool alive = true;

	Entity(const String& tag,const Figure& figure, const Vec2& _pos, const Vec2& _vel,int32 _hp) :tag{ tag },pos{_pos}, vel{_vel}, hp{_hp}, hitBox{&pos, &vel, figure, &hp} {}

	virtual ~Entity() = default;

	virtual void update() = 0;

	virtual void lateUpdate();

	virtual void draw()const = 0;

	virtual void damage(int32 n, const Vec2& force = {}, DamageType damageType = DamageType::Brakable);

	void kill()
	{
		alive = false;
	}

	virtual bool isActive();

	void setManager(EntityManager* _manager)
	{
		manager = _manager;
	}

	EntityManager* getManager()
	{
		return manager;
	}

	bool operator<(const Entity& right) const
	{
		return z < right.z;
	}

	double z = 0;

	Array<Entity*> attack(StringView target, const Figure& figure, int32 damage, double power=200, DamageType damageType=DamageType::Brakable, int32 kaisuu = -1);

	Array<Entity*> attack(StringView target, const Figure& figure, int32 damage, DamageType damageType, int32 kaisuu = -1)
	{
		return attack(target,figure,damage,200,damageType,kaisuu);
	}

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

	void update();

	void draw()const
	{
		entitys.each([](const PEntity& entity) {entity->draw(); });
	}


	Entity* get(StringView tag)
	{
		return table[tag][0];
	}

	const Entity* get(StringView tag)const
	{
		return table.at(tag)[0];
	}

	const Array<Entity*>& getArray(StringView tag)
	{
		return table[tag];
	}

	void add(Entity* entity)
	{
		entitys << std::unique_ptr<Entity>(entity);
		entitys.back()->setManager(this);
		table[entity->tag] << entity;
	}

};
