#pragma once
#include"Common.h"

namespace Maze2 {

	//こんど名前空間に入れる。

	//名前を変更してください
	using namespace std;

	class Rotate;

	class Drawing
	{
	public:
		String assetName = U"ChocolateWall";

		void draw(const Rotate& rotate, const Vec2& pos, const Vec2& size, const Color& color)const;
	};

	class Rotate {
	public:
		Vec2 pos{ 0,0 };
		Vec2 cpos{ 0,0 };
		double angle = 0;
		double angle_vel = 0;
		Drawing drawing;

		Vec2 vec(Vec2 _vec) {
			return Vec2(_vec.x * cos(angle) - _vec.y * sin(angle), _vec.x * sin(angle) + _vec.y * cos(angle));
		}
		void drawRect(Vec2 _pos, Vec2 _size, Color _color) {
			drawing.draw(*this, pos + vec(_pos + Vec2(_size.y / 2.0, _size.y / 2.0) - cpos), _size + Vec2{ 1,1 }, _color);

			//Line{ pos + vec(_pos + Vec2(_size.y / 2.0,_size.y / 2.0) - cpos) ,pos + vec(_pos + Vec2(_size.x - _size.y / 2.0 + 0.01,_size.y / 2.0) + -cpos) }.draw(_size.y + 0.00001, _color);
		}
	};
	class Judge {
	public:
		bool reacts(Vec2 _position, Vec2 _size, Vec2 _position2, Vec2 _size2) {//長方形同士の当たり判定
			Vec2 cSize = _size + _size2;
			Vec2 cPosition = _position + (_size - _size2) / 2 - _position2;
			return Abs(cPosition.x) < cSize.x / 2 && Abs(cPosition.y) < cSize.y / 2;
		}
		bool reactsInStage(Vec2 _position, Vec2 _size) {//画面内に長方形が表示されているか
			return reacts(_position, _size, Vec2(0, 0), Vec2(800, 600));
		}
	};
	class Maze
	{
	public:
		const Font font{ FontMethod::MSDF, 48, Typeface::Bold };
		Vec2 dpos{ 0, 0 };
		Vec2 spos{ 1,1 };
		Vec2 gpos{ 1,1 };
		Vec2 pos;
		vector<vector<int>> data;
		int getData(Vec2 _pos) {
			if (0 <= _pos.x && _pos.x <= data.size() - 1 && 0 <= _pos.y && _pos.y <= data[0].size() - 1) {
				return data[_pos.x][_pos.y];
			}
			return -1;
		}

