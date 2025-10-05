#pragma once

inline Optional<Input> GetDownXInput()
{
	if (XInput(0).isConnected())
	{
		const std::array<Input, 16>arr
		{
			XInput(0).buttonUp,
			XInput(0).buttonDown,
			XInput(0).buttonLeft,
			XInput(0).buttonRight,
			XInput(0).buttonStart,
			XInput(0).buttonBack,
			XInput(0).buttonLThumb,
			XInput(0).buttonRThumb,
			XInput(0).buttonLB,
			XInput(0).buttonRB,
			XInput(0).buttonA,
			XInput(0).buttonB,
			XInput(0).buttonX,
			XInput(0).buttonY,
		};

		for (auto& input : arr)
		{
			if (input.down())
			{
				return input;
			}
		}
	}

	return none;
}
