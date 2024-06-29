#pragma once

class MagicCircle
{
public:

	double rad;

	void start(double size)
	{
		m_size = size;
		m_active = true;
		m_t = 0;
	}

	void end()
	{
		m_active = false;
	}

	void setPos(const Vec2& pos)
	{
		m_pos = pos;
	}

	void update()
	{
		m_sizeTransition.update(m_active);
		if (m_active) {
			m_t += Scene::DeltaTime();
		}
	}

	void draw()const
	{
		const Transformer2D transformer0{ Mat3x2::Scale(0.6,1.0, m_pos).rotated(rad,m_pos) };
		const Vec2 center = m_pos;// +Vec2{ -m_size / 2.0,0 };
		{
			const Transformer2D transformer1{ Mat3x2::Rotate(m_t,  center) };
			MagicCircleDraw(Circle{ center, m_sizeTransition.value() * m_size * 3.6 * 0.5 }, HSV{ 0 });
		}
		{
			const Transformer2D transformer2{ Mat3x2::Rotate(-m_t, center) };
			MagicCircleDraw(Circle{ center, m_sizeTransition.value() * m_size * 2.7 * 0.5 }, HSV{ 120 });
		}
		{
			const Transformer2D transformer3{ Mat3x2::Rotate(m_t * 2,  center) };
			MagicCircleDraw(Circle{ center, m_sizeTransition.value() * m_size * 2.0 * 0.5 }, HSV{ 240 });
		}
	}

private:
	double m_size;

	bool m_active = false;

	double m_t;

	Vec2 m_pos{};

	Transition m_sizeTransition{ 0.5s, 0.5s };

	static void MagicCircleDraw(const Circle& circle, const ColorF& color) {
		const Vec2 pos = circle.center;
		const double size = circle.r;
		const double rate = size / 135;

		for (auto i : step(2))
		{
			const double d = (i == 0 ? 3 * rate : 2 * rate);
			const ColorF c = (i == 0 ? color : ColorF{ 1 });

			Circle{ pos,95 * rate }.drawFrame(d, c);

			Circle(pos, size).drawFrame(d, c);
			Circle(pos, 120 * rate).drawFrame(d, c);
			Triangle(pos, 200 * rate).drawFrame(d, c);
			Triangle(pos, 200 * rate, 180_deg).drawFrame(d, c);

			Shape2D::NStar(24, size, 120 * rate, pos).drawFrame(d, c);
			Shape2D::NStar(24, size, 120 * rate, pos, 180_deg / 24).drawFrame(d, c);

			for (auto j : step(6))
			{
				const double theta = j * 60_deg + 30_deg;
				const Vec2 vec = OffsetCircular{ pos, 95 * rate, theta };
				Circle{ vec, 15 * rate }.drawFrame(d, c);
			}
		}
	}
};
