#pragma once
#include"IMotionField.h"
#include"Movements.h"

class MotionLoader {
public:
	CSV csv;
	HashTable<String, std::function<Move* (const Array<String>&)>>moveResolver;

	struct ParamManager {
		//first->region
		HashTable<String, FieldContainer> parameters;

		FieldContainer& operator [] (const String& region) {
			return parameters[region];
		}

		HashSet<String>getReloadList()const {
			HashSet<String> ret;
			for (const auto& [region, container] : parameters) {
				for (const auto& [param_name, param] : container.table) {
					//パラメータが以前の値と変わっていたらretに追加
					if (pre.at(region).at(param_name) != param->getValueString()) {
						ret.emplace(region);
						break;
					}
				}
			}
			return ret;
		}

		void update()
		{
			//preの更新
			pre = HashTable<String, HashTable<String, String>>{};
			for (const auto& [region, container] : parameters) {
				for (const auto& [param_name, param] : container.table) {
					if (not pre.contains(region))pre.emplace(region, HashTable<String, String>{});
					pre[region].emplace(param_name, param->getValueString());
				}
			}
		}
	private:
		friend MotionLoader;
		HashTable<String, HashTable<String, String>>pre;
	}param;

	MotionLoader(const CSV& csv) :csv{ csv } {

		resetResolver();
	}

	void paramSetting()
	{
		String region = U"";
		for (size_t row = 0; row < csv.rows(); ++row)
		{
			if (csv.columns(row) == 0)continue;

			if (csv[row][0][0] == U'#')
			{
				region = csv[row][0];
				region.pop_front();
			}

			if (csv[row][0] == U"int") {
				getVariable<int32>(region, csv[row][1], csv[row][2]);
			}
			else if (csv[row][0] == U"double") {
				getVariable<double>(region, csv[row][1], csv[row][2]);
			}
		}
	};

	void resetResolver() {
		moveResolver[U"Rotate"] = [](const Array<String>& list) {return new Rotate{ list[1],Parse<double>(list[2]) * 1_deg,Parse<double>(list[3]) }; };

		moveResolver[U"Wait"] = [](const Array<String>& list) {return new Wait{ Parse<double>(list[1]) }; };

		moveResolver[U"RotateTo"] = [](const Array<String>& list) {
			if (list.size() <= 3)return new RotateTo(list[1], Parse<double>(list[2]) * 1_deg, 0);//0秒で指定した角度へ
			else if (list.size() <= 4)return new RotateTo(list[1], Parse<double>(list[2]) * 1_deg, Parse<double>(list[3]));//指定した秒数で指定した角度へ
			else return new RotateTo(list[1], Parse<double>(list[2]) * 1_deg, Parse<double>(list[3]), Parse<bool>(list[4]));//指定した秒数で指定された方向に指定された角度へ
		};
		//現在地から指定した分の移動
		moveResolver[U"Move"] = [](const Array<String>& list) {return new Translate(list[1], Vec2{ Parse<double>(list[2]),Parse<double>(list[3]) }, Parse<double>(list[4])); };

		moveResolver[U"MoveTo"] = [](const Array<String>& list) {return new SetPos(list[1], Vec2{ Parse<double>(list[2]),Parse<double>(list[3]) }, Parse<double>(list[4])); };

		moveResolver[U"ChangeTexture"] = [](const Array<String>& list) {
			if (list.size() < 4)return new ChangeTexture(list[1], list[2]);
			else return new ChangeTexture(list[1], list[2], Parse<double>(list[3]));
		};

		moveResolver[U"ChangeColor"] = [](const Array<String>& list) {
			if (list.size() <= 7)return new ChangeColor{ list[1], ColorF{ Parse<double>(list[2]),Parse<double>(list[3]),Parse<double>(list[4]),Parse<double>(list[5]) }, Parse<double>(list[6]) };
			else return new ChangeColor{ list[1], ColorF{ Parse<double>(list[2]),Parse<double>(list[3]),Parse<double>(list[4]),Parse<double>(list[5]) }, Parse<double>(list[6]),Parse<bool>(list[7]) };
		};

		moveResolver[U"ChangeScale"] = [](const Array<String>& list) {
			if (list.size() < 6)return new ChangeScale{ list[1],Parse<double>(list[2]),Parse<double>(list[3]),Parse<double>(list[4]) };
			else return new ChangeScale{ list[1],Parse<double>(list[2]),Parse<double>(list[3]),Parse<double>(list[4]) ,Parse<bool>(list[5]) };
		};

		moveResolver[U"SetScale"] = [](const Array<String>& list) {
			if (list.size() < 6)return new BaseChangeScale{ list[1],Parse<double>(list[2]),Parse<double>(list[3]),Parse<double>(list[4]) };
			else return new BaseChangeScale{ list[1],Parse<double>(list[2]),Parse<double>(list[3]),Parse<double>(list[4]) ,Parse<bool>(list[5]) };
		};

		moveResolver[U"StartMotion"] = [&](const Array<String>& list) {
			if (list.size() <= 4)return new StartMotion(csv, list[1], Parse<bool>(list[2]), Parse<double>(list[3]));
			else return new StartMotion(csv, list[1], Parse<bool>(list[2]), Parse<double>(list[3]), Parse<double>(list[4]));
		};

		moveResolver[U"SetAbsAngle"] = [](const Array<String>& list) {return new SetRad(list[1], Parse<double>(list[2])); };

		moveResolver[U"AddZ"] = [](const Array<String>& list) {
			if (list.size() <= 3)return new AddZ(list[1], Parse<double>(list[2]), 0);
			else if (list.size() <= 4)return new AddZ(list[1], Parse<double>(list[2]), Parse<double>(list[3]));
			else return new AddZ(list[1], Parse<double>(list[2]), Parse<double>(list[3]), Parse<bool>(list[4]));
		};

		moveResolver[U"SetZ"] = [](const Array<String>& list) {
			if (list.size() <= 3)return new LerpZ(list[1], Parse<double>(list[2]), 0);
			else if (list.size() <= 4)return new LerpZ(list[1], Parse<double>(list[2]), Parse<double>(list[3]));
			else return new LerpZ(list[1], Parse<double>(list[2]), Parse<double>(list[3]), Parse<bool>(list[4]));
		};

		moveResolver[U"Base"] = [](const Array<String>& list) {
			if (list.size() <= 2)
				return new TransformToBase(list[1], 0);
			else
				return new TransformToBase(list[1], Parse<double>(list[2]));
		};
	}

