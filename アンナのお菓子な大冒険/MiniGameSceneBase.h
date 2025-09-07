#pragma once
#include"Common.h"

using App = SceneManager<String, GameData>;

class MiniGameSceneBase : public App::Scene
{
public:

	UIManager uiManager{ Scene::Rect() };

	bool pause = false;
	bool pauseFlg = false;

	MiniGameSceneBase(const InitData& init)
		: IScene{ init }
	{

	}

	std::shared_ptr<UIElement> createPauseUI();

	virtual void gameUpdate();

	virtual void gameDraw()const;

	virtual void onPauseStart();

	virtual void onGameStart();

	void pauseUpdate();

	void pauseDraw()const;

	void goPause();

	void EndGame(bool clear);

	void goGame();

	void update() override;

	void draw() const override;

	void stageClear()
	{
		if(m_saveClearFlg)
		{
			return;
		}

		getData().saveMiniGameClear();

		m_saveClearFlg = true;
	}

private:

	LongPressInput leftInput;
	LongPressInput rightInput;
	int32 m_settingSelectIndex = 0;

	bool m_saveClearFlg = false;
};
