#pragma once

struct FadeUpEffect : IEffect
{
	Vec2 m_start;

	String m_text;

	Font m_font;

	ColorF m_color;

	FadeUpEffect(const Vec2& start, String text, const Font& font, ColorF color=Palette::Red)
		: m_start{ start }
		, m_text{ text }
		, m_font{ font }
		,m_color{color}{}

	bool update(double t) override
	{
		const ColorF color{ m_color, 1.0 - (t * 2.0) };

		m_font(m_text).drawAt(40,m_start.movedBy(0, t * -120), color);

		return (t < 0.5);
	}
};