	template<class T>
	MotionField<T>& getParam(const String& region, const String& paramName)
	{
		return param[region].get<T>(paramName);
	}

	Motion LoadMotion(String region = U"", bool debug = false) {

		Motion motion;

		CSV tmpCsv = csv;

		size_t index = 0;
		if (region != U"") {
			for (size_t row = 0; row < tmpCsv.rows(); ++row)
			{
				if ((tmpCsv.columns(row) != 0) && tmpCsv[row][0] == U"#{}"_fmt(region)) {
					index = row + 1;
					break;
				}
			}
		}

		HashTable<String, String>variableTable;

		Motion* tmpMotion = new Motion{ true };

		bool addFlg = false;

		for (size_t row = index; row < tmpCsv.rows(); ++row)
		{
			if (tmpCsv.columns(row) == 0) {
				if (addFlg) {
					motion.add(tmpMotion);
					tmpMotion = new Motion{ true };
					addFlg = false;
				}
			}
			else if (tmpCsv[row][0][0] == U'/' and tmpCsv[row][0][1] == U'/') {
			}
			else if (tmpCsv[row][0][0] == U'#') {
				break;
			}
			else if (tmpCsv[row][0][0] == U'@') {
				variableTable[tmpCsv[row][0]] = tmpCsv[row][1];
			}
			else if (csv[row][0] == U"int") {
				variableTable[tmpCsv[row][1]] = getVariable<int32>(region, csv[row][1], csv[row][2]);
			}
			else if (csv[row][0] == U"double") {
				variableTable[tmpCsv[row][1]] = getVariable<double>(region, csv[row][1], csv[row][2]);
			}
			else {
				for (size_t col = 0; col < tmpCsv.columns(row); ++col) {
					auto [able, cal_text] = remove_caliculatable_sign(tmpCsv[row][col]);
					if (able)//計算する部分であれば
					{
						for (const auto& v : variableTable)
							cal_text.replace(v.first, v.second);
						tmpCsv[row][col] = calculate(*able, cal_text);
					}

					if (variableTable.contains(tmpCsv[row][col])) {
						tmpCsv[row][col] = variableTable[tmpCsv[row][col]];
					}
				}

				if (moveResolver.contains(tmpCsv[row][0])) {
					try {
						tmpMotion->add(moveResolver[tmpCsv[row][0]](tmpCsv[row]));
						addFlg = true;
					}
					catch (...) {
						Print << U"おそらく引数が間違っています。";
					}
				}
				else if (debug)
				{
					Print << U"存在しない命令です。";
				}
			}
		}
		//あとで考える
		if (addFlg) {
			motion.add(tmpMotion);
		}
		else delete tmpMotion;

		return motion;
	}

private:
	template<class T>
	String getVariable(const String& region, const String& name, const String& default_value)
	{
		if (not param[region].table.contains(name)) {
			param[region].table[name] = new MotionField(Parse<T>(default_value));
			param.pre[region][name] = default_value;
		}
		return param[region].table[name]->getValueString();
	}

	//計算目印を検出&排除した文字列を返す
	std::tuple<Optional<String>, String> remove_caliculatable_sign(const String& text) {
		Optional<String> sign = none;
		String result{};

		if (text[1] == U'(' and text.back() == U')') {
			if (text[0] == U'i')sign = U"int";
			else if (text[0] == U'd')sign = U"double";
		}
		if (not sign)return std::make_tuple(sign, text);

		for (size_t i = 2; i < text.size() - 1; ++i)result << text[i];
		return std::make_tuple(sign, result);
	}

	String calculate(const String& type, const String& text)
	{
		String result{};
		if (type == U"int")result = Format(cal_recursive<int32>(text));
		else if (type == U"double")result = Format(cal_recursive<double>(text));

		return result;
	}

	//int か double
	template <class T>
	T cal_recursive(String text) {
		Optional<T> result = none;
		String term{};
		for (auto it = text.begin(), en = text.end(); it != en and (not text.isEmpty());)
		{
			String c{ (*it) };
			it = text.erase(it);

			if (c == U"+" or c == U"-" or c == U"*" or c == U"/") {
				result = Parse<T>(term);

				auto [type, cont] = remove_caliculatable_sign(text);

				auto next = Parse<double>(type ? calculate(*type, cont) : calculate(U"double", cont));
				if (c == U"+")*result += next;
				else if (c == U"-")*result -= next;
				else if (c == U"*")*result *= next;
				else if (c == U"/")*result /= next;
				break;
			}

			term += c;
		}

		if (not result)result = Parse<T>(term);

		return *result;
	}
};
