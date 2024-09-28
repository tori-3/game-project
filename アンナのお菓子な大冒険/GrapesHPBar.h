#pragma once
class GrapesHPBar
{
public:

	GrapesHPBar() = default;

	explicit GrapesHPBar(const Rect& rect);

	void update(double targetHP);

	void draw(const ColorF& liquidColorFront, const ColorF& solidColor) const;

private:

	static void GlassReflection(const Vec2& pos, double size);

	// 液体バーが減少するときの平滑化時間（小さいと早く目標に到達）
	static constexpr double LiquidSmoothTime = 0.03;

	// 固体バーが減少するときの平滑化時間（小さいと早く目標に到達）
	static constexpr double SolidSmoothTime = 0.5;

	// 固体バーが減少するときの最大の速さ
	static constexpr double MaxSolidBarSpeed = 0.25;

	static constexpr int32 FrameThickness = 2;

	Rect m_rect = Rect::Empty();

	Texture grapes{ U"🍇"_emoji };

	double m_targetHP = 1.0;
	double m_liquidHP = 1.0;
	double m_solidHP = 1.0;
	double m_liquidHPVelocity = 0.0;
	double m_solidHPVelocity = 0.0;
};
