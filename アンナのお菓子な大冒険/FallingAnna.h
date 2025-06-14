#pragma once
# include"Common.h"
#include"MiniGameSceneBase.h"

namespace FallingAnna {

	using namespace std;
	class picture
	{
	public:
		float x = 0, y = 0, w = 0, h = 0, tx = 0, ty = 0;
		string texname = "";
		std::map<string, Texture*> textures = map<string, Texture*>();
		bool mirror = false;
		picture(float X, float Y, float W, float H, float TX, float TY, float RAD, float OPA
			, map<string, Texture*> TEXTURES, string TEXNAME = "def")
		{
			x = X;
			y = Y;
			w = W;
			h = H;
			tx = TX;
			ty = TY;
			rad = RAD;
			setOpa(OPA);
			textures = TEXTURES;
			texname = TEXNAME;

		}
		static picture fromtex(Texture* t, float size, bool hin = false, float txp = 0.5f, float typ = 0.5f
			, float rad = 0, float opa = 1, float X = 0, float Y = 0)
		{

			float hi = t->width() / (float)t->height();
			float w = 0, h = 0;
			if (hin)
			{
				h = size;
				w = h * hi;
			}
			else
			{
				w = size;
				h = w / hi;
			}
			auto p = picture(X, Y, w, h, w * txp, h * typ, rad, opa
				, map<string, Texture*>{ {"def", t}});

			p.settxy(X, Y);
			return p;
		}
		float getTX()const
		{
			if (mirror) return w - tx; else return tx;
		}
		/// <summary>
		/// 画像の中心点yを返す
		/// </summary>
		float getTY()const
		{
			return ty;
		}
		void setOpa(float o)
		{
			opa = o;
			if (opa > 1)opa = 1;
			else if (opa < 0)opa = 0;
		}
		float getOpa()const
		{
			return opa;
		}
		void setRad(float r)
		{
			auto tx = gettx();
			auto ty = getty();
			rad = r;
			rad = atan2f(sinf(rad), cosf(rad));
			settxy(tx, ty);
		}
		float getRad()const
		{
			return rad;
		}


		float getcx(float ww, float hh)const
		{
			float W; if (mirror) W = w - ww; else W = ww;
			float H; if (mirror && 1 == 0) H = h - hh; else H = hh;
			float rx = x + W * cosf(rad) - H * sinf(rad);
			return rx;
		}
		/// <summary>
		/// 図形上の一点のy座標を取得する(回転の影響を考慮してるってこと)
		/// </summary>
		/// <param name="ww">左上を0としたときの図形の点の位置</param>
		/// <param name="hh">左上を0としたときの図形の点の位置</param>
		/// <returns>返されるのはy座標の値</returns>
		float getcy(float ww, float hh)const
		{
			float W; if (mirror) W = w - ww; else W = ww;
			float H; if (mirror && 1 == 0) H = h - hh; else H = hh;
			float ry = y + W * sinf(rad) + H * cosf(rad);
			return ry;
		}
		/// <summary>
		/// 中心点のx座標を返す。
		/// </summary>
		/// <returns>x座標</returns>
		float gettx()const
		{

			return x + getTX() * cosf(rad) - getTY() * sinf(rad);

		}
		/// <summary>
		/// 中心点のy座標を返す。
		/// </summary>
		/// <returns>y座標</returns>
		float getty()const
		{
			return y + getTX() * sinf(rad) + getTY() * cosf(rad);

		}
		/// <summary>
		/// 中心をxy座標にセットする。
		/// </summary>
		/// <param name="xx">セットするx座標</param>
		/// <param name="yy">セットするy座標</param>
		void settxy(float xx, float yy)
		{
			x = xx - getTX() * cosf(rad) + getTY() * sinf(rad);
			y = yy - getTX() * sinf(rad) - getTY() * cosf(rad);
		}


