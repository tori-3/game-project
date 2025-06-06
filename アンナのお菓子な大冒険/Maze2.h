﻿#pragma once
#include"MiniGameSceneBase.h"
#include"BGMManager.hpp"

namespace Maze2
{
	using namespace std;

	class Rotate;

	class Rotate {
	public:
		Vec2 pos{ 0,0 };
		Vec2 cpos{ 0,0 };
		double angle = 0;
		double angle_vel = 0;

		Vec2 vec(Vec2 _vec) {
			return Vec2(_vec.x * cos(angle) - _vec.y * sin(angle), _vec.x * sin(angle) + _vec.y * cos(angle));
		}
		void drawRect(Vec2 _pos, Vec2 _size, Color _color) {
			draw(U"CakeGround" ,*this, pos + vec(_pos + Vec2(_size.y / 2.0, _size.y / 2.0) - cpos), _size + Vec2{ 1,1 }, _color);

			//Line{ pos + vec(_pos + Vec2(_size.y / 2.0,_size.y / 2.0) - cpos) ,pos + vec(_pos + Vec2(_size.x - _size.y / 2.0 + 0.01,_size.y / 2.0) + -cpos) }.draw(_size.y + 0.00001, _color);
		}

		void drawRectPlayer(Vec2 _pos, Vec2 _size, Color _color){
			draw(U"Cloud", *this, pos + vec(_pos + Vec2(_size.y / 2.0, _size.y / 2.0) - cpos), _size + Vec2{ 1,1 }, _color);
		}

		void draw(AssetNameView assetName,const Rotate& rotate, const Vec2& pos, const Vec2& size, const Color& color)const
		{
			TextureAsset(assetName).resized(size).rotated(rotate.angle).drawAt(pos, color);
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
						//HSV{ 0, 0, 10 }
						_rotate.drawRect(dpos + Vec2(k * 20, i * 20), Vec2(20, 20), ColorF{1});
					}
					if (data[k][i] == 2) {
						_rotate.drawRect(dpos + Vec2(k * 20, i * 20), Vec2(20, 20),ColorF{0.7,0.5});
					}
					if (gpos == Vec2(k, i)) {
						_rotate.drawRect(dpos + Vec2(k * 20, i * 20), Vec2(20, 20), Palette::Red);
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
			vel.x += Scene::DeltaTime() * sin(dir);
			vel.y += Scene::DeltaTime() * cos(dir);
			Vec2 length = pos - _rotate.cpos / 20.0;
			double L = sqrt(pow(length.x, 2) + pow(length.y, 2));
			//vel.x += _dirVel * length.y;
			//vel.y += -_dirVel * length.x;
			//

			const Vec2 preVel = vel;

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

			if (Scene::DeltaTime()*2 <(preVel - vel).length())
			{
				AudioAsset{ U"ブロックを置く" }.playOneShot();
			}
		}
		void mazeUpdate(vector<vector<int>> _data, Maze maze) {
			data = _data;
			pos = maze.spos;
		}
		void draw(Rotate _rotate)const {
			_rotate.drawRectPlayer(pos * 20, size * 20, Palette::White);
		}
	};
	class mazeGame : public MiniGameSceneBase
	{
	public:

		Spotlight light;

		Maze maze{ 23 };//奇数
		Judge judge;
		Player player{ maze.data,maze };
		Rotate rotate;

		double timer = 0;

		bool clear = false;

		Audio clearAudio{ U"MiniGameAsset/Common/クリア2.wav" };

		mazeGame(const InitData& init);

		void gameUpdate() override;

		void gameDraw() const override;

	};
}
