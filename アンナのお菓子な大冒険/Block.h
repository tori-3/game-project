#pragma once
#include"HitBox.h"
#include"DataManager.h"
#include"setting.h"

#include"RingEffect.h"


class Block {
public:
	Block() {};
	virtual void reaction(const Point& pos, PhysicsBox* box)
	{
		box->hit(Rect{ pos * rect_size,rect_size });
	}

	virtual void update(const Point& pos){}

	virtual void draw(const Point& pos)const {};
};

class Untouchable: public Block {
	void reaction(const Point& pos, PhysicsBox* box)override{}
};

class CakeSurface :public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"CakeSurface").resized(rect_size).draw(pos * rect_size);
	}
};

class CakeGround :public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"CakeGround").resized(rect_size).draw(pos * rect_size);
	}
};

class CakeRightWall :public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"CakeRightWall").resized(rect_size).draw(pos * rect_size);
	}
};

class CakeLeftWall:public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"CakeLeftWall").resized(rect_size).draw(pos * rect_size);
	}
};

class CakeRightEdge :public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"CakeRightEdge").resized(rect_size).draw(pos * rect_size);
	}
};

class CakeLeftEdge :public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"CakeLeftEdge").resized(rect_size).draw(pos * rect_size);
	}
};

class ChocolateWall :public Block
{
public:
	void draw(const Point& pos)const override
	{
		TextureAsset(U"ChocolateWall").resized(rect_size).draw(pos * rect_size);
	}
};


class Slider :public Block
{
public:
	void reaction(const Point& pos, PhysicsBox* box)override
	{
		Point p = pos*rect_size;
		const Triangle triangle{ p, p + Point(0,rect_size), p + Point(rect_size,rect_size) };
		box->hit(triangle);
		if (box->lines(Direction::left).intersects(triangle)) {
			*(box->pos) += Vec2(5, 5);
		}
	}

	void draw(const Point& pos)const override
	{
		Point p = pos * rect_size;
		Triangle{ p, p + Point(0,rect_size), p + Point(rect_size,rect_size) }.draw(Color{ 240 });
	}
};

class Moving :public Block
{
public:
	void reaction(const Point& pos, PhysicsBox* box)override
	{
		RectF rect{ pos * rect_size,rect_size };
		rect.x += Periodic::Triangle0_1(3s, DataManager::get().time) * rect_size * 3;
		if (box->lines(Direction::down).center().intersects(rect)) {
			if (Periodic::Square0_1(3s, DataManager::get().time))box->pos->x += Scene::DeltaTime() / 1.5 * rect_size * 3;
			else box->pos->x -= Scene::DeltaTime() / 1.5 * rect_size * 3;
		}
		box->hit(rect);
	}

	void draw(const Point& pos)const override
	{
		RectF rect{ pos * rect_size,rect_size };
		rect.x += Periodic::Triangle0_1(3s, DataManager::get().time) * rect_size * 3;
		TextureAsset(U"ChocolateWall").resized(rect_size).draw(rect.pos);
	}
};

class Elevator :public Block
{
public:
	void reaction(const Point& pos, PhysicsBox* box)override
	{
		Rect rect{ pos * rect_size,rect_size };
		rect.y += rect_size * 3;
		rect.y -= (int32)(Periodic::Triangle0_1(6s, DataManager::get().time) * rect_size * 5);
		box->hit(rect);

		if (box->lines(Direction::down).intersects(rect) && not Periodic::Square0_1(6s, DataManager::get().time)) {
			box->pos->y += (rect_size * 5)*Scene::DeltaTime() / 3+0.2;
		}
	}
	void draw(const Point& pos)const override
	{
		Rect rect{ pos * rect_size,rect_size };
		rect.y += rect_size * 3;
		rect.y -= (int32)(Periodic::Triangle0_1(6s, DataManager::get().time) * rect_size * 5);
		TextureAsset(U"ChocolateWall").resized(rect_size).draw(rect.pos);
	}
};

class Spring :public Block
{
public:
	class CookieSpring
	{
	public:

		void update(bool pressed)
		{

			for (m_accumulatedTime += Scene::DeltaTime(); 0.005 <= m_accumulatedTime; m_accumulatedTime -= 0.005)
			{
				// ばねの力（変化を打ち消す方向）
				double force = (-0.02 * m_x);

				// 画面を押しているときに働く力
				if (pressed)force += 0.01;

				// 速度に力を適用（減衰もさせる）
				m_velocity = (m_velocity + force) * 0.92;

				// 位置に反映
				m_x += m_velocity;
			}
		}

		double get() const
		{
			return m_x;
		}

	private:

		// ばねの伸び
		double m_x = 0.0;

		// ばねの速度
		double m_velocity = 0.0;

		// ばねの蓄積時間
		double m_accumulatedTime = 0.0;
	}spring;

