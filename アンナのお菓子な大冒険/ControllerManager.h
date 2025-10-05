#pragma once

class ControllerManager
{
public:

	double vibration = 1.0;

	double vibrationTime = 0;

	void setVibration(double time = 0.5)
	{
		vibrationTime = Max(time, vibrationTime);
	}

	void update()
	{
		lastDown = DownPressed();

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