		/// <summary>
		/// 画像上の任意の一点をxy座標にセットする
		/// </summary>
		/// <param name="xx">セットするx座標</param>
		/// <param name="yy">セットするy座標</param>
		/// <param name="cx">画像上のwの点</param>
		/// <param name="cy">画像上のhの点</param>
		void setcxy(float xx, float yy, float cx, float cy)
		{
			x += xx - getcx(cx, cy);
			y += yy - getcy(cx, cy);

		}
		void draw(float camx, float camy, float scale = 1)const
		{
			if (textures.contains(texname))
			{
				auto a = textures.find(texname);
				a->second->mirrored(mirror)
					.resized(w * scale, h * scale)
					.rotatedAt(tx * scale, ty * scale, rad)
					.drawAt(getcx(w / 2, h / 2) * scale - camx, getcy(w / 2, h / 2) * scale - camy, ColorF{ 1,opa });


			}

		}
	private:
		float opa, rad;


	};

	//名前を変更してください
	class FallingAnna : public MiniGameSceneBase
	{
	public:
		//ここに変数などを宣言
		const Texture defanna{ U"MiniGameAsset/FallingAnna/tex/hutuuanna.png" };

		const Texture kasokuanna{ U"MiniGameAsset/FallingAnna/tex/rakkaanna.png" };
		const Texture sippaianna{ U"MiniGameAsset/FallingAnna/tex/sippai.png" };
		const Texture clearanna{ U"MiniGameAsset/FallingAnna/tex/clear.png" };
		Texture rock{ U"MiniGameAsset/FallingAnna/tex/rockball.png" };
		const Texture toge{ U"MiniGameAsset/FallingAnna/tex/thunderbolt.png" };
		const Array<Texture> cloud{ Texture(U"☁"_emoji),Texture(U"MiniGameAsset/FallingAnna/tex/cloud0.png"),Texture(U"MiniGameAsset/FallingAnna/tex/cloud1.png") };
		const Texture jimen{ U"MiniGameAsset/FallingAnna/tex/ground.bmp" };
		const Texture tower{ U"MiniGameAsset/FallingAnna/tex/tower2.png" };
		Texture bat{ U"MiniGameAsset/FallingAnna/tex/physicbit.bmp" };
		Array<picture>bats = Array<picture>();
		Array<picture>brokes = Array<picture>();
		Array<Vec2>batspeeds = Array<Vec2>();

		const Audio damageA{ U"MiniGameAsset/FallingAnna/oto/don.wav" };
		const Audio hakaiA{ U"MiniGameAsset/FallingAnna/oto/bomber.wav" };
		const Audio kasokuA{ U"MiniGameAsset/FallingAnna/oto/jett.wav" };
		const Audio clearA{ U"MiniGameAsset/FallingAnna/oto/fanfare.wav" };
		const Audio sippaiA{ U"MiniGameAsset/FallingAnna/oto/hoaa.wav" };
		const Audio bgm{ U"MiniGameAsset/FallingAnna/oto/sandou.mp3" };

		const Font Mfont{ 30 };
		const Font Sfont{ 60 };
		const Font Cfont{ 120 };

		const float ww = 1200, wh = 800;

		const float annaw = 45, annah = 90;
		const float annnaatari = 0.8f;


		const float batsize = 40;
		const float batspeed = 4;
		const float batkaiten = 0.3;
		const float batopa = 0.75;
		const float effnaga = 40;
		float eftime = 0;

		const float rocksize = 80, mrocksize = 120;
		const float rockkaiten = 0.02, rockfade = 30;
		const float togesize = 60;

		float perrock = 0.016;
		float pertoge = 0.016;
		float percloud = 0.04;
		float goukakuline = 400;

		const float spx = 6, spy = 5, Uspy = 1, breaky = 0.04, startK = 0.06;
		const float hitstopT = 120, attackT = 60;

		float nowspy = 0;
		Vec2 annas = Vec2(0, wh * 0.2);
		Array<picture>rocks = Array<picture>();
		Array<Vec2>toges = Array<Vec2>();
		Array<Vec2>clouds = Array<Vec2>();
		Array<float>cloudsizes = Array<float>();
		float score = 0;

		float nokori = 10000;
		float prenokori = 10000;
		//リザルトの地面の高さ
		const float jimentaka = 0.73;

