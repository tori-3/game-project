#pragma once

enum mode { Stage_Mode, Easy_Mode, Normal_Mode, Hard_Mode };

struct GameData
{
	double BGM_volume = 1.0;
	double Effect_volume = 1.0;
	int stage = 1;
	mode mini_mode = Easy_Mode;
	bool mini_clear = false;
	bool boss_clear = false;

	bool backFromMainGameScene = false;

	//ゲームをやめるでMapに戻ってきたか
	bool quitStage = false;

	int32 maxHP = 5;


	Input KeyUp = s3d::KeyW;
	Input KeyLeft = s3d::KeyA;
	Input KeyDown = s3d::KeyS;
	Input KeyRight = s3d::KeyD;

	InputGroup jumpKey=s3d::KeyW| s3d::KeySpace;
	Input attackKey=s3d::KeyEnter;
	Input leftKey=s3d::KeyA;
	Input rightKey = s3d::KeyD;
	Input downKey = s3d::KeyS;

	String backgroundTexture;

	String BGMPath;

	FilePath stageFile;
};

using App = SceneManager<String, GameData>;
