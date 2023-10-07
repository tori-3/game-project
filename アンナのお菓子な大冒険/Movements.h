#pragma once
#include"AnimationSystem.h"

class MotionLoader;

//設定した角度回転
class Rotate :public TimeMove {
public:
	double rad;
	String target;

	Rotate(const String target, double rad, double timelim) :TimeMove{ timelim }, target{ target }, rad{ rad } {}

	virtual ~Rotate() {}

	void update(Character* character, double dt = Scene::DeltaTime())override {
		if (timelim == 0)return;

		dt = calTime(dt);
		if (character->get(target) == nullptr)return;
		character->get(target)->angle += dt * rad / timelim;
	}
};
//0~360
inline double seikika(double theta) {
	if (0 <= theta)return Fmod(theta, 360_deg);//正規化
	else return 360_deg + Fmod(theta, 360_deg);
}
/// @brief 符号判定
/// @param A 
/// @return Aが負なら-1　Aが正なら+1
inline double sign(double A) {
	return (A > 0) - (A < 0);
}

class RotateTo :public Rotate {
	int32 direction;
	int32 baseDirection;
	double baseRad;
public:
	RotateTo(const String& target, double angle, double timelim, bool clockWise) :Rotate{ target,seikika(angle),timelim }
	{
		baseRad = rad;
		if (clockWise)baseDirection = 1; else baseDirection = -1;
	}
	RotateTo(const String& target, double angle, double timelim) :Rotate{ target,seikika(angle) ,timelim }
	{
		baseRad = rad;
		baseDirection = 0;
	}
	void start(Character* character)override {
		Rotate::start(character);
		//ループしたとき、radの値が変わるため、baseRadで修正
		rad = baseRad;
		direction = baseDirection;

		//character->get(target)->angle = rad;
		if (character->get(target) == nullptr)return;

		if (timelim == 0)character->get(target)->angle = rad;

		double delta = rad - seikika(character->get(target)->angle);
		if (direction == 0)
		{
			abs(delta) > 180_deg ? direction = sign(delta) * (-1) : direction = sign(delta);
		}

		if (direction > 0)
		{
			//時計回り
			delta < 0 ? rad = 360_deg + delta : rad = delta;
		}
		else
		{
			//反時計回り
			delta > 0 ? rad = delta - 360_deg : rad = delta;
		}
	}
	//Rotateのupdateを使う
};
/// @brief 等速直線で移動する
class Translate :public TimeMove
{
public:
	Vec2 dp;
	String target;

	Translate(const String& target, const Vec2& deltaPos, double time)
		:TimeMove{ time }, target(target), dp(deltaPos)
	{

	};

	void start(Character* character)override
	{
		TimeMove::start(character);
		if (timelim == 0)
		{
			character->get(target)->pos += dp;
			return;
		}
		//character->get(target)->pos = { 100,100 };
	}

	void update(Character* character, double dt = Scene::DeltaTime())override {
		dt = calTime(dt);
		if (timelim == 0)return;
		character->get(target)->pos += dt * dp / timelim;
	}
};

class SetPos :public Translate
{
public:
	const Vec2 pos;
	SetPos(const String& target, Vec2 pos, double time)
		:Translate(target, { 0,0 }, time), pos(pos)
	{}

	void start(Character* character)override
	{
		TimeMove::start(character);
		if (timelim == 0)
		{
			character->get(target)->pos = pos;
			return;
		}
		dp = pos - character->get(target)->pos;
	}
};

class ChangeTexture :public TimeMove
{
public:
	String path;
	String target;

	ChangeTexture(const String& target, const String& path, double time = 0)
		:TimeMove(time), target(target), path(path)
	{
		TextureAsset::Register(path, path);
	}

	void start(Character* character)override
	{
		TimeMove::start(character);
		if (timelim == 0)
		{
			character->get(target)->textureName = path;
		}
	}

	void update(Character* character, double dt = Scene::DeltaTime())override
	{
		dt = calTime(dt);
		if (not isActive())character->get(target)->textureName = path;
	}
};

class ChangeColor :public TimeMove
{
public:
	String target;
	const ColorF color;
	HashTable<Joint*, ColorF> start_color;
	const bool following;
	ChangeColor(const String& target, const ColorF& color, double time, bool following = true)
		:TimeMove(time), color(color), target(target), following(following)
	{}

	//ChangeColor(const String& target,String )

	void start(Character* character)override
	{
		TimeMove::start(character);
		start_color.clear();
		Array<Joint*> targets;
		if (following)
		{
			targets = character->get(target)->getAll();
		}
		else
		{
			targets << character->get(target);
		}
		for (auto& joint : targets)
		{
			start_color.emplace(joint, joint->color);
		}
	}

