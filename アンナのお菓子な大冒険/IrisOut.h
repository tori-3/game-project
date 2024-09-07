#pragma once

class IrisOut {
public:
	Vec2* pos=nullptr;
	double r = 0;
	double time = 0;
	bool isStart = false;

	IrisOut() = default;

	void start(Vec2* pos_, double r_)
	{
		pos = pos_;
		r = r_;
		time = 0;
		isStart = true;
	}

	void update()
	{
		if (isStart)
		{
			time += Scene::DeltaTime();
		}
	}

	void draw()const
	{
		if (not isStart)
		{
			return;
		}

		//2秒待つ
		if (time <= 2.0) {
			return;
		}
		else {

			double t = time - 2.0;

			Circle m_circle{ *pos,r };

			Circle circle{};
			if (t <= 1.0) {
				circle = Circle{ m_circle.center,Scene::Size().length() }.lerp(m_circle, t);
			}
			else if (t <= 2.0) {
				circle = m_circle;
			}
			else if (t <= 3.0) {
				circle = m_circle.lerp(Circle{ m_circle.center,0 }, t - 2);
			}
			else {
				RectF{ Arg::center = m_circle.center,Scene::Size() * 2 }.draw(Palette::Black);
				return;
			}

			circle.drawFrame(0, Scene::Size().length(), ColorF{ 0.0, 1.0 });
		}
	}
};
