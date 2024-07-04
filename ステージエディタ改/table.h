﻿#pragma once
constexpr int32 rect_size = 70;

//ここで画像の生成などをする
void Constructor() {

	for (const auto& path : FileSystem::DirectoryContents(U"StageTexture"))
	{
		TextureAsset::Register(FileSystem::BaseName(path), path, TextureDesc::Mipped);
	}

	for (const auto& path : FileSystem::DirectoryContents(U"EntityTexture"))
	{
		TextureAsset::Register(FileSystem::BaseName(path), path, TextureDesc::Mipped);
	}

	TextureAsset::Register(U"Hawk",U"🦅"_emoji, TextureDesc::Mipped);

}

void CakeSurface(Point pos) {
	TextureAsset(U"CakeSurface").resized(rect_size).draw(pos * rect_size);
}

void CakeGround(Point pos) {
	TextureAsset(U"CakeGround").resized(rect_size).draw(pos * rect_size);
}

void CakeRightWall(Point pos) {
	TextureAsset(U"CakeRightWall").resized(rect_size).draw(pos * rect_size);
}

void CakeLeftWall(Point pos) {
	TextureAsset(U"CakeLeftWall").resized(rect_size).draw(pos * rect_size);
}

void CakeRightEdge(Point pos) {
	TextureAsset(U"CakeRightEdge").resized(rect_size).draw(pos * rect_size);
}

void CakeLeftEdge(Point pos) {
	TextureAsset(U"CakeLeftEdge").resized(rect_size).draw(pos * rect_size);
}

void ChocolateWall(Point pos) {
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(pos * rect_size);
}

void slider(Point pos) {
	pos *= rect_size;
	Triangle{ pos, pos + Point(0,rect_size), pos + Point(rect_size,rect_size) }.draw(Color{ 240 });
}

void moving(Point pos) {
	Rect rect{ pos * rect_size,rect_size };
	rect.draw(ColorF{ 0,0.3 });
	rect.x += Periodic::Triangle0_1(3s) * rect_size * 3;
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(rect.pos);
}

void elevator(Point pos) {
	Rect rect{ pos * rect_size,rect_size };
	rect.y += rect_size * 3;
	rect.y -= (int32)(Periodic::Triangle0_1(6s) * rect_size * 5);
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(rect.pos);
}

void spring(Point pos) {
	Rect rect{ pos * rect_size,rect_size };
	rect.draw(Palette::White);
}

void StrawberrySoldierBlock(Point pos) {
	TextureAsset(U"StrawberrySoldier").resized(rect_size).draw(pos * rect_size);
}

void CookieSoldierBlock(Point pos) {
	TextureAsset(U"CookieSoldier").resized(rect_size).draw(pos * rect_size);
}

void IceCream(Point pos) {
	TextureAsset(U"IceCream").resized(rect_size * 2).draw(pos * rect_size);
}

//void LightPinkBlock(Point pos) {
//	Rect{ pos * rect_size,rect_size }.draw(ColorF{ Palette::Lightpink }*1.2);
//}

void Candle(Point pos) {
	TextureAsset(U"Candle").resized(rect_size * 2).draw(pos * rect_size- Vec2::UnitX()*rect_size/2);
	TextureAsset(U"fire").resized(rect_size).draw(pos * rect_size-Vec2::UnitY()* rect_size / 2);
}

void Candy(Point pos) {
	TextureAsset(U"Candy").resized(rect_size*2.5).rotated(15_deg).draw(pos * rect_size);
}

void CandyStick(Point pos) {
	TextureAsset(U"CandyStick").resized(rect_size * 2.2).draw(pos * rect_size);
}

void Cherries(Point pos) {
	TextureAsset(U"Cherries").resized(rect_size*1.3).draw(pos * rect_size);
}

void Blueberry(Point pos) {
	TextureAsset(U"Blueberry").resized(rect_size * 1.3).draw(pos * rect_size);
}

void Hawk(Point pos) {
	TextureAsset(U"Hawk").resized(rect_size*1.2).draw(pos * rect_size);
}

