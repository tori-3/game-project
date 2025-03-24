#pragma once
#include<Siv3D.hpp>

class LongPressInput
{
public:

	InputGroup input;
	Duration interval = 0.2s;
	Duration startTime = 0.4s;

	LongPressInput() = default;

	LongPressInput(const InputGroup& input, Duration interval = 0.2s, Duration startTime=0.4s)
		: input{ input }
		, interval{ interval }
		, startTime{startTime}
		, m_lastPressTime {startTime}
	{}

	bool down()
	{
		bool down = input.down();

		Duration pressedDuration = input.pressedDuration();

		if (m_lastPressTime + interval <= pressedDuration)
		{
			down = true;
			m_lastPressTime += interval;
		}

		if (not input.pressed())
		{
			m_lastPressTime = startTime;
		}

		return down;
	}

private:

	Duration m_lastPressTime = 0s;
};

