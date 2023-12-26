#pragma once
#include"Block.h"

//何ブロックまで描画するか
constexpr int range_right = 15;
constexpr int range_left = 10;

class Stage
{
public:
	Grid<Block*>map;

	HashTable<String, std::function<Block* (void)>> maker{
		{U"CakeSurface",[]() {return new CakeSurface(); }},
		{U"CakeGround",[]() {return new CakeGround(); }},
		{U"CakeRightWall",[]() {return new CakeRightWall(); }},
		{U"CakeLeftWall",[]() {return new CakeLeftWall(); }},
		{U"CakeRightEdge",[]() {return new CakeRightEdge(); }},
		{U"CakeLeftEdge",[]() {return new CakeLeftEdge(); }},
		{U"ChocolateWall",[]() {return new ChocolateWall(); }},
		{U"Slider",[]() {return new Slider(); }},
		{U"Moving",[]() {return new Moving(); }},
		{U"Elevator",[]() {return new Elevator(); }},
		{U"Spring",[]() {return new Spring(); }},
		{U"StrawberrySoldier",[]() {return new StrawberrySoldierBlock(); }},
		{U"CookieSoldier",[]() {return new CookieSoldierBlock(); }},
		{U"Hawk",[]() {return new Hawk(); }},
		{U"Door",[]() {return new Door(); }},
		{U"WeakWall",[]() {return new WeakWall(); }},
		{U"BeltConveyorRight",[]() {return new BeltConveyorRight(); }},
		{U"BeltConveyorLeft",[]() {return new BeltConveyorLeft(); }},
		{U"SpawnerStrawberrySoldier",[]() {return new SpawnerStrawberrySoldier(); }},
		{U"Snowman",[]() {return new SnowmanBlock(); }},
		{U"ItigoSlave",[]() {return new ItigoSlaveBlock(); }},
		{U"CloudEnemy",[]() {return new CloudEnemyBlock(); }},
		{U"Corn",[]() {return new CornBlock(); }}

	};

	~Stage() {
		for (auto& block : map) {
			delete block;
		}
	}

	Stage(String path) {

		//ステージの画像ファイルをTextureAssetに登録
		for (const auto& path : FileSystem::DirectoryContents(U"StageTexture"))
		{
			TextureAsset::Register(FileSystem::BaseName(path), path, TextureDesc::Mipped);
		}



		//辞書を作成
		//HashTable<String, Block*> table;
		//for (const auto& object : blocks)table.emplace(object->name, object);

		//ファイルをロード
		JSON json = JSON::Load(path);
		if (not json)throw Error{ U"ファイルを読み込めませんでした。" };

		{
			bool hasPlayer = false;

			//ファイルにあるブロック名を読み込む(ステージで使われていないブロックを読み込もうとしてエラーになるのを防ぐため)
			Array<String>block_name;
			for (const auto& block : json[U"Block"]) {
				block_name << block.key;
				if (block.key == U"Player") {
					DataManager::get().addEntity(U"Player", (block.value[0].get<Point>() + Vec2{ 0.5,1 })*rect_size);
					hasPlayer = true;
				}
			}

			//もしプレイヤーの位置が設定されていなかったら
			if (not hasPlayer) {
				DataManager::get().addEntity(U"Player", (Vec2{3,5} + Vec2{0.5,1}) * rect_size);
			}

			//ステージサイズのmapを用意
			Grid<Block*>_map(json[U"stage_size"].get<Point>(),nullptr);

			//mapにファイルのデータを読み込む。
			for (int i = 0; i < block_name.size(); i++) {

				if (block_name[i] != U"Player") {
					for (const auto& elem : json[U"Block"][block_name[i]].arrayView()) {
						//_map[elem.get<Point>()] = table[block_name[i]];
						_map[elem.get<Point>()] = maker[block_name[i]]();
					}
				}
			}

			this->map = std::move(_map);
		}

	}

	void hit(HitBox* hitbox) {
		hitbox->physics.resetFlg();
		int range = 6;
		Point point = hitbox->pos->asPoint() / Point(rect_size, rect_size);
		for (int y = Max(0, point.y - range); y < Min((int)map.height(), point.y + range); y++)
		{
			for (int x = Max(0, point.x - range); x < Min((int)map.width(), point.x + range); x++) {
				Point p(x, y);
				if (map[p] != nullptr)map[p]->reaction(p, &(hitbox->physics));//reaction関数がhanteiに図形を渡して呼び出す
			}
		}
	}

	void update(const Vec2& vec) {

		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - range_left); x < Min((int)map.width(), pos.x + range_right); x++) {
				Point p(x, y);
				if (map[p] != NULL)map[p]->update(p);
			}
		}
	}

	void draw(const Vec2& vec)const {
		//ブロックの描写
		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - range_left); x < Min((int)map.width(), pos.x + range_right); x++) {
				Point p(x, y);
				if (map[p] != NULL)map[p]->draw(p);
			}
		}
	}

	//消すかも
	//Grid <Block*>* getMapP() { return &map; }
	int width() { return (int)map.width(); }
};
