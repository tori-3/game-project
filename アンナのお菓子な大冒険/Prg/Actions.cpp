#include "../stdafx.h"
#include "Actions.h"

using namespace prg;

Actions::Actions()
	:IAction()
{
	endCondition->set<prg::FuncCondition>(
		[=] {
			return isAllFinished();
		}
	);
}

Actions::~Actions()
{
	for (auto& action : update_list)delete action;
}

int32 Actions::getIndex(IAction* action)
{
	int32 count = 0;
	for (auto& a : update_list)
	{
		if (a == action)return count;
		++count;
	}
	return -1;
}
IAction* Actions::getAction(const String& id)
{
	for (auto& action : update_list)if (action->id == id)return action;
	//見つからなければ
	return nullptr;
}

IAction* Actions::getAction(int32 index)
{
	if (0 <= index and index <= update_list.size() - 1) return update_list[index];

	return nullptr;
}

int32 Actions::getLoopCount()const
{
	return loopCount;
}

void Actions::restart()
{
	reset();
	start();
}

void Actions::start()
{
	IAction::start();
	activeIndex = 0;
	//最初のアクションをスタートさせる
	const auto& [start, end] = _getArea(activeIndex);
	for (auto it = update_list.begin() + start, en = update_list.begin() + end; it != en; ++it)
	{
		auto& act = (*it);
		if ((not act->started) and (not act->ended) and act->startCondition->check())
		{
			act->start();
		}
	}
}

void Actions::reset()
{
	IAction::reset();
	activeNum = update_list.size();
	activeIndex = 0;
	loopCount = 0;
	for (auto& action : update_list)
	{
		action->reset();
		endTimeList[action] = none;
	}
}

bool Actions::isAllFinished()
{
	return activeNum == 0;
}

void Actions::end()
{
	IAction::end();
	//実行中のアクションは終了させる
	for (auto& action : update_list)
	{
		if (action->isActive())action->end();
	}

	if (loop)
	{
		int32 temp = ++loopCount;
		reset();
		start();
		loopCount = Min(temp, maxLoopCount);
		return;
	}

	if (initialize)reset();
}

void Actions::update(double dt)
{
	if (stopped)return;

	dt *= timeScale;
	IAction::update(dt);

	const auto& [st, en] = _getArea(activeIndex);
	//優先度でソート
	std::stable_sort(update_list.begin()+st, update_list.begin()+en, [](const IAction* ac1, const IAction* ac2) {
		return ac1->updatePriority < ac2->updatePriority;
	});
	//終了したアクションの終了してからの経過時間を更新
	for (auto& endTime : endTimeList)if (endTime.second.has_value()) *endTime.second += dt;

	for (auto it = update_list.begin() + st, end = update_list.begin() + en; it != end; ++it)
	{
		auto& act = (*it);

		if ((not act->started) and (not act->ended) and act->startCondition->check())
		{
			act->start();
		}

		if (not act->isActive())continue;

		act->update(dt);

		if (act->endCondition->check())
		{
			act->end();//actionがループアクションならここでisEndedがfalseにならないので下の処理は行わない。
			if (act->isEnded())
			{
				endTimeList[act] = 0;
				activeNum--;
			}
		}
	}

	if (_isEnded({ st,en }))
	{
		++activeIndex;
	}
}

bool prg::Actions::_isEnded(std::tuple<int32, int32>se) const
{
	const auto& [start, end] = se;
	for (auto it = update_list.begin() + start, en = update_list.begin() + end; it != en; ++it)
		if (not (*it)->isEnded())
			return false;
	return true;
}

std::tuple<int32, int32> prg::Actions::_getArea(const int32& activeIndex) const
{
	return std::tuple<int32, int32>(
		separate[activeIndex],
		separate.size() - 1 > activeIndex ? separate[activeIndex + 1] : separate[activeIndex]);
}

