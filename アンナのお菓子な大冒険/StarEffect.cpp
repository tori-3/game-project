#include"StarEffect.h"

bool StarEffect::update(double t)
{
	t /= 0.4;

	for (auto& star : m_stars)
	{
		const Vec2 pos = star.start
			+ star.velocity * t + 0.5 * t * t * Gravity;

		const double angle = (pos.x * 3_deg);

		Shape2D::Star((30 * (1.0 - t)), pos, angle)
			.draw(star.color);
	}

	return (t < 1.0);
}
