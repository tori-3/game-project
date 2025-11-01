#include"Block.h"
#include"TalkManager.h"
#include"KeyInfo.h"
#include"ExplosionEffect.h"
#include"PlayMode.h"

void Block::reaction(const Point& pos, PhysicsBox* box)
{
	box->hit(Rect{ pos * rect_size,rect_size });
}

void Block::update(const Point&) {}

void Block::draw(const Point&)const {};

void CakeSurface::draw(const Point& pos)const
{
	TextureAsset(U"CakeSurface").resized(rect_size).draw(pos * rect_size);
}

void CakeGround::draw(const Point& pos)const
{
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
	if (box->lines(Direction::left).intersects(triangle))
	{
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
	if (box->lines(Direction::down).center().movedBy(0,1).intersects(rect))
	{
		if (Periodic::Square0_1(3s, DataManager::get().time))box->pos->x += Scene::DeltaTime() / 1.5 * rect_size * 3;
		else box->pos->x -= Scene::DeltaTime() / 1.5 * rect_size * 3;
	}
	box->hitCanNotRespawn(rect);
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
	box->hitCanNotRespawn(rect);

	if (box->lines(Direction::down).intersects(rect) && not Periodic::Square0_1(6s, DataManager::get().time))
	{
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

void StrawberrySoldierBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"StrawberrySoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void BigStrawberrySoldierBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"BigStrawberrySoldier", pos * rect_size + Vec2{ 1,1 }*rect_size);
		bornFlg = true;
	}
}

void CookieSoldierBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"CookieSoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void BigCookieSoldierBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"BigCookieSoldier", pos * rect_size + Vec2{ 1,1 }*rect_size);
		bornFlg = true;
	}
}

void Door::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"Door", pos * rect_size);
		bornFlg = true;
	}
}

void WeakWall::reaction(const Point& pos, PhysicsBox* box)
{
	const Rect rect{ pos * rect_size,rect_size };

	if (not breaked)
	{
		if (box->lines(Direction::left).intersects(rect) || box->lines(Direction::right).intersects(rect) || box->lines(Direction::up).intersects(rect) || box->lines(Direction::down).intersects(rect))
		{
			if (box->rush)
			{
				breaked = true;

				ControllerManager::get().setVibration(0.1);
				DataManager::get().additiveEffect.add<ExplosionEffect>(rect.center(), 80, ColorF{Palette::Lightyellow,0.3});
				AudioAsset{ U"突進衝突" }.play();
				return;
			}
		}
		box->hitCanNotRespawn(rect);
	}
}

void WeakWall::draw(const Point& pos)const
{
	if (not breaked)
	{
		TextureAsset(U"WeakWall").resized(rect_size).draw(pos * rect_size);
	}
}

void BeltConveyorRight::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size,rect_size };
	if (rect.stretched(0, 0.1).intersects(box->lines(Direction::down).begin)) {
		box->pos->x += Scene::DeltaTime() * rect_size / 0.5;
	}
	box->hitCanNotRespawn(rect);
}

void BeltConveyorRight::draw(const Point& pos)const
{
	const double size = TextureAsset(U"ChocolateWall").size().x;
	const double d = Periodic::Sawtooth0_1(0.5s, DataManager::get().time);
	TextureAsset(U"BeltConveyor")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
	TextureAsset(U"BeltConveyor")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
}

void BeltConveyorLeft::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size,rect_size };

	if (box->lines(Direction::down).end.intersects(rect.stretched(0, 0.1))) {
		box->pos->x -= Scene::DeltaTime() * rect_size / 0.5;
	}

	box->hitCanNotRespawn(rect);
}

void BeltConveyorLeft::draw(const Point& pos)const
{
	const double size = TextureAsset(U"ChocolateWall").size().x;
	const double d = 1 - Periodic::Sawtooth0_1(0.5s, DataManager::get().time);
	TextureAsset(U"BeltConveyor")(size - size * d, 0, size * d, size).resized(rect_size * d, rect_size).draw(pos * rect_size);
	TextureAsset(U"BeltConveyor")(0, 0, size - size * d, size).resized(rect_size * (1 - d), rect_size).draw((pos + Vec2{ d,0 }) * rect_size);
}

void SpawnerStrawberrySoldier::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"SpawnerStrawberrySoldier", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void SnowmanBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"Snowman", pos * rect_size + Vec2{ 1,1 }*rect_size);
		bornFlg = true;
	}
}

void ItigoSlaveBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"ItigoSlave", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void BigItigoSlaveBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"BigItigoSlave", pos * rect_size + Vec2{ 2,2 }*rect_size);
		bornFlg = true;
	}
}

void CloudEnemyBlock::update(const Point& pos) {
	if (not bornFlg) {
		DataManager::get().addEntity(U"CloudEnemy", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void CornBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
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

void CookieKabanBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"CookieKaban", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void ZerosenBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"Zerosen", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void SnowKnightBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"SnowKnight", pos * rect_size + Vec2{ 0.5,0 }*rect_size + Vec2{ 0,30 });
		bornFlg = true;
	}
}

void SlaversCookieBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"SlaversCookie", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void ThroughBlock::reaction(const Point& pos, PhysicsBox* box)
{
	//ヘッドドロップのときはすり抜ける(速度で判定)
	if (not(1150 < (box->vel->y)))
	{
		box->hitDown(Rect{ pos * rect_size,rect_size,20 });
	}
}

void ThroughBlock::draw(const Point& pos)const
{
	TextureAsset{ U"SlippingFloors" }.resized(rect_size).draw(pos * rect_size);
}

void PoleBlock::reaction(const Point& pos, PhysicsBox* box)
{
	RectF rect{ pos * rect_size ,rect_size };
	if (box->lines(Direction::right).intersects(rect) and box->rush)
	{
		DataManager::get().table.emplace(U"PoleHit");
		DataManager::get().table.emplace(U"ShakeCamera");
	}
	else if (box->lines(Direction::left).intersects(rect) and box->rush)
	{
		DataManager::get().table.emplace(U"PoleHit");
		DataManager::get().table.emplace(U"ShakeCamera");
	}

	box->hit(rect);
}

void PoleBlock::draw(const Point& pos)const
{
	if(pos.x==0)
	{
		TextureAsset{ U"moroiHashiraL" }.resized(rect_size).draw(pos * rect_size);
	}
	else
	{
		TextureAsset{ U"moroiHashiraR" }.resized(rect_size).draw(pos * rect_size);
	}
};

void CaptainBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"Captain", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void LastBossBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"LastBoss", pos * rect_size + Vec2{ 0.5,0 }*rect_size);
		bornFlg = true;
	}
}

void IceCream::draw(const Point& pos)const
{
	TextureAsset(U"IceCream").resized(rect_size * 2).draw(pos * rect_size);
};

void Candle::draw(const Point& pos)const
{
	TextureAsset(U"Candle").resized(rect_size * 2).draw(pos * rect_size - Vec2::UnitX() * rect_size / 2);
	TextureAsset(U"fire").resized(rect_size).draw(pos * rect_size - Vec2::UnitY() * rect_size / 2);
};

void Candy::draw(const Point& pos)const
{
	TextureAsset(U"Candy").resized(rect_size * 2.5).rotated(15_deg).draw(pos * rect_size);
};

void CandyStick::draw(const Point& pos)const
{
	TextureAsset(U"CandyStick").resized(rect_size * 2.2).draw(pos * rect_size);
};

void Cherries::draw(const Point& pos)const
{
	TextureAsset(U"Cherries").resized(rect_size * 1.3).draw(pos * rect_size);
};

void Blueberry::draw(const Point& pos)const
{
	TextureAsset(U"Blueberry").resized(rect_size * 1.3).draw(pos * rect_size);
};

void FallingRocksBlock::update(const Point& pos)
{
	constexpr double range = 1 * rect_size;

	const double blockX = pos.x * rect_size;

	if (not fallingFlg)
	{
		const double playerX = DataManager::get().playerPos.x;

		if (InRange(playerX, blockX - range, blockX + range))
		{
			fallingFlg = true;

			DataManager::get().addEntity(U"FallingRocks", (pos + Vec2{ 0.5,0.5 }) * rect_size);
		}
	}
};

void FallingRocksBlock::draw(const Point& pos)const
{
	if (not fallingFlg)
	{
		TextureAsset(U"PointedCorn").resized(rect_size).draw(pos * rect_size);
	}
};

void RollingRocksBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"RollingRocks", (pos+Vec2{1,1}) * rect_size);
		bornFlg = true;
	}
}

SignboardBlock::SignboardBlock(const Array<TalkWindow::TalkInfo>&list_)
	:list{ list_ }
{}

void  SignboardBlock::update(const Point& pos)
{
	if (DataManager::get().playerPos.intersects(RectF{ (pos - Vec2{2,2}) * rect_size,rect_size * 5 }))
	{
		if (DataManager::get().playerPos.x<RectF{ pos * rect_size,rect_size }.centerX())
		{
			DataManager::get().fairyPos = (pos + Vec2{ 1.5,0 }) * rect_size;
		}
		else
		{
			DataManager::get().fairyPos = (pos + Vec2{ -0.5,0 }) * rect_size;
		}
	}

	if (DataManager::get().playerPos.intersects(RectF{ (pos-Vec2{1,1}) * rect_size,rect_size*3}))
	{
		if (DataManager::get().gameData->attackKey.down())
		{
			for(auto& input: DataManager::get().gameData->attackKey.inputs())
			{
				input.clearInput();
			}

			for (auto& info : list)
			{
				info.text = DataManager::get().gameData->fmt(info.text);
			}

			TalkManager::get().talkWindow.setTalk(list);
		}

	}
}

