#pragma once

enum mode { Stage_Mode, Easy_Mode, Normal_Mode, Hard_Mode };

struct GameData
{
	double BGMVolume = 1.0;
	double EffectVolume = 1.0;
	int32 stage = 1;
	mode mini_mode = Easy_Mode;
	bool mini_clear = false;
	bool boss_clear = false;

	bool backFromMainGameScene = false;

	//ゲームをやめるでMapに戻ってきたか
	bool quitStage = false;

	int32 maxHP = 5;


	InputGroup KeyUp = s3d::KeyW | s3d::KeyUp;
	InputGroup KeyLeft = s3d::KeyA | s3d::KeyLeft;
	InputGroup KeyDown = s3d::KeyS | s3d::KeyDown;
	InputGroup KeyRight = s3d::KeyD | s3d::KeyRight;

	InputGroup jumpKey = s3d::KeyW | s3d::KeySpace | s3d::KeyUp;
	Input attackKey = s3d::KeyEnter;
	InputGroup leftKey = s3d::KeyA | s3d::KeyLeft;
	InputGroup downKey = s3d::KeyS | s3d::KeyDown;
	InputGroup rightKey = s3d::KeyD | s3d::KeyRight;

	String backgroundTexture;

	String BGMPath;

	FilePath stageFile;
};

using App = SceneManager<String, GameData>;
