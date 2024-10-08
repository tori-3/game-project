﻿#pragma once
#include"Entity.h"

class CookieItem :public Entity {
public:

	CookieItem(const Vec2& cpos) :Entity{ U"Item", Circle{0,0,25},cpos,{0,0},1} {

		if (not TextureAsset::IsRegistered(U"CookieItem")) {
			TextureAsset::Register(U"CookieItem", U"🍪"_emoji);
		}

		AudioAsset{ U"ポップな死亡" }.playOneShot();
	}

	void update()override {
		manager->stage->hit(&hitBox);
		hitBox.physicsUpdate();
		hitBox.update();
	}

	void draw()const override {
		TextureAsset(U"CookieItem").resized(50).drawAt(pos.x,pos.y- Periodic::Sine0_1(2s,DataManager::get().time)*20);
	}
};
