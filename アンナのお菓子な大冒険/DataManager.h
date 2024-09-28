#pragma once
#include"Singleton.h"
#include"TalkWindow.h"

class DataManager {
	Singleton(DataManager);
public:
	Effect effect, additiveEffect;
	HashSet<String> table;
	double time = 0;
	Array<std::pair<String, Vec2>>list;
	Vec2 playerPos{};
	//TalkWindow talkWindow;

	bool playerAlive = true;

	double tame = 0;

	//noneのときはプレイヤーに追従
	Optional<Vec2>cameraPos{};

	SizeF stageSize{};

	Optional<double> bossHPRate = none;

	void addEntity(const String& name, const Vec2& pos) {
		list << std::pair<String, Vec2>{name,pos};
	}
};


//エフェクトなどのシングルトンは別に作った方がよい?


class DataManagerStart {
public:
	DataManagerStart() {
		DataManager::create();
	}

	~DataManagerStart() {
		DataManager::destroy();
	}

};
