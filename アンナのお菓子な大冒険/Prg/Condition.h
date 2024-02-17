#pragma once

namespace prg
{
	class ICondition
	{
	public:
		ICondition();
		virtual ~ICondition() {};
		void forced(bool flag = true);

		virtual bool check();

		virtual void reset();
	protected:
		bool flag;
	};

	enum class Type {
		Every,
		Any
	};

	class ConditionArray:public ICondition
	{
	public:
		virtual bool check();

		template<class T,class ...Args>
		void add(Args&& ...args)
		{
			conditionArray << new T(args...);
		};

		template<class T, class ...Args>
		void set(Args&& ...args)
		{
			for (auto& tmp : conditionArray)delete tmp;
			conditionArray = Array<ICondition*>{ new T(args...)};
		}

		void reset()override;

		Array<ICondition*> conditionArray;
		
		//Every すべてのconditionがtrueならtrue
		//Any いずれかのconditionがtrueならtrue
		Type checkType=Type::Every;
	};

	class ConditionSwitch :public ICondition
	{
	public:
		ConditionSwitch(bool init = false);
		void on();
		void off();
		void turn();
		void reset()override;
	private:
		bool init;
	};

	//関数でチェック
	class FuncCondition :public ICondition
	{
	public:
		//ラムダ式などで関数を渡す　関数の戻り値はbool
		FuncCondition(std::function<bool()> _function) :m_function{ _function } {}

		bool check()
		{
			return flag or m_function();
		};
	
		std::function<bool()> m_function;
	};
}