	bool flg = false;

	void reaction(const Point& pos,PhysicsBox* box)override {
		RectF rect{ pos * rect_size + Vec2(0,spring.get() * rect_size),rect_size,rect_size * (1 - spring.get()) };
		box->hit(rect);
		if (box->lines(Direction::down).intersects(rect)) {
			(box->vel->y) = -Min(Abs(box->vel->y), 800.0);
			flg = true;

			DataManager::get().effect.add<RingEffect>(rect.center());

			AudioAsset{ U"ボヨン" }.playOneShot();
		}

	}

	void update(const Point& pos)override {
		spring.update(flg);
		flg = false;
	}

	void draw(const Point& pos)const override {
		RectF rect{ pos * rect_size + Vec2(0,spring.get() * rect_size),rect_size,rect_size * (1 - spring.get()) };
		rect.draw();
	}
};

class StrawberrySoldierBlock :public Untouchable {
public:

	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"StrawberrySoldier", pos* rect_size + Vec2{ 0.5,0.5 }*rect_size);
			bornFlg = true;
		}
	}

};

class CookieSoldierBlock :public Untouchable {
public:

	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"CookieSoldier", pos * rect_size+Vec2{0.5,0.5}*rect_size);
			bornFlg = true;
		}
	}

};

class Hawk :public Untouchable {
public:
	Hawk() {
		TextureAsset::Register(U"Hawk", U"🦅"_emoji, TextureDesc::Mipped);
	}

	bool touchFlg = false;

	void update(const Point& pos)override {
		if (DataManager::get().playerPos.intersects(Circle{ (pos + Vec2::All(0.5)) * rect_size,rect_size * 2 })) {
			DataManager::get().table.emplace(U"DoorOpen");
			if (not touchFlg) {
				touchFlg = true;
				DataManager::get().talkWindow.addTalk(U"鷹", U"やあそこのお嬢さん。");
				DataManager::get().talkWindow.addTalk(U"鷹", U"上の扉の鍵ならこの崖の向こうにありますよ。");
				DataManager::get().talkWindow.addTalk(U"鷹", U"よければ取って来ましょうか？");
				DataManager::get().talkWindow.addTalk(U"ゲーム", U"本当はここでミニゲームが始める");
				DataManager::get().talkWindow.addTalk(U"ゲーム", U"扉が開きました。");
			}
		}
	}

	void draw(const Point& pos)const override {
		Circle{ (pos + Vec2::All(0.5)) * rect_size,rect_size*2 }.drawFrame(2,Palette::Red);
		TextureAsset(U"Hawk").resized(rect_size * 1.2).draw(pos * rect_size);
	}
};

//class Door :public Untouchable {
//public:
//
//	Timer timer{ 1s };
//
//	void update(const Point& pos)override {
//
//		if (Rect{ pos * rect_size,rect_size * 2 }.intersects(DataManager::get().playerPos)) {
//			timer.start();
//			DataManager::get().table.emplace(U"Clear");
//		}
//	}
//
//	void draw(const Point& pos)const override {
//		Rect{ pos * rect_size,rect_size * 2 }.draw(Palette::Black);
//		double d = Sin(timer.sF() * 80_deg+10_deg);
//		TextureAsset(U"Door").resized(rect_size * 2).scaled(d,1).draw(pos * rect_size);
//		TextureAsset(U"Door").resized(rect_size * 2).scaled(d,1).mirrored().draw(pos * rect_size+Vec2::UnitX()*(1-d)*2* rect_size);
//	}
//
//};

class Door :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"Door", pos * rect_size);
			bornFlg = true;
		}
	}
};

class WeakWall:public Block {
public:
	bool breaked = false;

	void reaction(const Point& pos, PhysicsBox* box)override
	{

		const Rect rect{ pos * rect_size,rect_size };

		if (not breaked) {
			if (box->lines(Direction::left).intersects(rect) || box->lines(Direction::right).intersects(rect)) {
				if (Abs(box->vel->x) > 500) {					
					breaked = true;
					return;
				}
			}
			box->hit(rect);
		}		
	}

	void draw(const Point& pos)const override {
		if (not breaked) {
			TextureAsset(U"ChocolateWall").resized(rect_size).draw(pos * rect_size);
			Rect{ pos * rect_size,rect_size }.draw(ColorF{ Palette::Red,0.3 });
		}
	}
};

class BeltConveyorRight :public Block {
public:

	void reaction(const Point& pos, PhysicsBox* box)override
	{
		RectF rect{ pos * rect_size,rect_size };
		if (rect.stretched(0, 0.1).intersects(box->lines(Direction::down).begin)) {
			box->pos->x += Scene::DeltaTime() * rect_size / 0.5;
		}
		box->hit(rect);
	}

