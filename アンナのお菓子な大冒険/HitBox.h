#pragma once
#include<Siv3D.hpp>
#include"Figure.hpp"
#include"setting.h"

enum class Direction { up, down, right, left };

//ステージとの当たり判定に必要なクラス（このクラスをHitBoxに持たせる）
class PhysicsBox {
public:
	Vec2* pos, * vel;
	bool touch_flag[4] = { 0 };
	double width, height;
	Vec2 delta;//座表と左上の座標との差
	int32* hp;

	double acc1=0;

	bool leftFloor=false, rightFloor=false;

	PhysicsBox(Vec2* pos, Vec2* vel, Vec2 delta, double width, double height, int32* hp) :pos(pos), vel(vel), delta(delta), width(width), height(height), hp{hp} {}

	//それぞれの方向のshift関数を呼び出す
	template<typename T>void hit(T rect) {
		shift(rect, Direction::up, Vec2{ 0,0.1 });
		shift(rect, Direction::down, Vec2{ 0,-0.1 });
		shift(rect, Direction::right, Vec2{ -0.1,0 });
		shift(rect, Direction::left, Vec2{ 0.1,0 });

			const Vec2 p0 = *pos + delta;
			if (rect.intersects(Vec2{ p0.x, p0.y + height+20 })) {
				leftFloor = true;
			}
			if (rect.intersects(Vec2{ p0.x + width, p0.y + height+20 })) {
				rightFloor = true;
			}

	}

	//図形の埋め込みを解消する
	template<typename T>void shift(T rect, Direction num, Vec2 vec)
	{
		if (not rect.intersects(lines(num)))return;//Lineと当たっていなければ何もしない
		while (rect.intersects(lines(num)))*pos += vec;//Lineと当たらなくなるまで移動
		*pos -= vec;//少し戻す（これをしないと、ブロックと当たる、離れるを繰り返してしまう。）
		touch_flag[(int32)num] = true;//その方向の
	}

	//それぞれの方向のLineを返す
	Line lines(Direction num)
	{
		Vec2 p0 = *pos + delta;
		double d = 10;
		switch (num) {
		case Direction::up:return 	Line(p0 + Vec2(0 + d, 0), p0 + Vec2(width - d, 0));
		case Direction::down:return 	Line(p0.x + d, p0.y + height, p0.x + width - d, p0.y + height);
		case Direction::right:return 	Line(p0 + Vec2(width, 0 + d), p0 + Vec2(width, height - d));
		case Direction::left:return 	Line(p0 + Vec2(0, 0 + d), p0 + Vec2(0, height - d));
		}

		//使わないけど警告防止用
		return Line{};
	}

	void resetFlg() {
		touch_flag[0] = touch_flag[1] = touch_flag[2] = touch_flag[3] = 0;
		leftFloor = rightFloor = false;
	}

	void physicsUpdate() {

		if (touch_flag[0] && vel->y < 0)vel->y = 0;//天井に衝突したらy軸速度を0に
		if (touch_flag[1] && vel->y > 0)vel->y = 0;//床に衝突したらy軸速度を0に

		//速度分、座標を動かす
		double span = 0.01;
		for (acc1 += Scene::DeltaTime(); span <= acc1; acc1 -= span) {
			*pos += (*vel) * span;
			if (not touch_flag[1])vel->y += gravity;

			vel->x *= 0.8;//x軸方向の速度を減速
			//vel->x = 0;
		}
	}

};

class HitBox
{
public:
	HitBox(Vec2* Pos, Vec2* vel, const Figure& Hitbox,int32* hp)
		:hitbox(Hitbox), pos(Pos), PrePos(*Pos), Relative(Hitbox.center()), physics(NULL, NULL, Vec2{ 0,0 }, 0, 0,hp)
	{
		const RectF rect = hitbox.boundingRect();
		hitbox.setCenter(*pos + Relative + (hitbox.center() - rect.center()));
		physics = PhysicsBox{ pos, vel, Relative -Vec2{ rect.w / 2,rect.h / 2 }, rect.w, rect.h,hp };
	}

	virtual ~HitBox() {}

	void setFigure(const Figure& _figure) {
		const RectF rect = _figure.boundingRect();
		Relative = _figure.center();
		hitbox = _figure;
		hitbox.setCenter(*pos + Relative + (hitbox.center() - rect.center()));
		physics.width = rect.w;
		physics.height = rect.h;
		physics.delta = Relative - Vec2{ rect.w / 2,rect.h / 2 };

		//physics = PhysicsBox{ pos, vel, Relative - Vec2{ rect.w / 2,rect.h / 2 }, rect.w, rect.h,hp };
	}

	Figure getFigure() {
		return hitbox;
	}

	bool intersects(HitBox target)const
	{
		return target.hitbox.intersects(hitbox);
	}

	void update(){
		hitbox.moveBy(*pos - PrePos);
		PrePos = *pos;
	}

	void draw(ColorF color)const
	{
		hitbox.draw(color);
	}

	void setPos(Vec2 position) {
		hitbox.setCenter(position);
	}

	void physicsUpdate() {
		physics.physicsUpdate();
	}

	Figure Get_Box()const { return hitbox; }

	void SetShape(Figure box, Vec2 relative = Vec2{ 0,0 }) {
		Relative = relative;
		hitbox = box;
		const RectF rect = hitbox.boundingRect();
		hitbox.setCenter(*pos + Relative + (hitbox.center() - rect.center()));
		physics.delta = -Vec2{ rect.w / 2,rect.h / 2 };
		physics.width = rect.w; physics.height = rect.h;
	}

	bool touch(Direction num)const { return physics.touch_flag[(int32)num]; }

	bool touchAny() {
		return touch(Direction::up) or touch(Direction::down) or touch(Direction::right) or touch(Direction::left);
	}

	bool leftFloor() {
		return physics.leftFloor;
	}

	bool rightFloor() {
		return physics.rightFloor;
	}

	

	//ステージから影響を受ける
	template<typename T>void hit(T figure) { physics.hit(figure); }

	Vec2* pos, PrePos, Relative;
	PhysicsBox  physics;
	Figure hitbox;

protected:
};
