﻿#pragma once

struct ActionInfo
{
	std::function<bool()>startCondition = []{return false; };
	std::function<void()>start = [] {};
	std::function<bool(double)>update = [](double) {return true; };
	std::function<void()>end = [] {};
};

class SimpleAction
{
public:
	ActionInfo info;

	double time=0;

	bool active = false;

	bool isActive()const
	{
		return active;
	}

	SimpleAction() = default;

	SimpleAction(const ActionInfo& info):info{info}{}

	void start()
	{
		active = true;
		time = 0;
		info.start();
	}

	bool conditonStart()
	{
		if (info.startCondition())
		{
			start();
			return true;
		}
		else
		{
			return false;
		}
	}

	void cancel()
	{
		if (active)
		{
			active = false;
			info.end();
		}
	}

	void update()
	{
		if (active) {
			time += Scene::DeltaTime();
			if (not info.update(time))
			{
				active = false;
				info.end();
			}
		}
		else {
			if (info.startCondition()) {
				active = true;
				time = 0;
				info.start();
			}
		}
	}
};

class ActionManager
{
public:

	HashTable<String,SimpleAction>table;

	ActionManager() {}

	void cancelAll(HashSet<String> without={})
	{
		for (auto&& [key, value] : table)
		{
			if (not without.contains(key))
			{
				value.cancel();
			}
		}
	}

	void cancel(StringView name)
	{
		table[name].cancel();
	}

	void add(StringView name,const ActionInfo& info)
	{
		table[name] = SimpleAction{ info };
	}

	void update()
	{
		for (auto&& [key, value] : table)
		{
			value.update();
		}
	}

	bool conditonStart(StringView name)
	{
		return table[name].conditonStart();
	}

	void start(StringView name)
	{
		table[name].start();
	}

	void debugPrint()
	{
		for (auto&& [key, value] : table)
		{
			Print << key<<U":" << value.isActive();
		}
	}

	template<class... Arg>
	bool hasActive(Arg&& ...arg)const
	{
		for (const auto& name : std::initializer_list<String>{ arg... })
		{
			if (table.contains(name) and table.at(name).isActive())
			{
				return true;
			}
		}
		return false;
	}

	SimpleAction get(StringView name)const
	{
		return table.at(name);
	}
};
