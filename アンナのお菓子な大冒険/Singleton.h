#pragma once

//シングルトンのマクロ
//参考：https://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
#define Singleton(name)\
public:\
	name(const name&) = delete;\
	name& operator=(const name&) = delete;\
	name(name&&) = delete;\
	name& operator=(name&&) = delete;\
	static name& get() { return *instance; }\
	static void create() { if (!instance)instance = new name; }\
	static void destroy() { delete instance; instance = nullptr; }\
private:\
	name() = default;\
	~name() = default;\
	static name* instance;

