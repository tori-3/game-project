#pragma once
#include"Figure.hpp"

class Attack {
public:

	int32 damage;

	Attack(int32 damage):damage{damage}{}

	virtual void update() = 0;

	virtual void draw()const = 0;

	virtual bool isActive() = 0;

	int32 getDamage() {
		return damage;
	}

	virtual bool intersects(const Figure& figure) = 0;
};
