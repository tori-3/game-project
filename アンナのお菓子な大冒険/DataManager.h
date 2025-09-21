#pragma once
#include"TalkWindow.h"

class DataManager
{
public:
	Effect effect, additiveEffect;
	HashSet<String> table;
	double time = 0;
	Array<std::pair<String, Vec2>>list;
	Vec2 playerPos{};

	bool playerAlive = true;

	double tame = 0;

	Optional<Vec2> fairyPos;

	int32 maxHP = 5;

	//noneのときはプレイヤーに追従
	Optional<Vec2>cameraPos{};

	SizeF stageSize{};

	Optional<double> bossHPRate = none;

	String bossName;

	void addEntity(const String& name, const Vec2& pos)
	{
		list << std::pair<String, Vec2>{name,pos};
	}

	//シングルトン
	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;
	DataManager(DataManager&&) = delete;
	DataManager& operator=(DataManager&&) = delete;
	static DataManager& get() { return *instance; }
	static void create() { if (!instance)instance = new DataManager; }
	static void destroy() { delete instance; instance = nullptr; }
private:
	DataManager() = default;
	~DataManager() = default;
	static DataManager* instance;
};

class DataManagerStart
{
public:
	DataManagerStart()
	{
		DataManager::create();
	}

	~DataManagerStart()
	{
		DataManager::destroy();
	}
};
