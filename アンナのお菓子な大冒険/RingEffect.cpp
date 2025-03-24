#include"RingEffect.h"

bool RingEffect::update(double t)
{
	const double e = EaseOutExpo(t);

	Circle{ m_pos, (e * 100) }.drawFrame((20.0 * (1.0 - e)), m_color);

	return (t < 1.0);
}
