#pragma once

//https://gist.github.com/Reputeless/1f578c6b86c2f9a5d44172b47749eb58
class KeyConfigUtility
{
public:

	/// @brief InputGroup を JSON に変換します。
	/// @param inputGroup 変換する InputGroup
	/// @return JSON 形式のデータ
	static JSON ToJSON(const InputGroup& inputGroup)
	{
		JSON json;
		json[U"type"] = U"InputGroup";

		for (const auto& input : inputGroup.inputs())
		{
			json[U"inputs"].push_back(ToJSON(input));
		}

		for (const auto& inputCombination : inputGroup.inputCombinations())
		{
			json[U"inputCombinations"].push_back(ToJSON(inputCombination));
		}

		return json;
	}

	/// @brief JSON から InputGroup を生成します。
	/// @param json JSON 形式のデータ
	/// @return 生成された InputGroup
	/// @throws Error JSON の形式が不正な場合
	static InputGroup FromJSON(const JSON& json)
	{
		InputGroup inputGroup;

		if ((not json.hasElement(U"type")) || (json[U"type"].get<String>() != U"InputGroup"))
		{
			throw Error{ U"Invalid JSON format for InputGroup" };
		}

		if (json.hasElement(U"inputs") && json[U"inputs"].isArray())
		{
			for (const auto& inputJson : json[U"inputs"])
			{
				inputGroup = (inputGroup | ToInput(inputJson.value));
			}
		}

		if (json.hasElement(U"inputCombinations") && json[U"inputCombinations"].isArray())
		{
			for (const auto& inputCombinationJson : json[U"inputCombinations"])
			{
				inputGroup = (inputGroup | ToInputCombination(inputCombinationJson.value));
			}
		}

		return inputGroup;
	}

	static void CleapInput(InputGroup& inputGroup)
	{
		for (Input input : inputGroup.inputs())
		{
			input.clearInput();
		}
	}

private:

	static JSON ToJSON(const Input& input)
	{
		int32 device = FromEnum(input.deviceType());
		int32 code = input.code();
		int32 playerIndex = input.playerIndex();

		return JSON{
			{ U"device", device },
			{ U"code", code },
			{ U"playerIndex", playerIndex }
		};
	}

	static Input ToInput(const JSON& json)
	{
		const uint8 device = json[U"device"].get<uint8>();
		const uint8 code = json[U"code"].get<uint8>();
		const uint8 playerIndex = json[U"playerIndex"].get<uint8>();
		return Input{ ToEnum<InputDeviceType>(device), code, playerIndex };
	}

	static JSON ToJSON(const InputCombination& inputCombination)
	{
		JSON json;
		json.push_back(ToJSON(inputCombination.input1()));
		json.push_back(ToJSON(inputCombination.input2()));
		return json;
	}

	static InputCombination ToInputCombination(const JSON& json)
	{
		const Input input1 = ToInput(json[0]);
		const Input input2 = ToInput(json[1]);
		return InputCombination{ input1, input2 };
	}
};
