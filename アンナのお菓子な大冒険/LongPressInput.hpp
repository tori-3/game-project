#pragma once
#include"ControllerManager.h"

class LongPressInput
{
public:

	InputGroup input;
	Duration interval = 0.2s;
	Duration startTime = 0.4s;

	ControllerManager::Direction direction = ControllerManager::Direction::Up;

	LongPressInput() = default;

	LongPressInput(const InputGroup& input, ControllerManager::Direction direction, Duration interval = 0.2s, Duration startTime=0.4s)
		: input{ input }
		, interval{ interval }
		, startTime{startTime}
		, direction{ direction }
		, m_lastPressTime {startTime}
	{}

	bool down()
	{
		bool down = input.down() || ControllerManager::Down(direction);

		Duration pressedDuration = Max(input.pressedDuration(), ControllerManager::PressedDuration(direction));

		if (m_lastPressTime + interval <= pressedDuration)
		{
			down = true;
			m_lastPressTime = pressedDuration;
		}

		if (not (input.pressed() || ControllerManager::Pressed(direction)))
		{
			m_lastPressTime = startTime;
		}

		return down;
	}

private:

	Duration m_lastPressTime = 0s;
};

