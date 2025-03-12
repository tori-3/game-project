#pragma once
#include"Common.h"
#include"Entity.h"
#include"Player.h"
#include"CharacterSystem.h"

class Fairy :public Entity {
public:

	bool left = true;

	CharacterSystem character;

	Fairy(const Vec2& cpos) :Entity{ U"Fairy", RectF{Arg::center(0,0),70,69},cpos,{0,0},1 }
		, character{ U"Characters/yousei1/yousei.json" ,U"Characters/yousei1/motion.txt" ,0.3,cpos,true,false }
	{
		character.addMotion(U"Floating", true);


	}

	void update()override
	{
		character.update(pos, left);
	}

	void lateUpdate()override
	{
		

	}

	void draw()const override
	{
		character.draw();
	}
};
