#pragma once
#include"Attack.h"

class AttackManager {
public:

	Array<Attack*>list;

	template<class T, class... A> void add(A... args) {
		list << std::make_unique<T>(args...);
	}

	int32 check(const Figure& figure) {
		int32 damage = 0;
		list.each([&](Attack* attack) {if (attack->intersects(figure))damage += attack->getDamage(); });
		return damage;
	}

	void update() {
		list.each([](Attack* attack) {attack->update(); });
		auto a=list.removed_if([](Attack* attack) {return not attack->isActive(); });
	}

	void draw()const {
		list.each([](Attack* attack) {attack->draw(); });
	}
};