void Door(Point pos) {
	Rect{ pos* rect_size,rect_size * 2 }.draw(Palette::Black);
	TextureAsset(U"Door").resized(rect_size * 2).draw(pos * rect_size);
	TextureAsset(U"Door").resized(rect_size * 2).mirrored().draw(pos * rect_size);
}

void WeakWall(Point pos) {
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(pos * rect_size);
	Rect{ pos * rect_size,rect_size }.draw(ColorF{ Palette::Red,0.3 });
}

void BeltConveyorRight(Point pos) {
	const double size = TextureAsset(U"ChocolateWall").size().x;
	const double d = Periodic::Sawtooth0_1(2s);
	TextureAsset(U"ChocolateWall")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
	TextureAsset(U"ChocolateWall")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);

	//const double d2 = Periodic::Sawtooth0_1(2 / 3.0 * 2);
	//TextureAsset(U"ChocolateWall")(0,0,size/3*2,size).resized(rect_size*d2/3*2,rect_size).draw(pos * rect_size);
	//TextureAsset(U"ChocolateWall")(0, 0, size / 3*2, size).resized(rect_size * (1-d2)/3*2, rect_size).draw(Arg::topRight=(pos + Point::UnitX() * 3) * rect_size);
}

void BeltConveyorLeft(Point pos) {
	const double size = TextureAsset(U"ChocolateWall").size().x;
	const double d = 1-Periodic::Sawtooth0_1(2s);
	TextureAsset(U"ChocolateWall")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
	TextureAsset(U"ChocolateWall")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
}

void SpawnerStrawberrySoldier(Point pos) {
	TextureAsset(U"StrawberrySoldier").resized(rect_size).draw(pos * rect_size,ColorF{1,0.7});
	Rect{ pos * rect_size,rect_size }.drawFrame(5,0, Palette::Orange);
}

void Player(Point pos) {
	TextureAsset(U"アンナ").resized(rect_size,rect_size*2).draw(pos * rect_size);
}

void Snowman(Point pos) {
	TextureAsset(U"arm").resized(rect_size*2).draw(pos * rect_size);
	TextureAsset(U"base-body").resized(rect_size*2).draw(pos * rect_size);
	TextureAsset(U"hat").resized(rect_size*2).draw(pos * rect_size);
	TextureAsset(U"head").resized(rect_size*2).draw(pos * rect_size);
}

void ItigoSlave(Point pos) {
	TextureAsset(U"itigoSlave-rfoot").resized(rect_size ).draw(pos * rect_size);
	TextureAsset(U"itigoSlave-lfoot").resized(rect_size ).draw(pos * rect_size);
	TextureAsset(U"itigoSlave-body").resized(rect_size ).draw(pos * rect_size);
	TextureAsset(U"itigoSlave-weapon").resized(rect_size ).draw(pos * rect_size);
}

void CloudEnemy(Point pos) {
	TextureAsset(U"cloud-body").resized(rect_size).draw(pos * rect_size);
	TextureAsset(U"cloud-stick").resized(rect_size).draw(pos * rect_size);
	TextureAsset(U"cloud-face1").resized(rect_size).draw(pos * rect_size);
}

void Corn(Point pos) {
	TextureAsset(U"corn").resized(rect_size).draw(pos * rect_size);
}

void CookieMuti(Point pos) {
	TextureAsset(U"cookieMuti-body").resized(rect_size).draw(pos * rect_size);
	TextureAsset(U"cookieMuti-whip").resized(rect_size).draw(pos * rect_size);
}

void CookieKaban(Point pos) {
	TextureAsset(U"cookieKaban").resized(rect_size).draw(pos * rect_size);
}

void Zerosen(Point pos) {
	TextureAsset(U"zerosen-body").resized(rect_size).draw(pos * rect_size);
	TextureAsset(U"zerosen-pera").resized(rect_size).draw(pos * rect_size);
	TextureAsset(U"zerosen-sentan").resized(rect_size).draw(pos * rect_size);
}

void SnowKnight(Point pos) {
	Rect{ pos * rect_size,rect_size }.draw(Palette::Gray);
}