		const float spscore = 1.5, scorebai = 0.01, rockscore = 20, damagedscore = -40, damagedwari = 0.8;

		//リザルトの終了を告げる鐘
		bool superend = false;

		bool left = false;

		float hitstop = 0;
		float attack = 0;
		float soutaix(float x)const
		{
			auto dx = x - annas.x + ww / 2;
			while (dx < -ww + ww / 2)
			{
				dx += ww * 2;
			}
			while (dx > ww + ww / 2)
			{
				dx -= ww * 2;
			}
			return dx;
		}
		bool getend()
		{
			return nokori <= 0;
		}
		bool getattack()const
		{
			return attack > 0;
		}
		bool gethitstop()const
		{
			return hitstop > 0;
		}
		bool getclear()const
		{
			return score >= goukakuline;
		}
		bool getsuperend()const
		{
			return superend;
		}


		FallingAnna(const InitData& init)//名前を変更してください
			: MiniGameSceneBase{ init }
		{
			//変数などの初期化
			Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
			switch (getData().mini_mode)
			{
			case Stage_Mode:
				nokori *= 1.0;
				goukakuline *= 1.0;
				perrock *= 1.25;
				pertoge *= 0.75;
				percloud *= 1.0;
				break;
			case Easy_Mode:
				nokori *= 1.2;
				goukakuline *= 1.2;
				perrock *= 1.0;
				pertoge *= 1.0;
				percloud *= 1.0;
				break;
			case Normal_Mode:
				nokori *= 1.5;
				goukakuline *= 1.6;
				perrock *= 1.0;
				pertoge *= 1.2;
				percloud *= 1.2;
				break;
			case Hard_Mode:

				nokori *= 2;
				goukakuline *= 2.5;
				perrock *= 0.8;
				pertoge *= 1.4;
				percloud *= 1.6;
				break;
			default:
				break;
			}
			prenokori = nokori;
			nokori += 100;
			bgm.setLoop(true);
			bgm.play(BGMMixBus);
		}