		bool canPutHole(Vec2 _pos, Vec2 _dir) {
			Vec2 vec = _pos + _dir;
			if (getData(vec) == 1)
			{
				vec += _dir;
				if (getData(vec) == 1) {
					return 1;
				}
			}
			return 0;
		}
		int startAux(int value) {
			return (value - 3) / 4 * 2 + 1;
		}
		Maze(int size) {
			make(size);
		}
		void draw(Rotate _rotate)const {
			for (size_t i = 0; i < data[0].size(); i++)
			{
				for (size_t k = 0; k < data.size(); k++)
				{
					if (data[k][i] == 1) {
						_rotate.drawRect(dpos + Vec2(k * 20, i * 20), Vec2(20, 20), HSV{ 230, 0.5, 3 });
					}
					if (data[k][i] == 2) {
						_rotate.drawRect(dpos + Vec2(k * 20, i * 20), Vec2(20, 20), HSV{ 0, 0, 10 });
					}
					if (gpos == Vec2(k, i)) {
						_rotate.drawRect(dpos + Vec2(k * 20, i * 20), Vec2(20, 20), HSV{ 100, 30, 100 });
					}
				}
			}
		}
		void make(int size) {
			data.resize(size);
			for (size_t i = 0; i < size; i++)
			{
				data[i].resize(size);
			}
			for (size_t i = 0; i < size; i++)
			{
				for (size_t k = 0; k < size; k++)
				{
					data[k][i] = 1;
				}
			}
			//pos = Vec2(1, data[0].size() - 2);
			if (size >= 23) {
				pos = Vec2(startAux(data[0].size()), startAux(data.size()));
			}
			else {
				pos = Vec2(1, 1);
			}
			data[pos.x][pos.y] = 0;
			int flag = 0;
			vector <Vec2> dirs;
			vector <Vec2> dir;
			vector <Vec2> clone;
			vector <Vec2> clone2;
			while (flag == 0) {
				dir.clear();
				for (int i = -1; i <= 1; i++)
				{
					for (int k = -1; k <= 1; k++)
					{
						if (abs(k + i) == 1) {
							Vec2 vec = Vec2(k, i);
							if (canPutHole(pos, vec) == 1) {
								dir.push_back(vec);
							}
						}
					}
				}
				if (dir.size() == 0) {
					if (dirs.size() != 0) {
						pos -= 2 * dirs.back();
						dirs.pop_back();
					}
					else {
						flag = 1;
					}
				}
				else {
					int n = Random(0, (int)dir.size() - 1);
					pos += dir[n];
					data[pos.x][pos.y] = 0;
					pos += dir[n];
					data[pos.x][pos.y] = 0;
					dirs.push_back(dir[n]);
					if (pos == spos) {
						if (dirs.size() != 0) {
							pos -= 2 * dirs.back();
							dirs.pop_back();
						}
						else {
							flag = 1;
						}
					}
				}
			}
			pos = spos;
			clone.clear();
			clone.push_back(pos);
			int n = 0;
			data[spos.x][spos.y] = 2;
			int ss = 0;
			while (clone.size() != 0) {
				ss++;
				clone2.clear();
				for (int n = 0; n < clone.size(); n++)
				{
					for (int i = -1; i <= 1; i++)
					{
						for (int k = -1; k <= 1; k++)
						{
							if (abs(k + i) == 1) {
								pos = clone[n];
								Vec2 vec = pos + Vec2(k, i);
								if (getData(vec) == 0) {
									data[vec.x][vec.y] = 2;
									clone2.push_back(vec);
									gpos = vec;
								}
							}
						}
					}
				}
				clone.clear();
				for (int i = 0; i < clone2.size(); i++)
				{
					clone.push_back(clone2[i]);
				}
			}
		}
	};
	class Player {
	public:
		Judge judge;
		Vec2 pos{ 1.1,1.1 };
		Vec2 size{ 0.5,0.5 };
		Vec2 vel{ 0,0 };
		vector<vector<int>> data;
		Vec2 vRound(Vec2 _vec) {
			return Vec2(round(_vec.x), round(_vec.y));
		}
		Vec2 vAbs(Vec2 _vec) {
			return Vec2(abs(_vec.x), abs(_vec.y));
		}
		int getData(Vec2 _pos) {
			if (0 <= _pos.x && _pos.x <= data.size() - 1 && 0 <= _pos.y && _pos.y <= data[0].size() - 1) {
				return data[_pos.x][_pos.y];
			}
			return -1;
		}
		void hitX(Vec2 _pos, Vec2 _size, int direction) {
			if (judge.reacts(_pos, _size, pos, size)) {//プレーヤーとブロックの当たり判定
				pos.x = _pos.x - size.x - (direction - 1) / 2.0 * (size.x + _size.x);
				vel.x = 0;
			}	//↑めり込み補正
		}
		void hitY(Vec2 _pos, Vec2 _size, int direction) {
			if (judge.reacts(_pos, _size, pos, size)) {//プレーヤーとブロックの当たり判定
				pos.y = _pos.y - size.y - (direction - 1) / 2.0 * (size.y + _size.y);
				vel.y = 0;
			}	//↑めり込み補正
		}
		Player(vector<vector<int>> _data, Maze maze) {
			mazeUpdate(_data, maze);
		}
		void update(Rotate _rotate, double _dirVel) {
			double dir = _rotate.angle;
			vel.x += 0.01 * sin(dir);
			vel.y += 0.01 * cos(dir);
			Vec2 length = pos - _rotate.cpos / 20.0;
			double L = sqrt(pow(length.x, 2) + pow(length.y, 2));
			//vel.x += _dirVel * length.y;
			//vel.y += -_dirVel * length.x;
			//
			pos.x += vel.x;
			Vec2 d = vel / vAbs(vel);
			Vec2 p;
			for (int i = -1; i <= 1; i++)
			{
				for (int k = -1; k <= 1; k++)
				{
					p = vRound(pos);
					if (getData(p + Vec2(k, i)) == 1) {
						hitX(p + Vec2(k, i), Vec2(1, 1), d.x);
					}
				}
			}
			pos.y += vel.y;
			for (int i = -1; i <= 1; i++)
			{
				for (int k = -1; k <= 1; k++)
				{
					p = vRound(pos);
					if (getData(p + Vec2(k, i)) == 1) {
						hitY(p + Vec2(k, i), Vec2(1, 1), d.y);
					}
				}
			}
		}
		void mazeUpdate(vector<vector<int>> _data, Maze maze) {
			data = _data;
			pos = maze.spos;
		}
		void draw(Rotate _rotate)const {
			_rotate.drawRect(pos * 20, size * 20, HSV{ 200, 30, 10 });
		}
	};
	class mazeGame : public App::Scene
	{
	public:

