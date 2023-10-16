# include <Siv3D.hpp>
# include "table.h"


bool check_range(Point pos, Point size) {
	if (pos.x < 0 || size.x <= pos.x)return 0;
	if (pos.y < 0 || size.y <= pos.y)return 0;
	return 1;
}

BlendState MakeBlendState()
{
	BlendState blendState = BlendState::Default2D;
	blendState.srcAlpha = Blend::SrcAlpha;
	blendState.dstAlpha = Blend::DestAlpha;
	blendState.opAlpha = BlendOp::Max;
	return blendState;
}

Grid<String>Load(String path) {


	JSON json = JSON::Load(path);
	if (not json)throw Error{ U"Failed to load `config.json`" };

	Point stage_size = json[U"stage_size"].get<Point>();

	Grid<String>map(stage_size, U"");

	{
		Array<String>material_name;

		for (const auto& object : json[U"Block"])
		{
			material_name << object.key;
		}

		for (int i = 0; i < material_name.size(); i++) {
			for (const auto& elem : json[U"Block"][material_name[i]].arrayView())
			{
				map[elem.get<Point>()] = material_name[i];
			}
		}
	}

	return map;
}


void Main()
{
	setData();

	Constructor();

	Optional<FilePath> path = Dialog::OpenFile({ FileFilter::JSON() });

	if (not path)return;

	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	Window::Resize(1200, 800);

	Grid<String>map = Load(path.value());

	Camera2D camera{ Vec2{ 400, 300 }, 1.0 ,CameraControl::Keyboard };

	Rect UI_rect(1000, 500, 200, 300);

	String setting;

	const Array<std::pair<String, Array<String>>> menus
	{
		{ U"ファイル", { U"保存",U"画像を出力"}},
		{ U"ブロック", { U"ケーキの地表",U"ケーキのスポンジ",U"ケーキの右壁",U"ケーキの左壁",U"ケーキの右端",U"ケーキの左端",U"チョコレートの壁"}},
		{U"特殊ブロック",{U"スライダー",U"横に動くチョコレート",U"縦に動くチョコレート",U"ばね",U"もろい壁",U"ベルトコンベア(→)",U"ベルトコンベア(←)"}},
		{U"敵",{U"イチゴの兵士",U"クッキーの兵士",U"雪だるま",U"イチゴスポナー"}},
		{U"プレイヤー",{U"プレイヤー"}},
		{U"背景(ステージ不可)",{U"アイスクリーム",U"薄ピンク",U"キャンドル",U"キャンディー",U"ステック",U"さくらんぼ",U"ブルーベリー"}},
		{U"ギミック",{U"ドア",U"鷹"}}
	};

	SimpleMenuBar menuBar{ menus };

	bool UI_flag = false;

	while (System::Update())
	{
		if (MouseL.up())UI_flag = false;
		UI_flag |= UI_rect.mouseOver();

		//メニューバー
		if (const auto& item = menuBar.update())
		{
			
			UI_flag = true;

			// 保存が押されたら
			if (item == MenuBarItemIndex{ 0, 0 })
			{
				JSON json2;
				json2[U"stage_size"] = map.size();

				for (auto y : step(map.height()))
				{
					for (auto x : step(map.width()))
					{
						if (map[y][x] != U"") {
							json2[U"Block"][map[y][x]] = { Point(x, y) };
						}
					}
				}
				json2.save(path.value());
			}
			//背景画像として出力
			else if (item == MenuBarItemIndex{ 0, 1 }) {

				Size size{ 1200,800 };

				double r = (double)size.y / (map.size().y * rect_size);

				MSRenderTexture texture{ (map.size() * rect_size * r).asPoint(),ColorF{1,0} };


				Size blurAreaSize{ (map.size() * rect_size * r).asPoint() };
				const RenderTexture gaussianA1{ blurAreaSize }, gaussianB1{ blurAreaSize };


				{
					const ScopedRenderTarget2D target{ texture };
					const ScopedRenderStates2D blend{ MakeBlendState() };
					const Transformer2D transformer{ Mat3x2::Scale(r,{0,0}) };
					for (auto y : step(map.height()))
					{
						for (auto x : step(map.width())) {
							Point pos(x, y);
							if (map[pos] != U"")table[map[pos]](pos);
						}
					}
				}

				{
					Graphics2D::Flush();
					texture.resolve();
				}

				Shader::GaussianBlur(texture, gaussianB1, gaussianA1);

				Shader::Copy(gaussianA1.resized(blurAreaSize), texture);

				{
					Graphics2D::Flush();
					texture.resolve();
				}

				Image image;

				texture.readAsImage(image);
				image.save(U"背景.png");
			}		
			else {
				setting = menus[item->menuIndex].second[item->itemIndex];
			}
		}



		camera.update();

		//カメラの座標
		{
			const auto t = camera.createTransformer();

			Rect{ rect_size * map.size() }.drawFrame(0, 5, Palette::Red);

			//draw
			for (auto y : step(map.height()))
			{
				for (auto x : step(map.width())) {
					Point pos(x, y);
					if (map[pos] != U"")table[map[pos]](pos);
				}
			}

			//set
			if (MouseL.pressed() && (not UI_flag)) {
				Point set = Cursor::Pos() / rect_size;
				if (check_range(set, map.size())) {
					map[set] = nameTable[setting];
				}
			}

			//右クリックでも削除
			if (MouseR.pressed() && (not UI_flag)) {
				Point set = Cursor::Pos() / rect_size;
				if (check_range(set, map.size())) {
					map[set] = U"";
				}
			}

			{
				Point set = Cursor::Pos() / rect_size;
				if (check_range(set, map.size())) {
					Rect{ set * rect_size,rect_size }.draw(ColorF{ 1,0.4 });
				}
			}

		}

		//UI操作
		{
			UI_rect.draw(ColorF(0, 0.5));

			bool expansionFlg = false;
			bool reductionFlg = false;
			int32 addX = 0, addY = 0, moveX = 0, moveY = 0;
			if (SimpleGUI::Button(U"上+", Vec2{ 1020, 600 })) {
				expansionFlg = true;
				addY = 1; moveY = 1;
			}
			if (SimpleGUI::Button(U"下+", Vec2{ 1020, 650 })) {
				expansionFlg = true;
				addY = 1;
			}
			if (SimpleGUI::Button(U"左+", Vec2{ 1020, 700 })) {
				expansionFlg = true;
				addX = 1; moveX = 1;
			}
			if (SimpleGUI::Button(U"右+", Vec2{ 1020, 750 })) {
				expansionFlg = true;
				addX = 1;
			}

			if (SimpleGUI::Button(U"上-", Vec2{ 1120, 600 })) {
				reductionFlg = true;
				addY = -1; moveY = 1;
			}
			if (SimpleGUI::Button(U"下-", Vec2{ 1120, 650 })) {
				reductionFlg = true;
				addY = -1;
			}
			if (SimpleGUI::Button(U"左-", Vec2{ 1120, 700 })) {
				reductionFlg = true;
				addX = -1; moveX = 1;
			}
			if (SimpleGUI::Button(U"右-", Vec2{ 1120, 750 })) {
				reductionFlg = true;
				addX = -1;
			}

			if (expansionFlg) {
				Grid<String>tmp(map.size().x + addX, map.size().y + addY);

				for (auto y : step(map.height()))
				{
					for (auto x : step(map.width()))
					{
						tmp[y + moveY][x + moveX] = map[y][x];
					}
				}
				map = tmp;
			}

			if (reductionFlg) {
				Grid<String>tmp(map.size().x + addX, map.size().y + addY);

				for (auto y : step(tmp.height()))
				{
					for (auto x : step(tmp.width()))
					{
						tmp[y][x] = map[y + moveY][x + moveX];
					}
				}
				map = tmp;
			}

			SimpleGUI::GetFont()(setting).drawAt(UI_rect.centerX(), 550);


		}

		menuBar.draw();

	}
}
