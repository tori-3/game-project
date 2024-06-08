#pragma once

struct MagicEffect : IEffect
{
	Vec2 m_pos;

	Texture m_texture;

	ColorF m_color;

	double m_rad = Random(0_deg, 360_deg);

	double m_size;

	explicit MagicEffect(const Vec2& pos, const Texture& texture, const ColorF& color)
		: m_size{ Random(30.0, 50.0) }
		, m_pos{ pos}
		, m_texture{ texture }
		, m_color{ color } {}

	bool update(double t) override
	{

		constexpr double timeLim = 1.0;

		m_pos.y += Scene::DeltaTime() * 100;
		m_rad += Scene::DeltaTime() * 90_deg;
		m_texture.resized(m_size).rotated(m_rad).drawAt(m_pos, ColorF{ m_color, ((timeLim - t) / timeLim) });

		return (t < timeLim);
	}
};