		//ここに変数などを宣言
		Maze maze{ 23 };//奇数
		Judge judge;
		Player player{ maze.data,maze };
		Rotate rotate;

		double timer = 0;
		mazeGame(const InitData& init)//名前を変更してください
			: IScene{ init }
		{
			if (not TextureAsset::IsRegistered(U"BackGroundTexture/洞窟背景.png")) {
				TextureAsset::Register(U"BackGroundTexture/洞窟背景.png", U"BackGroundTexture/洞窟背景.png");
			}

			AudioAsset::Register(U"MiniGameBGM", U"BGM/MiniGameBGM.wav", Loop::Yes);
			BGMManager::get().play(U"MiniGameBGM");


			//変数などの初期化

			if (getData().mini_mode == Stage_Mode) {
				maze.make(21);
			}
			else if (getData().mini_mode == Easy_Mode) {
				maze.make(19);
			}
			else if (getData().mini_mode == Normal_Mode) {
				maze.make(23);
			}
			else if (getData().mini_mode == Hard_Mode) {
				maze.make(27);
			}
			player.mazeUpdate(maze.data, maze);
			rotate.cpos = 20 * Vec2{ maze.data[0].size(),maze.data.size() } / 2.0;
			rotate.pos = Vec2{ 1200,800 } / 2.0;

			if (not TextureAsset::IsRegistered(U"ChocolateWall"))
			{
				TextureAsset::Register(U"ChocolateWall", U"StageTexture/ChocolateWall.png");
			}
			TextureAsset::Load(U"ChocolateWall");
		}

		void update() override
		{
			if (judge.reacts(player.pos, player.size, maze.gpos, Vec2(1, 1))) {
				EndGame(true);
				//maze.make(23);
				//rotate.cpos = 20*Vec2{ maze.data[0].size(),maze.data.size() }/2.0;
				//player.mazeUpdate(maze.data,maze);
			}
			//player.vel = Vec2(KeyRight.pressed()-KeyLeft.pressed(), -KeyUp.pressed()+ KeyDown.pressed())/10;
			rotate.angle_vel = 0.02 * ((KeyRight | KeyD).pressed() - (KeyLeft | KeyA).pressed());
			if (MouseL.pressed()) {
				rotate.angle_vel = Cursor::DeltaF().x / 100.0;
			}
			rotate.angle += rotate.angle_vel;
			//rotate.angle = Cursor::Pos().x/100.0;
			player.update(rotate, rotate.angle_vel * 0.1);
			//maze.draw(rotate);
			//player.draw(rotate);
			//変数などの更新
		}

		void draw() const override
		{
			TextureAsset{ U"BackGroundTexture/洞窟背景.png" }.draw();

			//描画(draw関数内ではifの使用、変数の代入などができない)
			maze.draw(rotate);
			player.draw(rotate);
		}

	private:
		void EndGame(bool clear) {
			getData().mini_clear = clear;//クリア状況保存
			if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
			else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
		}
	};

	void Drawing::draw(const Rotate& rotate, const Vec2& pos, const Vec2& size, const Color& color)const
	{
		TextureAsset(assetName).resized(size).rotated(rotate.angle).drawAt(pos, color);
	}
}
