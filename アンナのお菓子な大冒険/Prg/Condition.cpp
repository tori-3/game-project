#include "../stdafx.h"
#include "Condition.h"
#include"Action.h"

using namespace prg;

ICondition::ICondition()
	:flag(false)
{
}

void ICondition::forced(bool _flag)
{
	flag = _flag;
}

void ICondition::reset()
{
	flag = false;
}

bool ICondition::check()
{
	return flag;
}

bool ConditionArray::check()
{
	if (flag)return true;
	if (conditionArray.isEmpty())return true;//無条件

	bool tmp = checkType == Type::Any;

	for (auto& condition : conditionArray)
	{
		if (tmp == condition->check())return tmp;
	}

	return not tmp;
}

void ConditionArray::reset()
{
	ICondition::reset();
	for (auto& a : conditionArray)a->reset();
}
