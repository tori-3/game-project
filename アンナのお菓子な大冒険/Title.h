#pragma once

#include"Common.h"

class Title : public App::Scene
{
public:

	Title(const InitData& init)
		: IScene{ init }
	{

	}

	void update() override
	{

	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });





	}
};



