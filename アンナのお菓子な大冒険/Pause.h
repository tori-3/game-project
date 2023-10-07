#pragma once

using App = SceneManager<String>;

// タイトルシーン
class IPauseScene : public App::Scene
{
public:

	bool pause = false;
	bool pauseFlg = false;

	Stopwatch time{ StartImmediately::Yes };

	// コンストラクタ（必ず実装）
	IPauseScene(const InitData& init)
		: IScene{ init }
	{

	}

	virtual void gameUpdate() {
	}

	virtual void gameDraw()const {
	}

	virtual void pauseUpdate() {
	}

	virtual void pauseDraw()const {
	}

	void goPause() {
		pauseFlg = true;
	}

	void goGame() {
		pauseFlg = false;
	}

	void update() override
	{
		if (pause != pauseFlg) {
			pause = pauseFlg;
			if (pause)time.pause();
			else time.resume();
		}
		if (not pause) {
			gameUpdate();
			DataManager::get().time += Scene::DeltaTime();
		}
		else pauseUpdate();
	}

	void draw() const override
	{
		gameDraw();
		if (pause)pauseDraw();
	}
};
