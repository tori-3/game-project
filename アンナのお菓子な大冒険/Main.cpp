# include"Common.h"
# include"MainGameScene.h"
#include"Map.h"


void Main()
{

	Window::Resize(1200, 800);

	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);

	App manager;

	manager.add<Map>(U"Map");
	manager.add<MainGameScene>(U"MainGameScene");

	while (System::Update())
	{
		// 現在のシーンを実行
		// シーンに実装した .update() と .draw() が実行される
		if (not manager.update())
		{
			break;
		}
	}
}
