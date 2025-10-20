#include"EnemyAdder.h"
#include"Enemy.h"
#include"Boss.h"
#include"LastBoss.h"
#include"Spawner.h"
#include"Gimmick.h"

EnemyAdder::EnemyAdder()
{
	table[U"StrawberrySoldier"] = [](const Vec2& pos) {return new StrawberrySoldier{ pos }; };
	table[U"BigStrawberrySoldier"] = [](const Vec2& pos) {return new StrawberrySoldier{ pos,Big{} }; };
	table[U"CookieSoldier"] = [](const Vec2& pos) {return new CookieSoldier{ pos }; };
	table[U"BigCookieSoldier"] = [](const Vec2& pos) {return new CookieSoldier{ pos,Big{} }; };
	table[U"SpawnerStrawberrySoldier"] = [](const Vec2& pos) {return new ItigoSpawnerEntity{ pos }; };
	table[U"Player"] = [](const Vec2& pos) {return new Player{ pos }; };
	table[U"Snowman"] = [](const Vec2& pos) {return new Snowman{ pos }; };
	table[U"ItigoSlave"] = [](const Vec2& pos) {return new ItigoSlave{ pos }; };
	table[U"BigItigoSlave"] = [](const Vec2& pos) {return new ItigoSlave{ pos,Big{} }; };
	table[U"CloudEnemy"] = [](const Vec2& pos) {return new CloudEnemy{ pos }; };
	table[U"Corn"] = [](const Vec2& pos) {return new Corn{ pos }; };
	table[U"CookieMuti"] = [](const Vec2& pos) {return new CookieMuti{ pos }; };
	table[U"CookieKaban"] = [](const Vec2& pos) {return new CookieKaban{ pos }; };
	table[U"SnowKnight"] = [](const Vec2& pos) {return new SnowKnight{ pos }; };
	table[U"Zerosen"] = [](const Vec2& pos) {return new Zerosen{ pos }; };
	table[U"SlaversCookie"] = [](const Vec2& pos) {return new SlaversCookie{ pos }; };
	table[U"Captain"] = [](const Vec2& pos) {return new Captain{ pos }; };
	table[U"Door"] = [](const Vec2& pos) {return new GoalDoor{ pos }; };
	table[U"LastBoss"] = [](const Vec2& pos) {return new LastBoss{ pos }; };
	table[U"FallingRocks"] = [](const Vec2& pos) {return new FallingRocks{ pos }; };
	table[U"RollingRocks"] = [](const Vec2& pos) {return new RollingRocks{ pos }; };
	table[U"NeedleUp"] = [](const Vec2& pos) {return new Needle{ pos,2 }; };
	table[U"NeedleDown"] = [](const Vec2& pos) {return new Needle{ pos,0 }; };
	table[U"NeedleLeft"] = [](const Vec2& pos) {return new Needle{ pos,1 }; };
	table[U"NeedleRight"] = [](const Vec2& pos) {return new Needle{ pos,3 }; };
	table[U"BigCloudEnemy"] = [](const Vec2& pos) {return new BigCloudEnemy{ pos }; };
	table[U"FloatingCookieItem"]=[](const Vec2& pos) {return new FloatingCookieItem{ pos }; };
}

void EnemyAdder::update(EntityManager& manager)
{
	for (auto& a : DataManager::get().list)
	{
		manager.add(table[a.first](a.second));
	}
	DataManager::get().list.clear();
}
