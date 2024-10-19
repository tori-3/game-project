#pragma once
#include"setting.h"

//class Background
//{
//public:
//	String texture;
//	double r, rate;
//	uint32 num;
//
//	Background(const String& pass, int x, uint32 n = 1) :texture{ pass } {
//		
//		r = Scene::Size().y / (double)TextureAsset { texture }.height();//倍率
//		rate = (TextureAsset{ texture }.width() * r * n - Scene::Size().x) / (rect_size * x);//画像を進める速度
//		num = n;
//	}
//	void draw(const Vec2& vec)const {
//		for (auto n : step(num)) {
//			TextureAsset{ texture }.scaled(r).draw(-vec.x * rate + (TextureAsset{ texture }.width() * r) * n, 0);
//		}
//	}
//
//};


//追従の割合を指定(xとy)



class Cloud {
public:

	struct CloudInfo {
		Vec2 pos;
		int32 type;

		void draw()const {
			if (type) {
				TextureAsset(U"Cloud1").resized(200).drawAt(pos);
			}
			else {
				TextureAsset(U"Cloud2").resized(200).drawAt(pos);
			}
		}
	};

	RectF range;
	RectF rect;

	Array<CloudInfo>list;

	Cloud(RectF rect, int32 n) :rect{ rect } {

		range = rect.stretched(100, 0);

		for (auto& i : step(n)) {
			list << CloudInfo{ RandomVec2(rect),Random(1)};
		}

	}

	void update(double d) {

		for (auto& i : step(list.size())) {
			list[i].pos.x += d;
			if (not list[i].pos.intersects(range)) {
				list[i].pos = { range.leftX(),Random(rect.y,rect.w) };
			}
		}
	}

	void draw()const {
		for (auto& cloud : list) {
			cloud.draw();
		}
	}
};

class CloudManager {
public:
	Rect rect{ 0,0,Scene::Size().x,200 };

	Cloud cloud1{ rect,10 };
	Cloud cloud2{ rect,10 };
	Cloud cloud3{ rect,10 };

	void update() {
		cloud1.update(Scene::DeltaTime() * 10);
		cloud2.update(Scene::DeltaTime() * 20);
		cloud3.update(Scene::DeltaTime() * 30);
	}

	void draw1()const {
		cloud1.draw();
		cloud2.draw();
	}
	void draw2()const {
		cloud3.draw();
	}

};
