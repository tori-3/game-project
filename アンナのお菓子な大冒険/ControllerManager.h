#pragma once

class ControllerManager
{
public:

	enum class Direction { Up, Down, Left, Right };

	double vibration = 1.0;

	double vibrationTime = 0;

	void setVibration(double time = 0.5)
	{
		vibrationTime = Max(time, vibrationTime);
	}

	void update()
	{
		lastDown = DownPressed();
		lastUp = UpPressed();
		lastLeft = LeftPressed();
		lastRight = RightPressed();

		if(lastDown)
		{
			downTime += Scene::DeltaTime();
		}
		else
		{
			downTime = 0;
		}

		if (lastUp)
		{
			upTime += Scene::DeltaTime();
		}
		else
		{
			upTime = 0;
		}

		if (lastLeft)
		{
			leftTime += Scene::DeltaTime();
		}
		else
		{
			leftTime = 0;
		}

		if (lastRight)
		{
			rightTime += Scene::DeltaTime();
		}
		else
		{
			rightTime = 0;
		}


		if(0<vibrationTime)
		{
			XInput(0).setVibration({ vibration ,vibration });

			vibrationTime = Max(vibrationTime - Scene::DeltaTime(), 0.0);
		}
		else
		{
			XInput(0).stopVibration();
		}
	}

	static bool RightPressed()
	{
		return  PressedHelper(XInput(0).leftThumbD8(), 1, 2, 3);
	}

	static bool LeftPressed()
	{
		return  PressedHelper(XInput(0).leftThumbD8(), 5, 6, 7);
	}

	static bool UpPressed()
	{
		return  PressedHelper(XInput(0).leftThumbD8(), 7, 0, 1);
	}

	static bool DownPressed()
	{
		return  PressedHelper(XInput(0).leftThumbD8(), 3, 4,5);
	}

	static bool DownDown()
	{
		return (not lastDown) && DownPressed();
	}

	static bool UpDown()
	{
		return (not lastUp) && UpPressed();
	}
	static bool LeftDown()
	{
		return (not lastLeft) && LeftPressed();
	}
	static bool RightDown()
	{
		return (not lastRight) && RightPressed();
	}

	static bool LeftUp()
	{
		return lastLeft && not LeftPressed();
	}

	static bool RightUp()
	{
		return lastRight && not RightPressed();
	}

	static bool Pressed(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:return UpPressed();
		case Direction::Down:return DownPressed();
		case Direction::Left:return LeftPressed();
		default:return RightPressed();
		}
	}

	static bool Down(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:return UpDown();
		case Direction::Down:return DownDown();
		case Direction::Left:return LeftDown();
		default:return RightDown();
		}
	}

	static Duration PressedDuration(Direction direction)
	{
		switch (direction)
		{
		case Direction::Up:return  Duration{ upTime };
		case Direction::Down:return  Duration{ downTime };
		case Direction::Left:return  Duration{ leftTime };
		default:return  Duration{ rightTime };
		}
	}

private:

	static bool PressedHelper(Optional<int32> input,int32 a,int32 b,int32 c)
	{
		if (not input)
		{
			return false;
		}

		if(input==a||input==b||input==c)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:

	inline static bool lastDown = false;
	inline static bool lastUp = false;
	inline static bool lastLeft = false;
	inline static bool lastRight = false;

	inline static double downTime = 0;
	inline static double upTime = 0;
	inline static double leftTime = 0;
	inline static double rightTime = 0;

//シングルトン
public:
	ControllerManager(const  ControllerManager&) = delete;
	ControllerManager& operator=(const  ControllerManager&) = delete;
	ControllerManager(ControllerManager&&) = delete;
	ControllerManager& operator=(ControllerManager&&) = delete;

	static ControllerManager& get()
	{
		static ControllerManager instance;
		return instance;
	}

private:
	ControllerManager() = default;
	~ControllerManager() = default;
};