	void draw(const Point& pos)const override {
		const double size = TextureAsset(U"ChocolateWall").size().x;
		const double d = Periodic::Sawtooth0_1(0.5s,DataManager::get().time);
		TextureAsset(U"ChocolateWall")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
		TextureAsset(U"ChocolateWall")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
	}
};

class BeltConveyorLeft :public Block {
public:

	void reaction(const Point& pos, PhysicsBox* box)override
	{
		RectF rect{ pos * rect_size,rect_size };

		if (box->lines(Direction::down).end.intersects(rect.stretched(0,0.1))) {
			box->pos->x -= Scene::DeltaTime() * rect_size / 0.5;
		}

		box->hit(rect);
	}

	void draw(const Point& pos)const override {
		const double size = TextureAsset(U"ChocolateWall").size().x;
		const double d = 1-Periodic::Sawtooth0_1(0.5s, DataManager::get().time);
		TextureAsset(U"ChocolateWall")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
		TextureAsset(U"ChocolateWall")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
	}
};

class SpawnerStrawberrySoldier :public Untouchable {
public:

	double accumlater = 0;

	void update(const Point& pos)override {

		if (not Rect{Arg::center(pos*rect_size),rect_size*5,9999 }.intersects(DataManager::get().playerPos)) {

			constexpr double spawn = 1.0;

			for (accumlater += Scene::DeltaTime(); spawn <= accumlater; accumlater -= spawn) {
				DataManager::get().addEntity(U"SpawnerStrawberrySoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
			}
		}
	}
};

class SnowmanBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"Snowman", pos * rect_size + Vec2{ 1,1 }*rect_size);
			bornFlg = true;
		}
	}
};

class ItigoSlaveBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"ItigoSlave", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};

class CloudEnemyBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"CloudEnemy", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
			bornFlg = true;
		}
	}
};

class CornBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"Corn", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
			bornFlg = true;
		}
	}
};

class CookieMutiBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"CookieMuti", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};

class CookieKabanBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"CookieKaban", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};

class ZerosenBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"Zerosen", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};

class SnowKnightBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"SnowKnight", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};

class SlaversCookieBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"SlaversCookie", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};


class ThroughBlock :public Block {
public:
	void reaction(const Point& pos, PhysicsBox* box)override
	{
		//ヘッドドロップのときはすり抜ける(速度で判定)
		if (not(1150<(box->vel->y))) {
			box->hitDown(Rect{ pos * rect_size,rect_size,20 });
		}
	}

	virtual void draw(const Point& pos)const override {

		Rect{ pos * rect_size,rect_size,20 }.draw(Palette::Blue);
	}
};

class PoleBlock :public Block {

	void reaction(const Point& pos, PhysicsBox* box)override
	{
		RectF rect{ pos * rect_size ,rect_size };
		if (box->lines(Direction::right).intersects(rect)and 600<box->vel->x) {
			DataManager::get().table.emplace(U"PoleHit");
			DataManager::get().table.emplace(U"ShakeCamera");
		}
		else if (box->lines(Direction::left).intersects(rect) and box->vel->x < -600) {
			DataManager::get().table.emplace(U"PoleHit");
			DataManager::get().table.emplace(U"ShakeCamera");
		}

		box->hit(rect);
	}

	void draw(const Point& pos)const override{
		RectF{ pos * rect_size ,rect_size }.draw(Palette::Darkgray);
	};

};

class CaptainBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override {
		if (not bornFlg) {
			DataManager::get().addEntity(U"Captain", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
			bornFlg = true;
		}
	}
};


//背景専門

class IceCream :public Untouchable {
public:
	void draw(const Point& pos)const override {
		TextureAsset(U"IceCream").resized(rect_size * 2).draw(pos * rect_size);
	};
};

class Candle :public Untouchable {
public:
	void draw(const Point& pos)const override {
		TextureAsset(U"Candle").resized(rect_size * 2).draw(pos * rect_size - Vec2::UnitX() * rect_size / 2);
		TextureAsset(U"fire").resized(rect_size).draw(pos * rect_size - Vec2::UnitY() * rect_size / 2);
	};
};

class Candy :public Untouchable {
public:
	void draw(const Point& pos)const override {
		TextureAsset(U"Candy").resized(rect_size * 2.5).rotated(15_deg).draw(pos * rect_size);
	};
};

class CandyStick :public Untouchable {
public:
	void draw(const Point& pos)const override {
		TextureAsset(U"CandyStick").resized(rect_size * 2.2).draw(pos * rect_size);
	};
};

class Cherries :public Untouchable {
public:
	void draw(const Point& pos)const override {
		TextureAsset(U"Cherries").resized(rect_size * 1.3).draw(pos * rect_size);
	};
};

class Blueberry :public Untouchable {
public:
	void draw(const Point& pos)const override {
		TextureAsset(U"Blueberry").resized(rect_size * 1.3).draw(pos * rect_size);
	};
};
