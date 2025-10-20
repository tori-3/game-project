#pragma once
#include"Entity.h"

class CookieItem :public Entity {
public:

	CookieItem(const Vec2& cpos) :Entity{ U"Item", Circle{0,0,25},cpos,{0,0},1} {

		if (not TextureAsset::IsRegistered(U"CookieItem")) {
			TextureAsset::Register(U"CookieItem", U"🍪"_emoji);
		}

		if(Abs(pos.x-DataManager::get().playerPos.x)<=rect_size*20)
		{
			AudioAsset{ U"ポップな死亡" }.playOneShot();
		}
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

class FloatingCookieItem :public Entity
{
public:

	FloatingCookieItem(const Vec2& cpos) :Entity{ U"Item", Circle{0,0,25},cpos,{0,0},1 } {

		if (not TextureAsset::IsRegistered(U"FloatingCookieItem")) {
			TextureAsset::Register(U"FloatingCookieItem", U"🥠"_emoji);
		}
	}

	void update()override {
		//manager->stage->hit(&hitBox);
		//hitBox.physicsUpdate();
		//hitBox.update();
	}

	void draw()const override {
		TextureAsset(U"FloatingCookieItem").resized(50).drawAt(pos.x, pos.y - Periodic::Sine0_1(2s, DataManager::get().time) * 20);
	}
};

