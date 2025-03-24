#include"FadeUpEffect.h"

bool FadeUpEffect::update(double t)
{
	const ColorF color{ m_color, 1.0 - (t * 2.0) };

	m_font(m_text).drawAt(40, m_start.movedBy(0, t * -120), color);

	return (t < 0.5);
}
