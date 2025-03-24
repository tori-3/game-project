#include"MagicEffect.h"

bool MagicEffect::update(double t)
{
	constexpr double timeLim = 1.0;

	m_pos = OffsetCircular{ m_pos, Scene::DeltaTime() * m_speed,m_angle };
	m_rad += Scene::DeltaTime() * 90_deg;
	m_texture.resized(m_size).rotated(m_rad).drawAt(m_pos, ColorF{ m_color, ((timeLim - t) / timeLim) * 0.7 });

	return (t < timeLim);
}
