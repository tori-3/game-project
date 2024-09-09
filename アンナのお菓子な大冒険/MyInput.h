#pragma once

class MyInput
{
public:

	double pressedTime = 0;

	bool downFlg = false;

	Optional<InputGroup> key = none;

	MyInput(InputGroup key_)
		:key{ key_ }
	{

	}

	MyInput()
	{

	}

	void setKey(InputGroup key_) {
		key = key_;
	}

	void update(bool pressedFlg)
	{
		downFlg = pressedFlg && pressedTime == 0;

		if (pressedFlg)
		{
			pressedTime += Scene::DeltaTime();
		}
		else
		{
			pressedTime = 0;
		}
	}

	bool down()
	{
		return key ? key->down() : downFlg;
	}

	bool pressed()
	{
		return key ? key->pressed() : 0 < pressedTime;
	}

	Duration pressedDuration() {
		return key ? key->pressedDuration() : Duration{ pressedTime };
	}
};