void SoapBubble(const Circle& circle, const ColorF& color)
{
	circle.draw(ColorF(1, 0), color).drawFrame(2, ColorF(1, 0.5));
	Circle{ circle.center.x - circle.r * 0.4,circle.center.y - circle.r * 0.4,circle.r * 0.3 }.draw(ColorF(1, 0.5));
}

void SignboardBlock::draw(const Point& pos)const
{
	const bool fairy = DataManager::get().table.contains(U"SignboardInvisible");

	if(fairy)
	{
		if(not photographyMode)
		{
			SoapBubble(Circle{ (pos + Vec2{0.5,0}) * rect_size,rect_size * 0.3 }, HSV{ Scene::Time() * 30 ,0.3 });
		}
	}
	else
	{
		TextureAsset(U"Signboard").resized(rect_size).draw(pos * rect_size);
	}

	if (not DataManager::get().table.contains(U"TalkWindow"))
	{
		if (DataManager::get().playerPos.intersects(RectF{ (pos - Vec2{1,1}) * rect_size,rect_size * 3 }))
		{
			constexpr double thickness = 5;

			const RoundRect window{ RectF{ Arg::center((pos + Vec2{0.5,0.5 - 1}) * rect_size),rect_size * 1.5,rect_size * 0.8},20 };
			window.draw({ Palette::Black, 0.8 }).drawFrame(thickness, 0);

			FontAsset(U"NormalFont")(ToKeyName(DataManager::get().gameData->attackKey, DataManager::get().gameData->gamepadMode)).drawAt(25, window.center());
		}
	}
}

void ChocolateWallCanNotRespawn::reaction(const Point& pos, PhysicsBox* box)
{
	box->hitCanNotRespawn(Rect{ pos * rect_size,rect_size });
}

void CloudBlock::draw(const Point& pos)const
{
	TextureAsset(U"Cloud").resized(rect_size).draw(pos * rect_size);
}

constexpr double cloudTextureSize = rect_size * (273.0 / 227.0);

void CloudSurfaceBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudT" }.resized(cloudTextureSize).draw(Arg::bottomLeft=(pos + Point{ 0,1 }) * rect_size);
}

void CloudBLBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudBL" }.resized(cloudTextureSize).draw(Arg::topRight = (pos + Point{ 1,0 }) * rect_size);
}

void CloudBRBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudBR" }.resized(cloudTextureSize).draw(pos * rect_size);
}

void CloudTLBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudTL" }.resized(cloudTextureSize).draw(Arg::bottomRight= (pos + Point{ 1,1 }) * rect_size);
}

void CloudTRBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudTR" }.resized(cloudTextureSize).draw(Arg::bottomLeft= (pos + Point{ 0,1 }) * rect_size);
}

void CloudLBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudL" }.resized(cloudTextureSize).draw(Arg::topRight = (pos + Point{ 1,0 }) * rect_size);
}

void CloudRBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudR" }.resized(cloudTextureSize).draw(pos * rect_size);
}

void CloudBBlock::draw(const Point& pos)const
{
	TextureAsset{ U"CloudB" }.resized(cloudTextureSize).draw(pos * rect_size);
}

void ChocoCakeBlock::draw(const Point& pos)const
{
	TextureAsset{ U"ChocoCake" }.resized(rect_size).draw(pos * rect_size);
}

void ChocoCakeWallBlock::draw(const Point& pos)const
{
	TextureAsset{ U"ChocoCakeWall" }.resized(rect_size).draw(pos * rect_size);
}

void NeedleUpBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"NeedleUp", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void NeedleDownBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"NeedleDown", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void NeedleLeftBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"NeedleLeft", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void NeedleRightBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"NeedleRight", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

FairyBlock::FairyBlock()
{
	character.addMotion(U"Floating",true);
	DataManager::get().table.emplace(U"SignboardInvisible");
}

void FairyBlock::update(const Point& pos)
{
	character.update((pos + Vec2{ 0.5,0.5 }) * rect_size, DataManager::get().playerPos.x < pos.x * rect_size);
}

void FairyBlock::draw(const Point& pos)const
{
	character.draw();
}

void FloatingCookieItemBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"FloatingCookieItem", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void StrawberrySoldierTowerBlock::update(const Point& pos)
{
	if (not bornFlg)
	{
		DataManager::get().addEntity(U"StrawberrySoldierTower", pos * rect_size + Vec2{ 0.5,0.5 }*rect_size);
		bornFlg = true;
	}
}

void BackgroundDoor::draw(const Point& pos)const
{
	TextureAsset(U"Door").resized(rect_size * 2).draw(pos * rect_size);
	TextureAsset(U"Door").resized(rect_size * 2).mirrored().draw(pos * rect_size);
}

void WindowBlock::draw(const Point& pos)const
{
	TextureAsset(U"Window").resized(rect_size).draw(pos * rect_size);
}
