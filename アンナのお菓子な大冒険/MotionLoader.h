#pragma once
#include"AnimationSystem.h"
#include"Movements.h"

class MotionLoader {
public:

	CSV csv;
	HashTable<String, std::function<Move* (const Array<String>&)>>moveResolver;

	MotionLoader(const CSV& csv) :csv{ csv } {
		resetResolver();
	}

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
			if (list.size() <= 3)return new AddZ(list[1], Parse<double>(list[2]));
			else return new AddZ(list[1], Parse<double>(list[2]), Parse<bool>(list[3]));
		};

		moveResolver[U"Base"] = [](const Array<String>& list) {
			/*if (list.size() <= 2)return new TransformToBase(list[1]);
			else return new TransformToBase(list[1], Parse<double>(list[2]));*/
			return new TransformToBase(list[1]);
		};
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
			else {
				for (size_t col = 0; col < tmpCsv.columns(row); ++col) {

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
};
