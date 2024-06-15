#pragma once

struct MagicEffect : IEffect
{
	Vec2 m_pos;

	Texture m_texture;

	ColorF m_color;

	double m_rad = Random(0_deg, 360_deg);

	double m_angle;

	double m_size;

	double m_speed;

	explicit MagicEffect(const Vec2& pos, const Texture& texture, const ColorF& color, double angle = 180_deg, double speed = 100)
		: m_size{ Random(30.0, 50.0) }
		, m_pos{ pos }
		, m_texture{ texture }
		, m_color{ color }
		, m_angle{ angle }
		, m_speed{ speed } {}

	bool update(double t) override
	{
		constexpr double timeLim = 1.0;

		m_pos = OffsetCircular{ m_pos, Scene::DeltaTime() * m_speed,m_angle };
		m_rad += Scene::DeltaTime() * 90_deg;
		m_texture.resized(m_size).rotated(m_rad).drawAt(m_pos, ColorF{ m_color, ((timeLim - t) / timeLim) });

		return (t < timeLim);
	}
};
