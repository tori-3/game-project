#pragma once

inline String GetKeyName(const Input& input)
{
	if (input.deviceType() == InputDeviceType::Keyboard)
	{
		const char32 code = 0x100000 + input.code();

		if (FontAsset{U"Keyboard"}.hasGlyph(code))
		{
			return String{ 1,code };
		}
		else
		{
			return input.name();
		}
	}
	else if (input.deviceType() == InputDeviceType::XInput)
	{
		const char32 code = 0x100100 + input.code();
		return String{ 1,code };
	}

	return input.name();

	//const std::map<Input, String>table
	//{
	//	{XInput(0).buttonStart,U"\U000F02B4Start"},
	//	{XInput(0).buttonBack,U"\U000F02B4Back"},
	//	{XInput(0).buttonLThumb,U"\U000F02B4\U000F0EC2(L)"},
	//	{XInput(0).buttonRThumb,U"\U000F02B4\U000F0EC2(R)"},
	//	{XInput(0).buttonLB,U"\U000F02B4LB"},
	//	{XInput(0).buttonRB,U"\U000F02B4RB"},
	//	{XInput(0).buttonUp,U"\U000F02B4\U000F0E42"},
	//	{XInput(0).buttonDown,U"\U000F02B4\U000F0E39"},
	//	{XInput(0).buttonLeft,U"\U000F02B4\U000F0E3A"},
	//	{XInput(0).buttonRight,U"\U000F02B4\U000F0E3B"},
	//	{XInput(0).buttonA,U"\U000F02B4A"},
	//	{XInput(0).buttonB,U"\U000F02B4B"},
	//	{XInput(0).buttonX,U"\U000F02B4X"},
	//	{XInput(0).buttonY,U"\U000F02B4Y"},
	//	{KeyUp,U"↑"},
	//	{KeyDown,U"↓"},
	//	{KeyLeft,U"←"},
	//	{KeyRight,U"→"},
	//};

	//if (table.contains(input))
	//{
	//	return table.at(input);
	//}
	//else
	//{
	//	return Format(input);
	//}
}

inline Input SelectKey(const InputGroup& inputGroup)
{
	//コントローラーモードでコントローラーのキーがあったら返す
	if (XInput(0).isConnected())
	{
		for(auto& input: inputGroup.inputs())
		{
			if(input.deviceType()== InputDeviceType::XInput)
			{
				return input;
			}
		}
	}

	Optional<Input> directionKey;

	//矢印キー以外のキーボードがあれば返す
	for (auto& input : inputGroup.inputs())
	{
		if (input.deviceType() == InputDeviceType::Keyboard)
		{
			if (input == KeyUp || input == KeyDown || input == KeyLeft || input == KeyRight)
			{
				directionKey = input;
			}
			else
			{
				return input;
			}
		}
	}

	//矢印キーを返す
	if(directionKey)
	{
		return directionKey.value();
	}

	return inputGroup.inputs().front();
}

inline String ToKeyName(const InputGroup& inputGroup)
{
	return GetKeyName(SelectKey(inputGroup));
}
