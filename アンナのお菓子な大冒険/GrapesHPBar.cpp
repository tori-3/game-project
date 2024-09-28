#include"GrapesHPBar.h"

GrapesHPBar::GrapesHPBar(const Rect& rect)
	: m_rect{ rect } {}

void GrapesHPBar::update(double targetHP)
{
	m_targetHP = targetHP;
	m_liquidHP = Math::SmoothDamp(m_liquidHP, targetHP, m_liquidHPVelocity, LiquidSmoothTime);

	if (m_solidHP < targetHP)
	{
		m_solidHP = targetHP;
	}
	else
	{
		m_solidHP = Math::SmoothDamp(m_solidHP, targetHP, m_solidHPVelocity, SolidSmoothTime, MaxSolidBarSpeed);
	}
}

void GrapesHPBar::draw(const ColorF& liquidColorFront, const ColorF& solidColor) const
{

	//ストロー
	Rect{ Arg::center = m_rect.tr() + Point{-10,0},5,40 }.rotated(30_deg).draw(Palette::Pink);

	// バーの背景を描く
	m_rect.draw(Palette::Skyblue);

	// バーの枠を描く
	m_rect.drawFrame(2, 0);

	const Point basePos = m_rect.pos.movedBy(FrameThickness, FrameThickness);
	const int32 height = (m_rect.h - (FrameThickness * 2));
	const double width = (m_rect.w - (FrameThickness * 2));

	const double solidWidth = Min(Max((width * m_solidHP) + (height * 0.5 * 0.3), 0.0), width);
	const double liquidWidth = (width * m_liquidHP);

	// 固体バーを描く
	{
		const RectF solidBar{ basePos, solidWidth, height };
		const double alpha = ((0.005 < AbsDiff(m_targetHP, m_solidHP)) ? 1.0 : (AbsDiff(m_targetHP, m_solidHP) / 0.005));
		solidBar.draw(ColorF{ solidColor, alpha });
	}


	// 液体バーを描く
	{
		const double t = Scene::Time();
		const double offsetScale = ((m_liquidHP < 0.05) ? (m_liquidHP / 0.05) : (0.98 < m_liquidHP) ? 0.0 : 1.0);

		// 前景の液体バーを描く
		for (int32 i = 0; i < height; ++i)
		{
			const Vec2 pos = basePos.movedBy(0, i);;
			const double waveOffset = (i * 0.3)
				+ (Math::Sin(i * 17_deg - t * 800_deg) * 0.8)
				+ (Math::Sin(i * 11_deg - t * 700_deg) * 1.2)
				+ (Math::Sin(i * 7_deg - t * 550_deg) * 1.6);
			const RectF rect{ pos, Clamp((liquidWidth + waveOffset * offsetScale), 0.0, width), 1 };
			const double distance = Clamp(1.0 - i / (height - 1.0) + 0.7, 0.0, 1.0);
			rect.draw(liquidColorFront);
		}
	}


	GlassReflection(m_rect.pos, m_rect.h * 0.8);

	grapes.resized(m_rect.h).drawAt(m_rect.br() - Vec2{ 10,10 });
}

void GrapesHPBar::GlassReflection(const Vec2& pos, double size)
{
	Triangle(pos.x, pos.y, pos.x + size * 0.85, pos.y, pos.x, pos.y + size * 0.85).draw(ColorF(1, 0), ColorF(1, 0.2), ColorF(1, 0.5));
	Quad{ pos + Vec2{ size * 0.9, 0 }, pos + Vec2{ size, 0 }, pos + Vec2{ 0,size  }, pos + Vec2{ 0, size * 0.9 } }.draw(ColorF(1, 0.5));
}
