#pragma once

using App = SceneManager<String, GameData>;

class MiniGameSceneBase : public App::Scene
{
public:

	UIManager uiManager{ Scene::Rect() };

	bool pause = false;
	bool pauseFlg = false;

	// コンストラクタ（必ず実装）
	MiniGameSceneBase(const InitData& init)
		: IScene{ init }
	{

	}

	virtual void gameUpdate()
	{

	}

	virtual void gameDraw()const
	{

	}

	virtual void onPauseStart()
	{

	}

	virtual void onGameStart()
	{

	}

	void pauseUpdate() {
		uiManager.update();

		if (uiManager.getChildren().size() == 0)
		{
			goGame();
		}
	}

	void pauseDraw()const {
		uiManager.draw();
	}

	void goPause() {
		onPauseStart();

		pauseFlg = true;

		auto continueButton = SimpleButton::Create({ .child = TextUI::Create({.text = U"ゲームを続ける",.color = Palette::White}) });
		auto endButton = SimpleButton::Create({ .child = TextUI::Create({.text = U"ゲームをやめる",.color = Palette::White}) });
		auto settingButton = SimpleButton::Create({ .child = TextUI::Create({.text = U"設定",.color = Palette::White}) });

		uiManager.addChild
		({
			SimpleDialog::Create
			({
				.child = RectPanel::Create
				({
					.padding = 30,
					.margine = 30,
					.child = Row::Create
					({
						.children
						{
							continueButton,
							endButton,
							settingButton
						}
					})
				}),
				.updateFunc=[=](SimpleDialog* dialog)
				{
					if(continueButton->clicked())
					{
						dialog->close();
					}
					if(endButton->clicked())
					{
						EndGame(false);
					}
					if(settingButton->clicked())
					{
						
					}
				}
			})
		});
	}

	void EndGame(bool clear) {
		getData().mini_clear = clear;//クリア状況保存
		if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
		else changeScene(U"Mini_Game_Select");//ミニゲームセレクトモードならミニゲームセレクトに帰る
	}

	void goGame() {
		pauseFlg = false;
		onGameStart();
	}

	void update() override
	{
		if (pause != pauseFlg)
		{
			pause = pauseFlg;
		}
		if (not pause)
		{
			gameUpdate();
			if(KeyEscape.down())
			{
				goPause();
			}
		}
		else pauseUpdate();
	}

	void draw() const override
	{
		gameDraw();
		if (pause)pauseDraw();
	}
};
