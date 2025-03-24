#pragma once

class MagicCircle
{
public:

	double rad = 0;

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

	void draw()const;

private:
	double m_size = 0;

	bool m_active = false;

	double m_t = 0;

	Vec2 m_pos{};

	Transition m_sizeTransition{ 0.5s, 0.5s };

	static void MagicCircleDraw(const Circle& circle, const ColorF& color);
};
