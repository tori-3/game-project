#pragma once

#include"MotionLoader.h"

class CharacterSystem
{
public:

	HashTable<String, Motion>table;

	Character character;

	DrawManager drawManager;

	MotionLoader loader;

	Vec2 tmpPos;

	bool tmpLeft;

	CharacterSystem(const String&jsonPath, const String& motionPath,double scale,const Vec2& pos,bool left,bool mirror=false)
		:character{ JSON::Load(jsonPath),scale }, loader{ CSV{motionPath} },tmpPos{pos},tmpLeft{left}
	{
		character.setDrawManager(&drawManager);
		character.joint->pos = pos;
		character.base->joint->pos = pos;

		if(mirror)character.angle = 180_deg;
		character.update(0);

		//シーン遷移によってdrawが先に呼ばれることがあるため。
		drawManager.update();
	}

	
	int32 mirrorCount = 0;
	void update(const Vec2& pos,bool left) {

		if (left != tmpLeft) {
			Motion m;
			m.add(new Mirror{ 0,720_deg });
			character.addMotion(U"Mirror{}"_fmt(mirrorCount), m);
			tmpLeft = left;
			mirrorCount++;
		}

		character.joint->pos += pos - tmpPos;
		character.update();

		character.base->joint->pos += pos - tmpPos;

		tmpPos = pos;

		drawManager.update();
	}

	void draw()const {
		drawManager.draw();
	}

	void clearMotion() {
		character.clearMotion();
	}

	void addMotion(const String& motionName,bool loop=false) {

		if (not table.contains(motionName)) {
			table[motionName] = loader.LoadMotion(motionName);
		}

		table[motionName].loop = loop;
		character.addMotion(motionName,table[motionName]);

		//うーん
		character.joint->pos = tmpPos;
	}

	void removeMotion(const String& motionName) {
		character.removeMotion(motionName);
	}

	bool hasMotion(const String& motionName) {
		return character.hasMotion(motionName);
	}

private:

};
