#include"Block.h"

void CakeSurface::draw(const Point& pos)const
{
	TextureAsset(U"CakeSurface").resized(rect_size).draw(pos * rect_size);
}

void CakeGround::draw(const Point& pos)const {
	TextureAsset(U"CakeGround").resized(rect_size).draw(pos* rect_size);
}

void CakeRightWall::draw(const Point& pos)const
{
	TextureAsset(U"CakeRightWall").resized(rect_size).draw(pos * rect_size);
}

void CakeLeftWall::draw(const Point& pos)const
{
	TextureAsset(U"CakeLeftWall").resized(rect_size).draw(pos * rect_size);
}

void CakeRightEdge::draw(const Point& pos)const
{
	TextureAsset(U"CakeRightEdge").resized(rect_size).draw(pos * rect_size);
}

void CakeLeftEdge::draw(const Point& pos)const
{
	TextureAsset(U"CakeLeftEdge").resized(rect_size).draw(pos * rect_size);
}

void ChocolateWall::draw(const Point& pos)const
{
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(pos * rect_size);
}

void Slider::reaction(const Point& pos, PhysicsBox* box)
{
	Point p = pos * rect_size;
	const Triangle triangle{ p, p + Point(0,rect_size), p + Point(rect_size,rect_size) };
	box->hit(triangle);
	if (box->lines(Direction::left).intersects(triangle)) {
		*(box->pos) += Vec2(5, 5);
	}
}

void Slider::draw(const Point& pos)const
{
	Point p = pos * rect_size;
	Triangle{ p, p + Point(0,rect_size), p + Point(rect_size,rect_size) }.draw(Color{ 240 });
}

void Moving::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size,rect_size };
	rect.x += Periodic::Triangle0_1(3s, DataManager::get().time) * rect_size * 3;
	if (box->lines(Direction::down).center().intersects(rect)) {
		if (Periodic::Square0_1(3s, DataManager::get().time))box->pos->x += Scene::DeltaTime() / 1.5 * rect_size * 3;
		else box->pos->x -= Scene::DeltaTime() / 1.5 * rect_size * 3;
	}
	box->hit(rect);
}

void Moving::draw(const Point& pos)const
{
	RectF rect{ pos * rect_size,rect_size };
	rect.x += Periodic::Triangle0_1(3s, DataManager::get().time) * rect_size * 3;
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(rect.pos);
}

void Elevator::reaction(const Point& pos, PhysicsBox* box)
{
	Rect rect{ pos * rect_size,rect_size };
	rect.y += rect_size * 3;
	rect.y -= (int32)(Periodic::Triangle0_1(6s, DataManager::get().time) * rect_size * 5);
	box->hit(rect);

	if (box->lines(Direction::down).intersects(rect) && not Periodic::Square0_1(6s, DataManager::get().time)) {
		box->pos->y += (rect_size * 5) * Scene::DeltaTime() / 3 + 0.2;
	}
}
void Elevator::draw(const Point& pos)const
{
	Rect rect{ pos * rect_size,rect_size };
	rect.y += rect_size * 3;
	rect.y -= (int32)(Periodic::Triangle0_1(6s, DataManager::get().time) * rect_size * 5);
	TextureAsset(U"ChocolateWall").resized(rect_size).draw(rect.pos);
}

void StrawberrySoldierBlock::update(const Point& pos){
	if (not bornFlg) {
		DataManager::get().addEntity(U"StrawberrySoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void CookieSoldierBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"CookieSoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void Door::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"Door", pos * rect_size);
		bornFlg = true;
	}
}

void WeakWall::reaction(const Point& pos, PhysicsBox* box)
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

void WeakWall::draw(const Point& pos)const {
	if (not breaked) {
		TextureAsset(U"ChocolateWall").resized(rect_size).draw(pos * rect_size);
		Rect{ pos * rect_size,rect_size }.draw(ColorF{ Palette::Red,0.3 });
	}
}

void BeltConveyorRight::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size,rect_size };
	if (rect.stretched(0, 0.1).intersects(box->lines(Direction::down).begin)) {
		box->pos->x += Scene::DeltaTime() * rect_size / 0.5;
	}
	box->hit(rect);
}

void BeltConveyorRight::draw(const Point& pos)const
{
	const double size = TextureAsset(U"ChocolateWall").size().x;
	const double d = Periodic::Sawtooth0_1(0.5s, DataManager::get().time);
	TextureAsset(U"ChocolateWall")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
	TextureAsset(U"ChocolateWall")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
}

void BeltConveyorLeft::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size,rect_size };

	if (box->lines(Direction::down).end.intersects(rect.stretched(0, 0.1))) {
		box->pos->x -= Scene::DeltaTime() * rect_size / 0.5;
	}

	box->hit(rect);
}

