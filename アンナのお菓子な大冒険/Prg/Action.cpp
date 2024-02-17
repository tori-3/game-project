#include "../stdafx.h"
#include "Action.h"
#include"Condition.h"
#include"Actions.h"

using namespace prg;

IAction::IAction(double t)
	:updatePriority(0)
	, time(0)
	, timeScale(1.0)
	, id(U"")
	, startCondition(new ConditionArray())
	, endCondition(new ConditionArray())
{
	endCondition->add<FuncCondition>(
		[=] {
			return time >= t;
		}
	);
}

IAction::~IAction()
{
	if (startCondition != nullptr) delete startCondition;
	if (endCondition != nullptr) delete endCondition;
}

void IAction::setStartCondition(ConditionArray* condition)
{
	if (startCondition != nullptr)delete startCondition;
	startCondition = condition;
}

void IAction::setEndCondition(ConditionArray* condition)
{
	if (endCondition != nullptr)delete endCondition;
	endCondition = condition;
}

void IAction::startIf(IAction* action, ActionSide state)
{
	startCondition->set<prg::FuncCondition>([=] {
		return state == ActionSide::start ? action->isStarted() : action->isEnded();
		});
}

void IAction::endIf(IAction* action, ActionSide state)
{
	endCondition->set<prg::FuncCondition>([=] {
		return state == ActionSide::start ? action->isStarted() : action->isEnded();
		});
}

bool IAction::isStarted()
{
	return started;
}

bool IAction::isEnded()
{
	return ended;
}

bool IAction::isActive()
{
	return started and (not ended);
}

void IAction::update(double dt)
{
	if (startCondition == nullptr)startCondition = new ConditionArray();
	if (endCondition == nullptr)endCondition = new ConditionArray();

	time += dt * timeScale;
};

void IAction::reset()
{
	if (startCondition != nullptr)startCondition->reset();
	if (endCondition != nullptr)endCondition->reset();

	time = 0;
	started = false;
	ended = false;
}

MyPrint::MyPrint(const String& text, double t)
	:text(text), IAction(t)
{}

void MyPrint::update(double dt)
{
	IAction::update(dt);
	Print << text;
}

FuncAction::FuncAction(const std::function<void(double, Self)>& upd, double time)
	:upd(upd), IAction(time) {}

FuncAction::FuncAction(const std::function<void()>& ini,const std::function<void(double, Self)>& upd,double time)
	:ini(ini),upd(upd),IAction(time) {}

FuncAction::FuncAction(const std::function<void()>& ini,double time)
	:ini(ini),IAction(time) {}

void FuncAction::update(double dt)
{
	IAction::update(dt);

	upd(dt, this);
}

void FuncAction::start()
{
	IAction::start();

	ini();
}

void FuncAction::end()
{
	IAction::end();

	fin();
}

