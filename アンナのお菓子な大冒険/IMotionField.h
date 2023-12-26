#pragma once

class Move;

class FieldContainer;

class IMotionField {
public:
	virtual String getValueString()const = 0;
};

template<class T>
class MotionField :public IMotionField
{
public:
	MotionField(T init)
		:value(init) {}

	String getValueString()const {
		return Format(value);
	}

	T getValue()const
	{
		return value;
	}

	void setValue(const T& v)
	{
		value = v;
	}

	void operator = (const T& v)
	{
		setValue(v);
	}
private:
	T value;
};

class FieldContainer
{
public:
	//first-> paramName
	HashTable<String, IMotionField*> table;

	String region = U"";

	~FieldContainer()
	{
		for (auto& [key, value] : table)delete value;
	}

	template<class T>
	MotionField<T>& get(const String& name)
	{
		return *dynamic_cast<MotionField<T>*>(table[name]);
	}
};