void ThroughBlock(Point pos) {
	RectF{ pos * rect_size,rect_size,rect_size / 10 }.draw(Palette::Blue);
}

void SlaversCookie(Point pos) {
	RectF{ pos * rect_size,rect_size,rect_size / 10 }.draw(Palette::Yellow);
}

void PoleBlock(Point pos) {
	RectF{ pos * rect_size,rect_size }.draw(Palette::Darkgray);
}

void CaptainBlock(Point pos) {
	RectF{ pos * rect_size,rect_size }.draw(Palette::Lightyellow);
}

void LastBossBlock(Point pos) {
	RectF{ pos * rect_size,rect_size }.draw(Palette::Deeppink);
}

struct Info {
	String tag;
	std::function<void(Point)>func;
	String name = tag;
};

Array<Info>list{
	{U"CakeSurface", CakeSurface,U"ケーキの地表"},
	{U"CakeGround", CakeGround,U"ケーキのスポンジ"},
	{U"CakeRightWall", CakeRightWall,U"ケーキの右壁"},
	{U"CakeLeftWall", CakeLeftWall,U"ケーキの左壁"},
	{U"CakeRightEdge", CakeRightEdge,U"ケーキの右端"},
	{U"CakeLeftEdge", CakeLeftEdge,U"ケーキの左端"},
	{U"ChocolateWall",ChocolateWall,U"チョコレートの壁"},
	{U"Slider",slider,U"スライダー"},
	{U"Moving",moving,U"横に動くチョコレート"},
	{U"Elevator",elevator,U"縦に動くチョコレート"},
	{U"Spring",spring,U"ばね"},
	{U"StrawberrySoldier",StrawberrySoldierBlock,U"イチゴの兵士"},
	{U"CookieSoldier",CookieSoldierBlock,U"クッキーの兵士"},
	{U"IceCream",IceCream,U"アイスクリーム"},
	//{U"LightPinkBlock",LightPinkBlock,U"薄ピンク"},
	{U"Candle",Candle,U"キャンドル"},
	{U"Candy",Candy,U"キャンディー"},
	{U"CandyStick",CandyStick,U"ステック"},
	{U"Cherries",Cherries,U"さくらんぼ"},
	{U"Blueberry",Blueberry,U"ブルーベリー"},
	{U"Hawk",Hawk,U"鷹"},
	{U"Door",Door,U"ドア"},
	{U"WeakWall",WeakWall,U"もろい壁"},
	{U"BeltConveyorRight",BeltConveyorRight,U"ベルトコンベア(→)"},
	{U"BeltConveyorLeft",BeltConveyorLeft,U"ベルトコンベア(←)"},
	{U"SpawnerStrawberrySoldier",SpawnerStrawberrySoldier,U"イチゴスポナー"},
	{U"Player",Player,U"プレイヤー"},
	{U"Snowman",Snowman,U"雪だるま"},
	{U"ItigoSlave",ItigoSlave,U"奴隷イチゴ"},
	{U"CloudEnemy",CloudEnemy,U"わたあめ"},
	{U"Corn",Corn,U"コーン"},
	{U"CookieMuti",CookieMuti,U"鞭クッキー"},
	{U"CookieKaban",CookieKaban,U"鞄クッキー"},
	{U"Zerosen",Zerosen,U"零戦クッキー"},
	{U"SnowKnight",SnowKnight,U"雪だるナイト"},
	{U"ThroughBlock",ThroughBlock,U"すり抜ける床"},
	{U"SlaversCookie",SlaversCookie,U"奴隷商クッキー"},
	{U"PoleBlock",PoleBlock,U"柱ブロック"},
	{U"CaptainBlock",CaptainBlock,U"船長"},
	{U"LastBossBlock",LastBossBlock,U"ラスボス"}
};

HashTable<String, std::function<void(Point)>> table;
Array<String>name;
HashTable<String, String>nameTable;

void setData() {
	name << U"削除";
	nameTable[U"削除"] = U"";
	for (const auto& info : list) {
		name << info.name;
		table[info.tag] = info.func;
		nameTable[info.name] = info.tag;
	}
}