		void generateRock()
		{
			float size = Random(rocksize, mrocksize);
			rocks.push_back
			(
				picture::fromtex(&rock, size, false, 0.5, 0.5, Math::Pi * Random(0.0, 2.0)
					, 1, Random<float>(0, ww * 2), wh + size)
			);

		}
		void generateToge()
		{
			toges.push_back(Vec2(Random<float>(0, ww * 2), wh + togesize));
		}
		void generateCloud()
		{
			//ここで雲のサイズを定義
			auto size = Random<float>(45, 180);
			clouds.push_back(Vec2(Random<float>(0, ww * 2), wh + size));
			cloudsizes.push_back(size);
		}
		void idou(float x)
		{
			annas.x += x;
			while (annas.x < -ww)
			{
				annas.x += ww * 2;
			}
			while (annas.x > ww)
			{
				annas.x -= ww * 2;
			}
		}
		void rakka(float y)
		{
			nokori -= y;
			for (int i = rocks.size() - 1; i >= 0; i--)
			{
				rocks[i].y -= y;
			}
			for (int i = toges.size() - 1; i >= 0; i--)
			{
				toges[i].y -= y;
			}
			for (int i = clouds.size() - 1; i >= 0; i--)
			{
				clouds[i].y -= y;
			}
			/*for (int i = bats.size() - 1; i >= 0; i--)
			{
				bats[i].y -= y;

			}*/
			for (int i = brokes.size() - 1; i >= 0; i--)
			{
				brokes[i].y -= y;

			}
		}
		void clear()
		{
			for (int i = rocks.size() - 1; i >= 0; i--)
			{
				auto a = rocks[i];
				auto size = a.w * 100;
				if (a.y < -size)
				{
					rocks.erase(rocks.begin() + i);
				}
			}
			for (int i = toges.size() - 1; i >= 0; i--)
			{
				auto a = toges[i];
				auto size = togesize * 2;
				if (a.y < -size)
				{
					toges.erase(toges.begin() + i);
				}
			}
			for (int i = clouds.size() - 1; i >= 0; i--)
			{
				auto a = clouds[i];
				auto size = cloudsizes[i] * 100;
				if (a.y < -size)
				{
					clouds.erase(clouds.begin() + i);
					cloudsizes.erase(cloudsizes.begin() + i);
				}
			}
			for (int i = bats.size() - 1; i >= 0; i--)
			{
				auto a = bats[i];


				if (a.y < -a.h - a.w)
				{
					bats.erase(bats.begin() + i);
					batspeeds.erase(batspeeds.begin() + i);
				}
			}
			for (int i = brokes.size() - 1; i >= 0; i--)
			{
				auto a = brokes[i];


			}
		}
		void death()
		{
			auto r = Ellipse(soutaix(annas.x), annas.y, annaw / 2 * annnaatari, annah / 2 * annnaatari);
			for (int i = rocks.size() - 1; i >= 0; i--)
			{
				auto a = rocks[i];
				auto size = a.w / 2;
				if (Circle(soutaix(a.gettx()), a.getty(), size).intersects(r))
				{
					if (gethitstop())
					{
					}
					else if (getattack())
					{
						hakaiA.playOneShot();
						brokes.push_back(a);
						rocks.erase(rocks.begin() + i);
						score += rockscore;
					}
					else
					{
						rocks.erase(rocks.begin() + i);
						damage();
						break;
					}

				}

			}
			for (auto a : toges)
			{
				auto size = togesize / 2;
				if (Circle(soutaix(a.x), a.y, size).intersects(r))
				{
					if (hitstop > 0)
					{
					}
					else
					{
						damage();
						break;
					}
				}

			}

		}
		void bateffect()
		{
			if (gethitstop())
			{
				eftime -= 0.5;
			}
			else if (getattack())
			{
				eftime -= 2;
			}
			else
			{
				eftime -= 1;
			}
			if (eftime <= 0)
			{
				auto p = picture::fromtex(&bat, batsize, false
				, 0.5, 0.5, Math::Pi * Random(0.0, 2.0), batopa, annas.x + annaw / 2.0 * Random(-1.0, 1.0)
					, annas.y + annah / 2.0 * Random(-1.0, 1.0));
				bats.push_back(p);
				batspeeds.push_back(Vec2(batspeed * Random(-1.0, 1.0)
					, -nowspy + batspeed * Random(-1.0, 0.0)));
				eftime += effnaga;
			}

		}
		void batframe(float cl)
		{
			for (auto i = 0; i < bats.size(); i++)
			{
				auto a = &(bats[i]);
				a->x += batspeeds[i].x * cl;
				a->y += batspeeds[i].y * cl;
				if (batspeeds[i].x > 0) {
					a->setRad(a->getRad() + batkaiten * cl);
				}
				else
				{
					a->setRad(a->getRad() - batkaiten * cl);
				}
			}
		}
		void rockframe(float cl)
		{
			for (auto i = 0; i < rocks.size(); i++)
			{
				auto a = &rocks[i];
				if ((int)a->gettx() % 2 == 0)
				{
					a->setRad(a->getRad() + rockkaiten * cl);
				}
				else
				{
					a->setRad(a->getRad() - rockkaiten * cl);
				}
			}
			for (auto i = 0; i < brokes.size(); i++)
			{
				auto a = &brokes[i];
				a->setOpa(a->getOpa() - 1.0 / rockfade * cl);

			}
		}
		void annaframe(float cl)
		{
			hitstop -= cl;

			if (hitstop < 0)hitstop = 0;
			attack -= cl;
			if (attack < 0)attack = 0;
			if (!hitstop && !getend())score += Math::Pow(nowspy, spscore) * cl * scorebai;


		}
		void damage()
		{
			hitstop = hitstopT;
			nowspy = spy;
			damageA.playOneShot();
			score = (score - damagedscore) * damagedwari;
			if (score < 0)score = 0;
		}
		void kasoku()
		{
			nowspy += Uspy;
			attack = attackT;
			kasokuA.playOneShot();
		}
		void dosuperend()
		{
			superend = true;
			if (getclear())
			{
				clearA.playOneShot();
			}
			else
			{
				sippaiA.playOneShot();
			}
		}
		void gameUpdate() override
		{
			float cl = 1;
			batframe(cl);
			rockframe(cl);
			if (getData().minigameLeftKey.pressed())
			{
				left = true;
			}
			else if (getData().minigameRightKey.pressed())
			{
				left = false;
			}
			if (!getend())
			{
				annaframe(cl);

				if (getData().minigameUpKey.pressed())
				{
					nowspy += (spy - nowspy) * breaky;
				}
				else if (getData().minigameDownKey.down())
				{
					kasoku();
				}
				else if (nowspy < spy)
				{
					nowspy += (spy - nowspy) * startK;
				}

				rakka(nowspy * cl);
				if (getData().minigameLeftKey.pressed())
				{
					idou(-spx * cl);
				}
				if (getData().minigameRightKey.pressed())
				{
					idou(spx * cl);
				}
				death();
				clear();
				while (prenokori > nokori)
				{

					prenokori--;
					bateffect();
					if (RandomBool(perrock))
					{
						generateRock();
					}
					if (RandomBool(pertoge))
					{
						generateToge();
					}
					if (RandomBool(percloud))
					{
						generateCloud();
					}
				}
				if (getend())
				{

					bgm.fadeVolume(0, Duration(2.5));
				}
			}
			if (getend())
			{
				//この辺はクリア演出
				rakka(cl * 4);
				attack = 0;
				hitstop = 0;
				nokori = 0;
				if (annas.y <= wh * jimentaka)
				{
					annas.y += (wh * jimentaka - annas.y) * 0.01 + cl * 0.5;
					annas.x += (0 - annas.x) * 0.005;
				}
				else if (!getsuperend())
				{
					annas.y = wh * jimentaka;

					dosuperend();

					if(getclear())
					{
						getData().saveMiniGameClear();
					}
				}
				else if (getsuperend())
				{
					if (KeyEnter.down())
					{
						//	changeScene(U"Fallinganna");
						EndGame(getclear());
					}
					////ステージモードかつクリア時はリトライできない
					//else if (!(getData().mini_mode == Stage_Mode && getclear()) && getData().KeyUp.down())
					//{
					//	changeScene(U"FallingAnna");
					//}
				}

			}

		}
		int yset(int y, int h) const
		{
			while (y < h - 0)y += 800;
			while (y > h + 800) y -= 800;
			return y;
		}
		void drawtower() const
		{

			double hi = 3.0 / 24;

			auto px = (annas.x / 4);
			auto x = px;

			while (x > ww * (hi))x -= (float)(ww * (hi));
			while (x < 0)x += (float)(ww * (hi));
			auto isou = x / (ww * hi);

			auto a1 = tower(0, 0, tower.width() * isou, tower.height()).resized(x, wh);
			auto a2 = tower(tower.width() * isou, 0, tower.width() - tower.width() * isou, tower.height()).resized(ww * (hi)-x, wh);

			auto b1 = tower(0, 0, tower.width() * isou, tower.height()).resized(x * 3.0 / 4.0, wh);
			auto b2 = tower(tower.width() * isou, 0, tower.width() - tower.width() * isou, tower.height()).resized(ww * (hi * 3.0 / 4.0) - x * 3.0 / 4.0, wh);

			auto c1 = tower(0, 0, tower.width() * isou, tower.height()).resized(x / 2.0, wh);
			auto c2 = tower(tower.width() * isou, 0, tower.width() - tower.width() * isou, tower.height()).resized(ww * (hi / 2.0) - x / 2.0, wh);

			auto d1 = tower(0, 0, tower.width() * isou, tower.height()).resized(x / 4.0, wh);
			auto d2 = tower(tower.width() * isou, 0, tower.width() - tower.width() * isou, tower.height()).resized(ww * (hi / 4.0) - x / 4.0, wh);



			a1.draw(ww / 2 - ww * (hi / 2.0) + ww * (hi)-x, yset(nokori - 400, 0));
			a2.draw(ww / 2 - ww * (hi / 2.0), yset(nokori - 400, 0));

			b1.draw(ww / 2 - ww * (hi * 5.0 / 4.0) + ww * (hi * 3.0 / 4.0) - x * 3.0 / 4.0, yset(nokori - 400, 0));
			b2.draw(ww / 2 - ww * (hi * 5.0 / 4.0), yset(nokori - 400, 0));

			b1.draw(ww / 2 + ww * (hi / 2.0) + ww * (hi * 3.0 / 4.0) - x * 3.0 / 4.0, yset(nokori - 400, 0));
			b2.draw(ww / 2 + ww * (hi / 2.0), yset(nokori - 400, 0));

			c1.draw(ww / 2 - ww * (hi * 7.0 / 4.0) + ww * (hi / 2.0) - x / 2.0, yset(nokori - 400, 0));
			c2.draw(ww / 2 - ww * (hi * 7.0 / 4.0), yset(nokori - 400, 0));

			c1.draw(ww / 2 + ww * (hi * 5.0 / 4.0) + ww * (hi / 2.0) - x / 2.0, yset(nokori - 400, 0));
			c2.draw(ww / 2 + ww * (hi * 5.0 / 4.0), yset(nokori - 400, 0));

			d1.draw(ww / 2 - ww * (hi * 8.0 / 4.0) + ww * (hi / 4.0) - x / 4.0, yset(nokori - 400, 0));
			d2.draw(ww / 2 - ww * (hi * 8.0 / 4.0), yset(nokori - 400, 0));

			d1.draw(ww / 2 + ww * (hi * 7.0 / 4.0) + ww * (hi / 4.0) - x / 4.0, yset(nokori - 400, 0));
			d2.draw(ww / 2 + ww * (hi * 7.0 / 4.0), yset(nokori - 400, 0));


			a1.draw(ww / 2 - ww * (hi / 2.0) + ww * (hi)-x, yset(nokori - 400, -800));
			a2.draw(ww / 2 - ww * (hi / 2.0), yset(nokori - 400, -800));

			b1.draw(ww / 2 - ww * (hi * 5.0 / 4.0) + ww * (hi * 3.0 / 4.0) - x * 3.0 / 4.0, yset(nokori - 400, -800));
			b2.draw(ww / 2 - ww * (hi * 5.0 / 4.0), yset(nokori - 400, -800));

			b1.draw(ww / 2 + ww * (hi / 2.0) + ww * (hi * 3.0 / 4.0) - x * 3.0 / 4.0, yset(nokori - 400, -800));
			b2.draw(ww / 2 + ww * (hi / 2.0), yset(nokori - 400, -800));

			c1.draw(ww / 2 - ww * (hi * 7.0 / 4.0) + ww * (hi / 2.0) - x / 2.0, yset(nokori - 400, -800));
			c2.draw(ww / 2 - ww * (hi * 7.0 / 4.0), yset(nokori - 400, -800));

			c1.draw(ww / 2 + ww * (hi * 5.0 / 4.0) + ww * (hi / 2.0) - x / 2.0, yset(nokori - 400, -800));
			c2.draw(ww / 2 + ww * (hi * 5.0 / 4.0), yset(nokori - 400, -800));

			d1.draw(ww / 2 - ww * (hi * 8.0 / 4.0) + ww * (hi / 4.0) - x / 4.0, yset(nokori - 400, -800));
			d2.draw(ww / 2 - ww * (hi * 8.0 / 4.0), yset(nokori - 400, -800));

			d1.draw(ww / 2 + ww * (hi * 7.0 / 4.0) + ww * (hi / 4.0) - x / 4.0, yset(nokori - 400, -800));
			d2.draw(ww / 2 + ww * (hi * 7.0 / 4.0), yset(nokori - 400, -800));


		}
		void gameDraw() const override
		{
			TextureAsset{ U"BackGroundTexture/雲背景.png" }.resized(Scene::Size()).draw();

			//アンナちゃんとかだけは相対座標で表示することでいい感じだったりする
			drawtower();
			//描画(draw関数内ではifの使用、変数の代入などができない)
			if (getsuperend())
			{
				if (getclear()) {
					clearanna.resized(annaw, annah)
						.mirrored(left)
						.drawAt(soutaix(annas.x), annas.y);
				}
				else
				{
					sippaianna.resized(annaw, annah)
						.mirrored(left)
						.drawAt(soutaix(annas.x), annas.y);
				}
			}
			else if (getattack()) {
				kasokuanna.resized(annaw, annah)
					.mirrored(left)
					//.rotatedAt(rocksize / 2, rocksize / 2,90)
					.drawAt(soutaix(annas.x), annas.y, ColorF{ 1.0, Cos(hitstop / 5) });
			}
			else
			{
				defanna.resized(annaw, annah)
					.mirrored(left)
					.drawAt(soutaix(annas.x), annas.y, ColorF{ 1.0, Cos(hitstop / 5) });
			}

			jimen.resized(ww * 4, wh).drawAt(soutaix(0), wh * jimentaka - (annas.y + annah / 2 - wh * jimentaka) + wh / 2 + annah);
			for (int i = 0; i < rocks.size(); i++)
			{
				rocks[i].draw(-soutaix(rocks[i].gettx()) + rocks[i].gettx(), 0);
			}
			for (int i = 0; i < brokes.size(); i++)
			{
				brokes[i].draw(-soutaix(brokes[i].gettx()) + brokes[i].gettx(), 0);
			}
			/*
			for (auto a : rocks)
			{
				auto sp = 1;
				//最後のとこは回転速度
				if ((int)a.x % 2 == 0)sp *= -1;
				rock.resized(rocksize, rocksize)
					.rotatedAt(rocksize / 2, rocksize / 2, sp*(a.x + nokori) * 0.1 / 100)
					.drawAt(soutaix(a.x), a.y);

			}*/
			for (auto a : toges)
			{
				auto sp = 1;
				//最後のとこは回転速度
				if ((int)a.x % 2 == 0)sp *= -1;
				toge.resized(togesize, togesize)
					.rotatedAt(togesize / 2, togesize / 2, sp * (a.x + nokori) * 0.8 / 100)
					.drawAt(soutaix(a.x), a.y);

			}
			for (int i = 0; i < clouds.size(); i++)
			{
				auto cloudsize = cloudsizes[i];
				auto a = clouds[i];

				//雲はちょっと薄くしたりして表示
				cloud[(int)a.x % cloud.size()].resized(cloudsize, cloudsize)
					.rotatedAt(cloudsize / 2, cloudsize / 2, (a.x))
					.drawAt(soutaix(a.x), a.y, ColorF{ 1,0.75 });
			}
			for (int i = 0; i < bats.size(); i++)
			{
				bats[i].draw(-soutaix(bats[i].gettx()) + bats[i].gettx(), 0);
			}
			if (getsuperend())
			{
				if (getclear())
				{
					Cfont(U"クリア―――！").drawAt(ww * 0.5f, wh * 0.35f);
				}
				else
				{
					Cfont(U"失敗……").drawAt(ww * 0.5f, wh * 0.35f);
				}
				Sfont(U"合格ライン:", goukakuline).draw(Arg::topLeft = Vec2(ww * 0.5f, wh * 0.47f));
				Mfont(U"Enterでおわる").drawAt(Vec2(ww * 0.5f, wh * 0.6f));
			}
			if (getclear())
			{
				Sfont(Math::Round(score)).draw(Arg::topLeft = Vec2(ww * 0.05f, wh * 0.05f), Palette::Yellow);
			}
			else
			{
				Sfont(Math::Round(score)).draw(Arg::topLeft = Vec2(ww * 0.05f, wh * 0.05f));
			}
			Sfont(U"残り:", Math::Round(nokori / annah * 1.6), U"m").draw(Arg::topRight = Vec2(ww * 0.95f, wh * 0.05f));

			FontAsset{ U"NormalFont" }(U"[ESC]ポーズ").draw(Arg::topRight = Vec2{ Scene::Width() - 10,5 });
		}

	};


}
