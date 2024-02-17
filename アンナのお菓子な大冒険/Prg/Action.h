#pragma once

namespace prg
{
	class ConditionArray;

	class Actions;

	enum ActionSide
	{
		start, end
	};

	class IAction
	{
	public:		
		IAction(double time=Math::Inf);

		virtual ~IAction();

		void setStartCondition(ConditionArray* condition);

		void setEndCondition(ConditionArray* condition);
		//第一引数のアクションが開始(state=start)または終了(state=end)なら開始
		void startIf(IAction* action, ActionSide state);
		//第一引数のアクションが開始(state=start)または終了(state=end)なら終了
		void endIf(IAction* action, ActionSide state);

		bool isStarted();

		bool isEnded();

		bool isActive();

		double updatePriority;

		double time;

		double timeScale;

		ConditionArray* startCondition = nullptr;
		ConditionArray* endCondition = nullptr;
	protected:
		String id;
		bool started = false;
		bool ended = false;
		friend Actions;

		virtual void reset();

		virtual void start()
		{
			started = true;
		};
		virtual void end()
		{
			ended = true;
		};

		virtual void update(double dt);
	};
	//IActionはWaitとしても使える
	//using Wait = IAction;

	class MyPrint final :public IAction
	{
	public:
		MyPrint(const String& text,double time=Math::Inf);

		String text;

	private:
		void update(double dt);
	};

	class FuncAction :public IAction
	{
	public:
		using IAction::IAction;

		using Self = FuncAction*;
		std::function<void(double, Self)> upd = [] (double,Self){};
		std::function<void()> ini = [] {};
		std::function<void()> fin = [] {};

		FuncAction(const std::function<void(double, Self)>& upd, double time = Math::Inf);

		FuncAction(const std::function<void()>& ini, const std::function<void(double, Self)>& upd, double time = Math::Inf);

		FuncAction(const std::function<void()>& ini, double time = 0);
	private:
		void update(double dt);
		void start();
		void end();
	};

	
}
