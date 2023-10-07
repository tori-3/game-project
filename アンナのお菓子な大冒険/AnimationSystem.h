#pragma once

#include<Siv3D.hpp>

class Movable {
public:
	Vec2 pos;
	SizeF size;
	double angle;
	double z;
	Vec2 rotatePos;
	ColorF color;
	bool mirror;

	Movable(const Vec2& pos, const SizeF& size, const Vec2& rotatePos = {}, double angle = 0_deg, double z = 0, const ColorF& color = ColorF{ 1 }, bool mirror = false)
		:pos{ pos }, size{ size }, angle{ angle }, z{ z }, rotatePos{ rotatePos }, color{ color }, mirror{ mirror } {}

	Movable() {}

	virtual void draw()const = 0;

	virtual void drawDebug()const {}
};

class Character;


class Move {
public:
	Move() {}

	virtual void start(Character* character) {};

	virtual void update(Character* character, double t = Scene::DeltaTime()) = 0;

	virtual bool isActive() = 0;
};

class Motion :public Move {
public:

	Array<std::shared_ptr<Move>>list;

	double speed = 1;
	bool parallel = false;
	bool loop = false;

	Motion(bool parallel = false, bool loop = false)
		:parallel{ parallel }, loop{ loop } {}

	void clear() {
		list.clear();
		index = 0;
		active = false;
		speed = 1;
		parallel = false;
		loop = false;
	}

	void setSpeed(double _speed) {
		speed = _speed;
	}

	void add(Move* move) {
		list << std::shared_ptr<Move>(move);
	}

	void start(Character* character)override {
		index = 0;
		if (parallel) {
			for (auto& move : list) {
				move->start(character);
			}
		}
		else {
			if (list)list[0]->start(character);
		}
	}

	void update(Character* character, double t = Scene::DeltaTime())override {

		if (parallel) {

			active = false;

			for (auto& move : list) {
				if (move->isActive()) {
					move->update(character, t * speed);
					active = true;
				}
			}
		}
		else {
			if (checkIndex()) {
				list[index]->update(character, t * speed);
				if (not list[index]->isActive()) {
					index++;
					if (checkIndex()) {
						list[index]->start(character);
					}
					//終了なら
					else {
						active = false;
					}
				}
			}
		}

		if ((not active) && loop) {
			start(character);
			active = true;
		}
	}

	bool isActive()override {
		return active;
	}

private:
	size_t index = 0;
	bool active = true;

	bool checkIndex() {
		return index < list.size();
	}
};

class DrawManager {
public:

	Array<Movable*>movables;

	DrawManager() {}

	DrawManager(const Array<Movable*>& movables) :movables{ movables } {}

	void add(Movable* movable) {
		movables << movable;
	}

	void update() {
		movables.stable_sort_by([](Movable* left, Movable* right) {return left->z < right->z; });
	}

	void draw()const {
		movables.each([](Movable* movable) {movable->draw(); });
	}

	void drawDebug()const {
		movables.each([](Movable* movable) {movable->drawDebug(); });
	}

	void remove(Movable* movable) {
		movables.remove(movable);
	}

	void clear() {
		movables.clear();
	}

};

class Joint :public Movable {
public:
	String textureName;

	Array<Joint*>joints;

	Mat3x2 mat = Graphics2D::GetLocalTransform();

	Joint() {}

	Joint(const Vec2& center, const Vec2& rotatePos, const String& textureName, double z, double scale)
		:Movable{ center,TextureAsset{textureName}.size() * scale,rotatePos,0_deg,z }, textureName{ textureName } {}

	void add(Joint* joint) {
		joints << joint;
	}
	//自分と子ジョイントをすべて返す
	Array<Joint*> getAll()
	{
		Array<Joint*>result;
		result << this;
		for (auto& joint : joints)
		{
			result.append(joint->getAll());
		}
		return result;
	}

	//Moveのあと
	void update() {
		const Transformer2D t1{ Mat3x2::Translate(pos) };//中心をずらす
		const Transformer2D t2 = Transformer2D{ Mat3x2::Rotate(angle,rotatePos) };// { Mat3x2::Identity() };
		mat = Graphics2D::GetLocalTransform();//変換行列を保存
		for (auto& joint : joints) {
			joint->update();
		}
	}

	void draw()const override {
		Transformer2D t{ mat };
		TextureAsset{ textureName }.resized(size).drawAt({ 0,0 }, color);
	}

	void drawDebug()const override {
		Transformer2D t{ mat };
		(rotatePos).asCircle(2).draw(Palette::Red);
		RectF{ Arg::center(0,0),size }.drawFrame(1, Palette::Red);
	}

	double getMaxY() {
		RectF rect{ Arg::center(0,0),size };
		double max = -Math::Inf;
		for (auto i : step(4)) {
			max = Max(mat.transformPoint(rect.point(i)).y, max);
		}
		return max;
	}
};

