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
	Input KeyUp = s3d::KeyUp;
	Input KeyLeft = s3d::KeyLeft;
	Input KeyDown = s3d::KeyDown;
	Input KeyRight = s3d::KeyRight;

	InputGroup jumpKey=s3d::KeyW| s3d::KeySpace;
	Input attackKey=s3d::KeyEnter;
	Input leftKey=s3d::KeyA;
	Input rightKey = s3d::KeyD;
	Input downKey = s3d::KeyS;



	FilePath stageFile;
};

using App = SceneManager<String, GameData>;
