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

	bool update(double t) override;
};
