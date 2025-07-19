#pragma once
#include"Common.h"
#include"Entity.h"
#include"Player.h"
#include"CharacterSystem.h"

class Fairy :public Entity
{
public:

	bool left = true;

	CharacterSystem character;

	Vec2 velocity{};

	Fairy(const Vec2& cpos={ -100,400 })
		: Entity{ U"Fairy", RectF{Arg::center(0,0),70,69},cpos,{0,0},1 }
		, character{ U"Characters/yousei1/yousei.json" ,U"Characters/yousei1/motion.txt" ,0.25,cpos,true,false }
	{
		character.addMotion(U"Floating", true);

		DataManager::get().table.emplace(U"SignboardInvisible");

		z = 110;
	}

	void update()override
	{
		if(DataManager::get().fairyPos)
		{
			pos = Math::SmoothDamp(pos, DataManager::get().fairyPos.value(), velocity, 0.2, 800);
		}
		else
		{
			pos = Math::SmoothDamp(pos, DataManager::get().playerPos + Vec2{ -50,-30 }, velocity, 0.5, 500);
		}

		character.update(pos, DataManager::get().playerPos.x<pos.x);
		DataManager::get().fairyPos = none;
	}

	void lateUpdate()override
	{
		

	}

	void draw()const override
	{
		character.draw();
	}
};