class Character {
public:

	std::unique_ptr<Character> base = nullptr;
	//Hitboxパーツがある場合はこれがtrueになる
	bool hasHitboxParts = false;

	class Body {
	public:
		Joint joint;
		String parentName;

		Body(const Joint& joint, const String& parentName) :joint{ joint }, parentName{ parentName } {}

		Body() {}
	};

	//Array<Motion>motions;

	HashTable<String, Motion>motionTable;

	double angle = 0_deg;
	bool mirror = false;
	double scale = 1.0;
	//first->name second->joint,parentName
	HashTable<String, Body>table;

	Joint* joint = nullptr;

	double nowMaxY = 0;

	Character(double scale = 1.0) :scale{ scale } {}

	Character(const JSON& json, double scale = 1.0) :scale{ scale } {
		for (const auto& body : json[U"Body"])
		{
			String name = body.key;
			if (name == U"Hitbox")hasHitboxParts = true;;
			const auto& object = json[U"Body"][name];
			set(name, object[U"Parent"].getString(), object[U"Position"].get<Vec2>(), object[U"RotateCenter"].get<Vec2>(), object[U"TexturePath"].getString(), object[U"Z"].get<double>(), object[U"Scale"].get<double>());
		}
		complete();
		setBase();
	}

	void set(const String& name, const String& parentName, const Vec2& center, const Vec2& rotatePos, const String& textureName, double z, double scale) {
		TextureAsset::Register(textureName, textureName);
		table[name] = Body{ Joint{center, rotatePos, textureName,z,scale},parentName };
	}

	void complete() {
		for (auto it = table.begin(); it != table.end(); ++it)
		{
			if (it->second.parentName == U"__Main__") {
				joint = &(it->second.joint);
			}
			else {
				if (table.contains(it->second.parentName)) {
					table[it->second.parentName].joint.add(&(it->second.joint));
				}
			}
		}
	}

	void removeDrawManager(DrawManager* manager) {

		for (auto it = table.begin(); it != table.end(); ++it)
		{
			manager->remove(&it->second.joint);
		}
	}

	void setBase() {
		base = std::make_unique<Character>();
		base->angle = angle;
		base->mirror = mirror;
		base->scale = scale;
		base->table = table;
		for (auto& joint : base->table)
		{
			joint.second.joint.joints.clear();
		}
		base->complete();
	}

	Character* getBase() {
		return base.get();
	}

	void update(double dt = Scene::DeltaTime()) {
		if (KeyZ.pressed())for (auto& joint : getBase()->get(U"base-body")->getAll())Print << joint->size;
		for (auto it = motionTable.begin(); it != motionTable.end();)
		{
			if (not it->second.isActive())
			{
				it = motionTable.erase(it);
			}
			else
			{
				it->second.update(this, dt);
				++it;
			}
		}

		Transformer2D trans{ Mat3x2::Scale(Cos(angle) * scale,scale,joint->pos) };
		joint->update();


		nowMaxY += ((getMaxY()-joint->pos.y) - nowMaxY) / (100);
	}

	void addMotion(const String&name, const Motion& motion) {
		motionTable[name] = motion;
		motionTable[name].start(this);
	}

	void removeMotion(const String& name) {
		motionTable.erase(name);
	}

	bool hasMotion(const String& name) {
		return motionTable.contains(name);
	}

	void clearMotion() {
		motionTable.clear();
	}

	Joint* get(const String& name) {
		return &table[name].joint;
	}

	void setDrawManager(DrawManager* manager) {
		for (auto it = table.begin(); it != table.end(); ++it)
		{
			manager->add(&it->second.joint);
		}
	}

	void touchGround(double y) {
		addPos({0,y - getMaxY()});
	}

	void setPos(const Vec2& pos)
	{
		joint->pos = pos;
		joint->update();
	}

	void addPos(const Vec2& pos) {
		joint->pos += pos;
		joint->update();
	}

	double getMaxY() {
		double max = -Math::Inf;
		for (auto it = table.begin(); it != table.end(); ++it)
		{
			if ((not hasHitboxParts) or it->first == U"Hitbox")
				max = Max(it->second.joint.getMaxY(), max);
		}
		return max;
	}
};

class TimeMove :public Move {
public:
	double time = 0;
	double timelim;

	TimeMove(double timelim = 1) : timelim{ timelim } {}

	virtual void start(Character* character)override {
		time = 0;
	}

	virtual void update(Character* character, double t = Scene::DeltaTime())override = 0;

	double calTime(double dt) {
		//もし制限時間を超えるなら
		if (timelim < time + dt) {
			dt = timelim - time;
			time = timelim;
		}
		else {
			time += dt;
		}
		return dt;
	}

	virtual bool isActive() {
		return time < timelim;
	}
};