void BeltConveyorLeft::draw(const Point& pos)const {
	const double size = TextureAsset(U"ChocolateWall").size().x;
	const double d = 1 - Periodic::Sawtooth0_1(0.5s, DataManager::get().time);
	TextureAsset(U"ChocolateWall")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
	TextureAsset(U"ChocolateWall")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
}

void SpawnerStrawberrySoldier::update(const Point& pos) {

	if (not Rect{ Arg::center(pos * rect_size),rect_size * 5,9999 }.intersects(DataManager::get().playerPos)) {

		constexpr double spawn = 1.0;

		for (accumlater += Scene::DeltaTime(); spawn <= accumlater; accumlater -= spawn) {
			DataManager::get().addEntity(U"SpawnerStrawberrySoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		}
	}
}

void SnowmanBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"Snowman", pos * rect_size + Vec2{ 1,1 }*rect_size);
		bornFlg = true;
	}
}

void ItigoSlaveBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"ItigoSlave", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}


void CloudEnemyBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"CloudEnemy", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void CornBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"Corn", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void CookieMutiBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"CookieMuti", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void CookieKabanBlock::update(const Point& pos){
	if (not bornFlg) {
		DataManager::get().addEntity(U"CookieKaban", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void ZerosenBlock::update(const Point& pos){
	if (not bornFlg) {
		DataManager::get().addEntity(U"Zerosen", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void SnowKnightBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"SnowKnight", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void SlaversCookieBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"SlaversCookie", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void ThroughBlock::reaction(const Point& pos, PhysicsBox* box)
{
	//ヘッドドロップのときはすり抜ける(速度で判定)
	if (not(1150 < (box->vel->y))) {
		box->hitDown(Rect{ pos * rect_size,rect_size,20 });
	}
}

void ThroughBlock::draw(const Point& pos)const {

	Rect{ pos * rect_size,rect_size,20 }.draw(Palette::Blue);
}

void PoleBlock::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size ,rect_size };
	if (box->lines(Direction::right).intersects(rect) and 600 < box->vel->x) {
		DataManager::get().table.emplace(U"PoleHit");
		DataManager::get().table.emplace(U"ShakeCamera");
	}
	else if (box->lines(Direction::left).intersects(rect) and box->vel->x < -600) {
		DataManager::get().table.emplace(U"PoleHit");
		DataManager::get().table.emplace(U"ShakeCamera");
	}

	box->hit(rect);
}

void PoleBlock::draw(const Point& pos)const{
	RectF{ pos * rect_size ,rect_size }.draw(Palette::Darkgray);
};

void CaptainBlock::update(const Point& pos){
	if (not bornFlg) {
		DataManager::get().addEntity(U"Captain", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void LastBossBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"LastBoss", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void IceCream::draw(const Point& pos)const {
	TextureAsset(U"IceCream").resized(rect_size * 2).draw(pos * rect_size);
};

void Candle::draw(const Point& pos)const {
	TextureAsset(U"Candle").resized(rect_size * 2).draw(pos * rect_size - Vec2::UnitX() * rect_size / 2);
	TextureAsset(U"fire").resized(rect_size).draw(pos * rect_size - Vec2::UnitY() * rect_size / 2);
};

void Candy::draw(const Point& pos)const {
	TextureAsset(U"Candy").resized(rect_size * 2.5).rotated(15_deg).draw(pos * rect_size);
};

void CandyStick::draw(const Point& pos)const {
	TextureAsset(U"CandyStick").resized(rect_size * 2.2).draw(pos * rect_size);
};

void Cherries::draw(const Point& pos)const {
	TextureAsset(U"Cherries").resized(rect_size * 1.3).draw(pos * rect_size);
};

void Blueberry::draw(const Point& pos)const {
	TextureAsset(U"Blueberry").resized(rect_size * 1.3).draw(pos * rect_size);
};


void FallingRocksBlock::update(const Point& pos) {

	constexpr double range = 1 * rect_size;

	const double blockX = pos.x * rect_size;

	if (not fallingFlg) {

		const double playerX = DataManager::get().playerPos.x;

		if (InRange(playerX, blockX - range, blockX + range)) {
			fallingFlg = true;

			DataManager::get().addEntity(U"FallingRocks",(pos+Vec2{0.5,0.5})*rect_size);
		}

	}
};

void FallingRocksBlock::draw(const Point& pos)const {

	if (not fallingFlg)
	{
		RectF{ pos * rect_size,rect_size }.draw(Palette::Darkred);
	}
};

void RollingRocksBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"RollingRocks", (pos+Vec2{1,1}) * rect_size);
		bornFlg = true;
	}
}
