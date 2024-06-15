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

	bool update(double t) override
	{
		constexpr double time = 5;

		double size = rect.h;
		if (t < 0.5)size *= Periodic::Jump0_1(1s, t);
		else if (time - 0.5 < t)size *= Periodic::Jump0_1(1s, time - t);

		size += rect.h * Min(Fmod(t, 0.5) / 3, Fmod(-t, 0.5) / 4);

		const Transformer2D transformer{ Mat3x2::Rotate(rad, rect.leftCenter()) };

		const RectF center{ Arg::center(rect.center()),rect.w,size };

		Circle{ rect.leftCenter(),size / 2 }.draw().drawArc(180_deg, 180_deg, 0, size, color, ColorF{ color,0 });
		center.movedBy(0, -size).draw(Arg::top = ColorF{ color,0 }, Arg::bottom = color);
		center.draw(Palette::White);
		center.movedBy(0, +size).draw(Arg::top = color, Arg::bottom = ColorF{ color,0 });

		return (t < time);
	}
};
