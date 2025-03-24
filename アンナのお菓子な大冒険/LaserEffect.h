#pragma once

struct LaserEffect : IEffect
{
	RectF rect;

	ColorF color;

	double rad;

	explicit LaserEffect(const RectF& rect, double rad, const ColorF& color)
		: rect{ rect }
		, color{ color }
		, rad{ rad } {}

	bool update(double t) override;
};
