﻿# include"Common.h"
# include"MainGameScene.h"
#include"Map.h"
#include"FallingAnna.h"

#include"MAZE.h"
#include"Maze2.h"
#include"ManjuRush.h"
#include"HawkDropOut.h"
#include"AnnaMusicGame.h"
#include"MiniGameSelect.h"
#include"EndRollScene.h"

#include"TitleScene.h"

#include"LoadAsset.h"

void Main()
{
	Window::SetTitle(U"アンナのお菓子な大冒険");

	Window::Resize(1200, 800);

	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);

	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 60, Typeface::Heavy);

	App manager;

	manager.add<TitleScene>(U"TitleScene");
	manager.add<Map>(U"Map");
	manager.add<MainGameScene>(U"MainGameScene");
	manager.add<MiniGameSelect>(U"MiniGameSelect");
	manager.add<FallingAnna::FallingAnna>(U"FallingAnna");
	manager.add<HawkDropOut::HawkDropOut>(U"HawkDropOut");
	manager.add<ManjuRush::ManjuRush>(U"ManjuRush");
	manager.add<Maze1::MAZE>(U"Maze1");
	manager.add<Maze1::Clear>(U"Clear");
	manager.add<Maze2::mazeGame>(U"Maze2");
	manager.add<AnnaMusicGame::AnnaMusicGame>(U"AnnaMusicGame");
	manager.add<EndRollScene>(U"EndRollScene");

	//manager.init(U"EndRollScene");

	LoadAsset::LoadAudio();

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