	void update(Character* character, double dt = Scene::DeltaTime())override
	{
		dt = calTime(dt);
		for (auto& joint_color : start_color)
		{
			joint_color.first->color = joint_color.second.lerp(color, time / timelim);
		}
	}
};

class ChangeScale :public TimeMove
{
public:
	String target;
	const SizeF scale;
	HashTable<Joint*, SizeF> d_scale;
	HashTable<Joint*, SizeF> d_position;
	HashTable<Joint*, SizeF> d_center;

	bool following;
	ChangeScale(const String& target, double x_scale, double y_scale, double time, bool following = true)
		:TimeMove(time), scale(SizeF{ x_scale,y_scale }), target(target), following(following)
	{}

	//ChangeColor(const String& target,String )

	void start(Character* character)override
	{
		TimeMove::start(character);
		d_scale.clear();
		d_position.clear();
		d_center.clear();

		Array<Joint*> targets;
		if (following)
		{
			targets = character->get(target)->getAll();
		}
		else
		{
			targets << character->get(target);
		}
		for (auto& joint : targets)
		{
			d_scale.emplace(joint, joint->size * scale - joint->size);
			d_center.emplace(joint, joint->rotatePos * scale - joint->rotatePos);
			d_position.emplace(joint, joint->pos * scale - joint->pos);
		}
		if (targets[0] == character->joint)
			d_position[targets[0]] = 2 * targets[0]->rotatePos * (SizeF{ 1,1 } - scale);
		else
			d_position[targets[0]] = targets[0]->rotatePos - targets[0]->rotatePos * scale;

		//d_position[targets[0]] = { 0,0 };
		if (timelim == 0)
		{
			for (auto& joint_scale : d_scale)
			{
				joint_scale.first->size += joint_scale.second;
			}
			for (auto& joint_position : d_position)
			{
				joint_position.first->pos += joint_position.second;
			}
			for (auto& joint_center : d_center)
			{
				joint_center.first->rotatePos += joint_center.second;
			}
		}
	}

	void update(Character* character, double dt = Scene::DeltaTime())override
	{
		dt = calTime(dt);
		if (timelim == 0)return;
		for (auto& joint_scale : d_scale)
		{
			joint_scale.first->size += (dt / timelim) * joint_scale.second;
		}
		for (auto& joint_scale : d_position)
		{
			joint_scale.first->pos += (dt / timelim) * joint_scale.second;
		}
		for (auto& joint_scale : d_center)
		{
			joint_scale.first->rotatePos += (dt / timelim) * joint_scale.second;
		}
	}
};

class BaseChangeScale :public TimeMove
{
public:
	String target;
	const SizeF scale;
	HashTable<Joint*, SizeF> d_scale;
	HashTable<Joint*, SizeF> d_position;
	HashTable<Joint*, SizeF> d_center;

	const bool following;
	BaseChangeScale(const String& target, double x_scale, double y_scale, double time, bool following = true)
		:TimeMove(time), scale(SizeF{ x_scale,y_scale }), target(target), following(following)
	{}

	void start(Character* character)override
	{
		TimeMove::start(character);

		d_scale.clear();
		d_position.clear();
		d_center.clear();

		Array<Joint*> targets;
		Array<Joint*> baseTargets;
		if (following)
		{
			targets = character->get(target)->getAll();
			baseTargets = character->getBase()->get(target)->getAll();
		}
		else
		{
			targets << character->get(target);
			baseTargets << character->getBase()->get(target);
		}

		for (auto i : step(size(targets))) {
			d_scale.emplace(targets[i], baseTargets[i]->size * scale - targets[i]->size);
			d_center.emplace(targets[i], baseTargets[i]->rotatePos * scale - targets[i]->rotatePos);
			d_position.emplace(targets[i], baseTargets[i]->pos * scale - targets[i]->pos);
		}

		if (targets[0] == character->joint)
			d_position[targets[0]] = 2 * (targets[0]->rotatePos - baseTargets[0]->rotatePos * scale);
		else
			d_position[targets[0]] = targets[0]->rotatePos - baseTargets[0]->rotatePos * scale;

		if (timelim == 0)
		{
			for (auto& joint_scale : d_scale)
			{
				joint_scale.first->size += joint_scale.second;
			}
			for (auto& joint_position : d_position)
			{
				joint_position.first->pos += joint_position.second;
			}
			for (auto& joint_center : d_center)
			{
				joint_center.first->rotatePos += joint_center.second;
			}
		}
	}

	void update(Character* character, double dt = Scene::DeltaTime())override
	{
		dt = calTime(dt);
		if (timelim == 0)return;
		for (auto& joint_scale : d_scale)
		{
			joint_scale.first->size += (dt / timelim) * joint_scale.second;
		}
		for (auto& joint_scale : d_position)
		{
			joint_scale.first->pos += (dt / timelim) * joint_scale.second;
		}
		for (auto& joint_scale : d_center)
		{
			joint_scale.first->rotatePos += (dt / timelim) * joint_scale.second;
		}
	}
};

