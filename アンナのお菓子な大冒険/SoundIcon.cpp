#pragma once
#include"SoundIcon.hpp"

SizeF SoundIcon::onGetSize()const
{
	return SizeF{ m_size,m_size };
}

void SoundIcon::onDraw(const RectF&)const
{
	const RectF rect = getRect();
	const double w = rect.w / 4.0;
	const Vec2 pos = rect.center();

	if (volume == 0)Shape2D::Cross(w * 0.7, w * 0.1, { pos.x + w * 1.4,pos.y }).draw(color);
	if (0 < volume)Circle{ pos,w }.drawArc(45_deg, 90_deg, w * 0.1, 0, color);
	if (0.3 <= volume)Circle{ pos,w * 1.5 }.drawArc(45_deg, 90_deg, w * 0.1, 0, color);
	if (0.7 <= volume)Circle{ pos,w * 2 }.drawArc(45_deg, 90_deg, w * 0.1, 0, color);
	Triangle{ pos, w * 2, -90_deg }.draw(color);
	RectF{ Arg::center(pos.x - w * 0.7,pos.y),w }.draw(color);
}
