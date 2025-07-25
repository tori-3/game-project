﻿#pragma once
#include"Block.h"

//何ブロックまで描画するか
constexpr int32 range_right = 15;
constexpr int32 range_left = 12;

class Stage
{
public:
	Grid<std::unique_ptr<Block>>map;

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
		{U"Door",[]() {return new Door(); }},
		{U"WeakWall",[]() {return new WeakWall(); }},
		{U"BeltConveyorRight",[]() {return new BeltConveyorRight(); }},
		{U"BeltConveyorLeft",[]() {return new BeltConveyorLeft(); }},
		{U"SpawnerStrawberrySoldier",[]() {return new SpawnerStrawberrySoldier(); }},
		{U"Snowman",[]() {return new SnowmanBlock(); }},
		{U"ItigoSlave",[]() {return new ItigoSlaveBlock(); }},
		{U"CloudEnemy",[]() {return new CloudEnemyBlock(); }},
		{U"Corn",[]() {return new CornBlock(); }},
		{U"CookieMuti",[]() {return new CookieMutiBlock(); }},
		{U"CookieKaban",[]() {return new CookieKabanBlock(); }},
		{U"SnowKnight",[]() {return new SnowKnightBlock(); }},
		{U"Zerosen",[]() {return new ZerosenBlock(); }},
		{U"ThroughBlock",[]() {return new ThroughBlock(); }},
		{U"SlaversCookie",[]() {return new SlaversCookieBlock(); }},
		{U"PoleBlock",[]() {return new  PoleBlock(); }},
		{U"CaptainBlock",[]() {return new CaptainBlock(); }},
		{U"IceCream",[]() {return new IceCream(); }},
		{U"Candle",[]() {return new Candle(); }},
		{U"Candy",[]() {return new Candy(); }},
		{U"CandyStick",[]() {return new CandyStick(); }},
		{U"Cherries",[]() {return new Cherries(); }},
		{U"Blueberry",[]() {return new Blueberry(); }},
		{U"LastBossBlock",[]() {return new LastBossBlock(); }},
		{U"FallingRocksBlock",[]() {return new FallingRocksBlock(); }},
		{U"RollingRocksBlock",[]() {return new RollingRocksBlock(); }},
		{U"CloudBlock",[]() {return new CloudBlock(); }},
		{U"CloudSurfaceBlock",[]() {return new CloudSurfaceBlock(); }},
		{U"CloudBlock",[]() {return new CloudBlock(); }},
		{U"CloudBLBlock",[]() {return new CloudBLBlock(); }},
		{U"CloudBRBlock",[]() {return new CloudBRBlock(); }},
		{U"CloudTLBlock",[]() {return new CloudTLBlock(); }},
		{U"CloudTRBlock",[]() {return new CloudTRBlock(); }},
		{U"CloudLBlock",[]() {return new CloudLBlock(); }},
		{U"CloudRBlock",[]() {return new CloudRBlock(); }},
		{U"CloudBBlock",[]() {return new CloudBBlock(); }},
		{U"ChocolateWallCanNotRespawn",[]() {return new ChocolateWallCanNotRespawn(); }},
		{U"ChocoCakeBlock",[]() {return new ChocoCakeBlock(); }},
		{U"ChocoCakeWallBlock",[]() {return new ChocoCakeWallBlock(); }},
		{U"FairyBlock",[]() {return new FairyBlock(); }},
	};

	Stage(const JSON& json,bool backGround=false) {

		//ステージの画像ファイルをTextureAssetに登録
		for (const auto& path : FileSystem::DirectoryContents(U"StageTexture"))
		{
			TextureAsset::Register(FileSystem::BaseName(path), path, TextureDesc::Mipped);
		}

		//ファイルをロード
		if (not json)throw Error{ U"ファイルを読み込めませんでした。" };

		{
			bool hasPlayer = false;

			//ファイルにあるブロック名を読み込む(ステージで使われていないブロックを読み込もうとしてエラーになるのを防ぐため)
			Array<String>block_name;
			for (const auto& block : json[U"Block"])
			{
				block_name << block.key;
				if (block.key == U"Player")
				{
					DataManager::get().addEntity(U"Player", (block.value[0].get<Point>() + Vec2{ 0.5,1 })*rect_size);
					hasPlayer = true;
				}
			}

			if (not backGround)
			{
				//もしプレイヤーの位置が設定されていなかったら
				if (not hasPlayer) {
					DataManager::get().addEntity(U"Player", (Vec2{ 3,5 } + Vec2{ 0.5,1 }) * rect_size);
				}
			}

			////ステージサイズのmapを用意
			Grid<std::unique_ptr<Block>>_map(json[U"stage_size"].get<Point>());

			//mapにファイルのデータを読み込む。
			for (int i = 0; i < block_name.size(); i++)
			{
				if (block_name[i] == U"SignboardBlock")
				{
					Array<String>pathList;

					for (const auto& elem : json[U"SignboardPath"].arrayView())
					{
						pathList << elem.getString();
					}

					size_t count = 0;
					for (const auto& elem : json[U"Block"][block_name[i]].arrayView())
					{
						Array<TalkWindow::TalkInfo>list;

						const CSV csv{ pathList[count]};

						for (size_t row = 0; row < csv.rows(); ++row)
						{
							list << TalkWindow::TalkInfo{ csv[row][0] ,csv[row][1] };
						}

						_map[elem.get<Point>()] = std::unique_ptr<Block>(new SignboardBlock(list));

						++count;
					}
				}
				else if (block_name[i] != U"Player")
				{
					for (const auto& elem : json[U"Block"][block_name[i]].arrayView())
					{
						_map[elem.get<Point>()] = std::unique_ptr<Block>(maker[block_name[i]]());
					}
				}
			}

			this->map = std::move(_map);

		}

		if (not backGround)
		{
			DataManager::get().stageSize = map.size() * rect_size;
		}
	}

	void hit(HitBox* hitbox)
	{
		hitbox->physics.resetFlg();
		int range = 6;
		Point point = hitbox->pos->asPoint() / Point(rect_size, rect_size);
		for (int y = Max(0, point.y - range); y < Min((int)map.height(), point.y + range); y++)
		{
			for (int x = Max(0, point.x - range); x < Min((int)map.width(), point.x + range); x++)
			{
				Point p(x, y);
				if (map[p] != nullptr)map[p]->reaction(p, &(hitbox->physics));//reaction関数がhanteiに図形を渡して呼び出す
			}
		}

		//ステージの右端と左端にも当たり判定
		hitbox->physics.hit(RectF{ Arg::rightCenter(Vec2{0,0}),rect_size*1000 });
		hitbox->physics.hit(RectF{ Arg::leftCenter(Vec2{map.width()*rect_size,0}),rect_size * 1000 });
	}

	void update(const Vec2& vec,int32 left= range_left,int32 right= range_right) {

		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - left); x < Min((int)map.width(), pos.x + right); x++) {
				Point p(x, y);
				if (map[p] != NULL)map[p]->update(p);
			}
		}
	}

	void draw(const Vec2& vec, int32 left = range_left, int32 right = range_right)const {
		//ブロックの描写
		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - left); x < Min((int)map.width(), pos.x + right); x++)
			{
				Point p(x, y);
				if (map[p] != NULL)map[p]->draw(p);
			}
		}
	}

	void updateAsBackGround(const Vec2& vec, int32 left = range_left, int32 right = range_right)
	{
		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - left); x < pos.x + right; x++)
			{
				Point p(x % map.width(), y);
				if (map[p] != NULL)map[p]->update(Point{ x,y });
			}
		}
	}

	void drawAsBackGround(const Vec2& vec, int32 left = range_left, int32 right = range_right)const
	{
		//ブロックの描写
		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - left); x < pos.x + right; x++) {
				Point p(x % map.width(), y);
				if (map[p] != NULL)map[p]->draw(Point{x,y});
			}
		}
	}

	size_t width()const { return map.width(); }

	size_t height()
	{
		return map.height();
	}
};


