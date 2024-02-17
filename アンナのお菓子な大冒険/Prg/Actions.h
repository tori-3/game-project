#pragma once
#include "Action.h"
#include"Condition.h"

namespace prg
{
	class Actions :public IAction
	{
	public:
		Actions();

		~Actions();

		template<class T, int32 Index = -1, class... Args>
		T* add(Args&&... args)
		{
			auto action = new T(args...);

			add<Index>(action);

			return action;
		}

		template<int32 Index = -1>
		IAction* add(IAction* action)
		{
			int32 n = Index >= int32(separate.size()) ? separate.size() - 1 : Index;

			if (n < 0)n = separate.size() + n;

			update_list.insert(update_list.begin() + separate[n], action);

			separate << 0;

			for (auto it = separate.rbegin(), en = separate.rbegin() + (separate.size() - n - 1); it != en; ++it)
			{
				//一つ前の値+1して後ろにシフト
				*it = *(it + 1) + 1;
			}

			endTimeList[action] = none;

			++activeNum;

			return action;
		}

		template<class T, int32 Index = -1, class... Args>
		T* addParallel(Args&&... args)
		{
			auto action = new T(args...);

			addParallel(action);

			return action;
		}

		template<int32 Index = -1>
		IAction* addParallel(IAction* action)
		{
			if (update_list.isEmpty() and Index == -1)return add<Index>(action);

			int32 n = Index >= int32(separate.size()) ? separate.size() - 1: Index;

			if (n < 0)n = separate.size() + n;

			update_list.insert(update_list.begin() + separate[n], action);

			for (auto itr = separate.begin() + n, en = separate.end(); itr != en; ++itr)
			{
				++(*itr);
			}

			endTimeList[action] = none;

			activeNum++;

			return action;
		}

		int32 getIndex(IAction* action);

		//int32 getVertualIndex(IAction* action);

		//Array<IAction*> getActionLayer(const int32& vertual_index);

		IAction* getAction(const String& id);

		IAction* getAction(int32 index);

		int32 getLoopCount()const;
		/// @brief アクションをリセットしてから開始する
		void restart();
		/// @brief アクションを開始する
		void start()override;
		/// @brief アクションをリセット
		void reset()override;

		void end()override;

		bool isAllFinished();

		void update(double dt)override;
		//終了したときにリセットを行う
		bool initialize = true;
		//終了したときに再開する
		bool loop = false;
		//停止する
		bool stopped = false;
		//ループカウントの上限　別にカンストしてもループは続く
		const int32 maxLoopCount = 10000;
	private:
		bool _isEnded(std::tuple<int32, int32> se)const;

		std::tuple<int32, int32> _getArea(const int32& activeIndex)const;

		int32 activeIndex = 0;
		//HashTable<IAction*,ActIndex> ind_list;
		//終了していないIActionがいくつあるか addしたときに++ updateでendのとき-- resetのとき=
		int32 activeNum = 0;
		//secondにはアクションが終わってからの経過時間を入れる
		HashTable<IAction*,Optional<double>> endTimeList;
		//HashTableにしてendTimeListの役割も兼ねるようにしたかったけど,addは.backを使うのと,HashTableのソートがうまく機能するのかわからなかったのでやめた。
		Array<IAction*> update_list;
		Array<IAction*> draw_list;
		//仕切り
		Array<int32> separate{ 0 };

		int32 loopCount = 0;
	};	
}
