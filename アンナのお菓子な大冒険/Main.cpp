# include"Common.h"
# include"MainGameScene.h"
#include"Map.h"
#include"FallingAnna.h"

#include"MAZE.h"
#include"Maze2.h"
#include"ManjuRush.h"
#include"HawkDropOut.h"

void Main()
{

	Window::Resize(1200, 800);

	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);

	App manager;

	manager.add<Map>(U"Map");
	manager.add<MainGameScene>(U"MainGameScene");
	manager.add<FallingAnna::FallingAnna>(U"FallingAnna");
	manager.add<HawkDropOut::HawkDropOut>(U"HawkDropOut");
	manager.add<ManjuRush::ManjuRush>(U"ManjuRush");
	manager.add<Maze1::MAZE>(U"Maze1");
	manager.add<Maze1::Game>(U"Game");
	manager.add<Maze1::Clear>(U"Clear");
	manager.add<Maze2::mazeGame>(U"Maze2");


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
