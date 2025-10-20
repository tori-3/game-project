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
}

inline Input SelectKey(const InputGroup& inputGroup,bool gamepadMode)
{
	//コントローラーモードでコントローラーのキーがあったら返す
	if (gamepadMode)
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

inline String ToKeyName(const InputGroup& inputGroup, bool gamepadMode)
{
	return GetKeyName(SelectKey(inputGroup, gamepadMode));
}
