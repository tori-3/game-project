#pragma once

#include"MotionLoader.h"

class CharacterSystem
{
public:

	Character character;

	CharacterSystem(const String&jsonPath, const String& motionPath,double scale,const Vec2& pos,bool left,bool mirror=false)
		: character{ JSON::Load(jsonPath),scale }
		, loader{ CSV{motionPath} }
		, tmpPos{pos}
		, tmpLeft{left}
	{
		character.setDrawManager(&drawManager);

		character.joint->pos = {0,0};
		character.base->joint->pos = {0,0};

		character.pos = pos;

		if(mirror)character.angle = 180_deg;
		character.update(0);

		//シーン遷移によってdrawが先に呼ばれることがあるため。
		drawManager.update();
	}



	int32 mirrorCount = 0;
	void update(const Vec2& pos,bool left)
	{
		if (left != tmpLeft)
		{
			if(not character.hasMotion(U"Mirror"))
			{
				Motion m;
				m.add(new Mirror{ 0,720_deg });
				character.addMotion(U"Mirror", m);

				tmpLeft = left;
			}
		}

		character.pos += pos - tmpPos;

		loader.param.update();

		character.update();
		drawManager.update();

		tmpPos = pos;

	}

	void draw()const
	{
		drawManager.draw();
	}

	void clearMotion()
	{
		for(auto it=character.motionTable.begin();it!=character.motionTable.end();)
		{
			if(it->first== U"Mirror")
			{
				++it;
			}
			else
			{
				it = character.motionTable.erase(it);
			}
		}
	}

	void addMotion(const String& motionName,bool loop=false)
	{
		if (not table.contains(motionName))
		{
			table[motionName] = loader.LoadMotion(motionName);
		}

		table[motionName].loop = loop;
		character.addMotion(motionName,table[motionName]);
	}

	void removeMotion(const String& motionName)
	{
		character.removeMotion(motionName);
	}

	bool hasMotion(const String& motionName)
	{
		return character.hasMotion(motionName);
	}

private:
	HashTable<String, Motion>table;

	DrawManager drawManager;

	MotionLoader loader;

	Vec2 tmpPos;

	bool tmpLeft;
};
