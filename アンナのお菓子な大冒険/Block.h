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
	void draw(const Point& pos)const override;
};

class CakeGround :public Block
{
public:
	void draw(const Point& pos)const override;
};

class CakeRightWall :public Block
{
public:
	void draw(const Point& pos)const override;
};

class CakeLeftWall:public Block
{
public:
	void draw(const Point& pos)const override;
};

class CakeRightEdge :public Block
{
public:
	void draw(const Point& pos)const override;
};

class CakeLeftEdge :public Block
{
public:
	void draw(const Point& pos)const override;
};

class ChocolateWall :public Block
{
public:
	void draw(const Point& pos)const override;
};

class Slider :public Block
{
public:
	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
};

class Moving :public Block
{
public:
	void reaction(const Point& pos, PhysicsBox* box);

	void draw(const Point& pos)const;
};

class Elevator :public Block
{
public:
	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
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

	void update(const Point& pos)override;
};

class CookieSoldierBlock :public Untouchable {
public:

	bool bornFlg = false;

	void update(const Point& pos)override;
};

class Door :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class WeakWall:public Block {
public:
	bool breaked = false;

	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
};

class BeltConveyorRight :public Block {
public:

	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
};

class BeltConveyorLeft :public Block {
public:

	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
};

class SpawnerStrawberrySoldier :public Untouchable {
public:

	double accumlater = 0;

	void update(const Point& pos)override;
};

class SnowmanBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class ItigoSlaveBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class CloudEnemyBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class CornBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class CookieMutiBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class CookieKabanBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class ZerosenBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class SnowKnightBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class SlaversCookieBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class ThroughBlock :public Block {
public:
	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
};

class PoleBlock :public Block {

	void reaction(const Point& pos, PhysicsBox* box)override;

	void draw(const Point& pos)const override;
};

class CaptainBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};

class LastBossBlock :public Untouchable {
public:
	bool bornFlg = false;

	void update(const Point& pos)override;
};


//背景専門

class IceCream :public Untouchable {
public:
	void draw(const Point& pos)const override;
};

class Candle :public Untouchable {
public:
	void draw(const Point& pos)const override;
};

class Candy :public Untouchable {
public:
	void draw(const Point& pos)const override;
};

class CandyStick :public Untouchable {
public:
	void draw(const Point& pos)const override;
};

class Cherries :public Untouchable {
public:
	void draw(const Point& pos)const override;
};

class Blueberry :public Untouchable {
public:
	void draw(const Point& pos)const override;
};

class FallingRocksBlock :public Untouchable {
public:

	bool fallingFlg = false;

	void update(const Point& pos)override;

	void draw(const Point& pos)const override;
};

class RollingRocksBlock :public Untouchable {
public:

	bool bornFlg = false;

	void update(const Point& pos)override;
};
