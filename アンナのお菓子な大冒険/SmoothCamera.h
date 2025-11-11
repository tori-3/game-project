#pragma once

class SmoothCamera {
public:

	Vec2 pos{};

	double vel = 0;

	double stageHeight = 0;

	double stageWidth = 0;

	Timer shakeTimer{ 2s };//カメラの揺れ

	void setPos(const Vec2& _pos) {
		pos = _pos;
	}

	void setStageHeight(double h) {
		stageHeight = h;
	}

	void setStageWidth(double w) {
		stageWidth = w;
	}


	void update(const Vec2& _pos) {

		pos = _pos;

		pos.x = Clamp<double>(pos.x, 0, stageWidth - Scene::Width());

		pos.y = Clamp<double>(pos.y, 0, stageHeight - Scene::Height());

		if (DataManager::get().table.contains(U"ShakeCamera"))
		{
			DataManager::get().table.erase(U"ShakeCamera");
			shakeTimer.restart();
		}

	}

	Mat3x2 getMat3x2()const
	{
		return Mat3x2::Translate(-pos) * ShakeMat(shakeTimer, shakeTimer.sF() * 50);

		//if (DataManager::get().table.contains(U"LastBoss"))
		//{
		//	return Mat3x2::Translate(5 - rect_size, -rect_size) * ShakeMat(shakeTimer, shakeTimer.sF() * 50);
		//}
		//else
		//{
		//	return Mat3x2::Translate(-pos) * ShakeMat(shakeTimer, shakeTimer.sF() * 50);
		//}
	}

	static Mat3x2 ShakeMat(const Timer& timer, double amplitude = 20)
	{
		constexpr double N = 2;//揺れの回数
		return Mat3x2::Translate(amplitude * (Periodic::Sine0_1(timer.duration() / N, timer.sF()) - 1 / N), 0);
	}
};