class AddZ :public Move
{
public:
	const double d;
	const bool following;
	const String target;

	AddZ(const String& target, double delta, bool following = true)
		:target(target), d(delta), following(following) {};

	void start(Character* character)override
	{
		Array<Joint*> joints;
		following ? joints.append(character->get(target)->getAll()) : joints << character->get(target);
		for (auto& joint : joints)joint->z += d;
	};

	void update(Character* character, double dt = Scene::DeltaTime())override {}

	bool isActive()override { return false; }
};

class TransformToBase :public Move
{
public:
	String target;
	//Array<TimeMove*>m_movements;
	//RotateTo,SetPosがfollowingの選択に対応してないので,とりあえず後続に対しても影響を与える仕様にする
	TransformToBase(String target)
		:target(target)
	{

	}

	void start(Character* character)override
	{
		/*for (auto& move : m_movements)
		{
			delete move;
		}
		m_movements.clear();*/

		Array<String> jointNames;
		HashSet<Joint*>joints;
		for (auto& joint : character->get(target)->getAll())joints.emplace(joint);

		for (auto& joint : character->table)
		{
			//名前を得る
			if (joints.contains(&joint.second.joint) or character->joint == &joint.second.joint)jointNames << joint.first;
		}
		for (auto& name : jointNames)
		{
			const Joint* baseJoint = character->getBase()->get(name);
			/*m_movements << new SetPos(name, baseJoint->pos, timelim);
			m_movements << new RotateTo(name, baseJoint->angle, timelim);
			m_movements << new BaseChangeScale(name, 1, 1, timelim,false);
			m_movements << new ChangeColor(name, character->getBase()->get(target)->color, timelim,false);
			m_movements << new ChangeTexture(name, baseJoint->textureName, timelim);*/
			character->get(name)->size = baseJoint->size;
			character->get(name)->angle = baseJoint->angle;
			character->get(name)->pos = baseJoint->pos;
			character->get(name)->rotatePos = baseJoint->rotatePos;
			character->get(name)->textureName = baseJoint->textureName;
			character->get(name)->z = baseJoint->z;
		}
		//for (auto& move : m_movements)move->start(character);
	}

	void update(Character* character, double dt = Scene::DeltaTime())override
	{
		//dt = calTime(dt);
		//for (auto& move : m_movements)move->update(character, dt);
	}
	bool isActive()override { return false; }
};

class Mirror :public TimeMove {
public:
	double rate;
	int32 mirror;
	bool trueMirror = false;
	/// @brief 
	/// @param mirror -1左　0反転　1右
	/// @param rate 
	Mirror(int32 mirror, double rate = 360_deg) :TimeMove{ 180_deg / rate }, rate{ rate }, mirror{ mirror } {
	}

	void start(Character* character)override {
		TimeMove::start(character);

		if (mirror == 0) {
			character->mirror = not character->mirror;
			trueMirror = not character->mirror;
		}
		else if (0 < mirror) {
			character->mirror = false;
			trueMirror = false;
		}
		else {
			character->mirror = true;
			trueMirror = true;
		}
	}

	void update(Character* character, double dt = Scene::DeltaTime())override {
		dt = calTime(dt);
		if (trueMirror) {
			character->angle += dt * rate;
		}
		else {
			character->angle -= dt * rate;
		}
	}
};

//設定した秒数待つ
class Wait :public TimeMove {
public:
	Wait(double timelim) :TimeMove{ timelim } {}

	void update(Character* character, double dt = Scene::DeltaTime())override {
		dt = calTime(dt);
	}
};

class StartMotion :public TimeMove {
public:
	const CSV csv;
	const String motionName;
	Motion motion;
	const double timeScale;
	const bool loop;

	StartMotion(const CSV& csv, const String& motionName, bool loop, double timeScale, double time = Math::Inf)
		:csv(csv), motionName(motionName), timeScale(timeScale), TimeMove(time), loop(loop)
	{}

	void start(Character* character)override;

	void update(Character* character, double dt = Scene::DeltaTime())override;

	bool isActive()override;
};

class SetRad :public Move
{
public:
	String target;
	double rad;
	SetRad(const String& target, double rad)
		:target(target), rad(rad)
	{}
	void start(Character* character)override
	{
		String joint = character->table[target].parentName;
		double relative_rad = 0;
		while (joint != U"__Main__")
		{
			relative_rad += character->table[joint].joint.angle;
			joint = character->table[joint].parentName;
		}
		character->get(target)->angle = rad - relative_rad;
	}
	void update(Character* character, double dt = Scene::DeltaTime())override {};

	bool isActive()override { return false; };
};