class StageBackGround
{
public:

	Stage stage;

	double rate;


	StageBackGround(const JSON& json)
		: stage{ json,true }
		, rate{ json[U"Rate"].get<double>() }
	{}

	void draw(const Vec2& pos)const
	{
		{
			const Transformer2D transformer1{ Mat3x2::Scale(rate, Vec2{0,0}) };
			const Transformer2D transformer2{ Mat3x2::Translate(Vec2{-pos * rate}) };
			stage.drawAsBackGround(pos * rate, 4, brockNum());
		}
	}

	int32 brockNum()const
	{
		return int32(Scene::Width()/ (rect_size*rate)+4.0);
	}
};

class StageBackGroundManager
{
public:

	Array<StageBackGround>backGrounds;

	StageBackGroundManager(const JSON& json)
	{
		for (const auto& elem : json[U"BackGround"].arrayView())
		{
			backGrounds << StageBackGround{ JSON::Load(elem.getString()) };
		}
	}

	void draw(const Vec2& pos)const
	{
		for (auto& backGround : backGrounds)
		{
			backGround.draw(pos);
			{
				const ScopedRenderStates2D blend{ BlendState::NonPremultiplied };
				Rect{ Scene::Size() }.draw(ColorF{ Palette::White,0.2 });
			}
		}
	}
};
