#pragma once
#include"DataManager.h"

struct LaserEffect : IEffect
{
	RectF rect;

	ColorF color;

	double rad;

	explicit LaserEffect(const RectF& rect, double rad, const ColorF& color)
		: rect{ rect }
		, color{ color }
		, rad{ rad } {
		DataManager::get().laser = true;
	}

	~LaserEffect()
	{
		DataManager::get().laser = false;
	}

	bool update(double t) override;
};
