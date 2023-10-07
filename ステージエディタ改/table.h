﻿#pragma once
# include <Siv3D.hpp>
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

void LightPinkBlock(Point pos) {
	Rect{ pos * rect_size,rect_size }.draw(ColorF{ Palette::Lightpink }*1.2);
}

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
	{U"LightPinkBlock",LightPinkBlock,U"薄ピンク"},
	{U"Candle",Candle,U"キャンドル"},
	{U"Candy",Candy,U"キャンディー"},
	{U"CandyStick",CandyStick,U"ステック"},
	{U"Cherries",Cherries,U"さくらんぼ"},
	{U"Blueberry",Blueberry,U"ブルーベリー"},
	{U"Hawk",Hawk,U"鷹"},
	{U"Door",Door,U"ドア"}
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
