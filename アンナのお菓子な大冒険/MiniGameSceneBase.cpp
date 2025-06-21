#include"Common.h"
#include"MiniGameSceneBase.h"
#include"SweetsPanel.hpp"

void MiniGameSceneBase::gameUpdate()
{

}

void MiniGameSceneBase::gameDraw()const
{

}

void MiniGameSceneBase::onPauseStart()
{

}

void MiniGameSceneBase::onGameStart()
{

}

void MiniGameSceneBase::pauseUpdate()
{
	uiManager.update();

	if (uiManager.getChildren().size() == 0)
	{
		goGame();
	}
}

void MiniGameSceneBase::pauseDraw()const
{
	uiManager.draw();
}

std::shared_ptr<UIElement> MiniGameSceneBase::createPauseUI()
{
	auto redoButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 200,.child = TextUI::Create({.text = U"やり直す",.color = Palette::White}) });
	auto endButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 200, .child = TextUI::Create({.text = U"やめる",.color = Palette::White}) });
	auto settingButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 200,.child = TextUI::Create({.text = U"設定",.color = Palette::White}) });
	auto continueButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 20,.margine = 10,.width = 200,.child = TextUI::Create({.text = U"続ける",.color = Palette::White}) });

	leftInput = LongPressInput{ getData().minigameLeftKey };
	rightInput = LongPressInput{ getData().minigameRightKey };

	redoButton->selected = m_settingSelectIndex == 0;
	endButton->selected = m_settingSelectIndex == 1;
	settingButton->selected = m_settingSelectIndex == 2;
	continueButton->selected = m_settingSelectIndex == 3;

	return SimpleDialog::Create
	({
		.child = SweetsPanel::Create
		({
			.margine = 30,
			.relative = Relative::Stretch(),
			.child =Column::Create
			({
				.mainAxis=MainAxis::spaceBetween,
				.children
				{
					TextUI::Create({.text=U"ポーズ",.fontSize = 40,.color=Palette::White}),
					TextUI::Create({.text = getData().description,.fontSize = 30,.color = Palette::White,.flex = 1,}),
					Row::Create
					({
						.children
						{
							redoButton,
							endButton,
							settingButton,
							continueButton,
						}
					})
				}
			})
		}),
		.updateFunc = [=](SimpleDialog* dialog)
		{
			if(leftInput.down())
			{
				if(0<m_settingSelectIndex)
				{
					--m_settingSelectIndex;
					AudioAsset{ U"カーソル移動" }.playOneShot();
				}
				else
				{
					AudioAsset{ U"ビープ音" }.playOneShot();
				}
			}

			if (rightInput.down())
			{
				if (m_settingSelectIndex<3)
				{
					++m_settingSelectIndex;
					AudioAsset{ U"カーソル移動" }.playOneShot();
				}
				else
				{
					AudioAsset{ U"ビープ音" }.playOneShot();
				}
			}

			redoButton->selected = m_settingSelectIndex == 0;
			endButton->selected = m_settingSelectIndex == 1;
			settingButton->selected = m_settingSelectIndex == 2;
			continueButton->selected = m_settingSelectIndex == 3;

			if (redoButton->clicked() || (redoButton->selected && getData().menuDecisionKey.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();
				changeScene(getData().sceneName);
			}
			else if (continueButton->clicked()||(continueButton->selected&& getData().menuDecisionKey.down()))
			{
				AudioAsset{ U"キャンセル" }.playOneShot();
				dialog->close();
			}
			else if (endButton->clicked() || (endButton->selected && getData().menuDecisionKey.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();
				EndGame(false);
			}
			else if (settingButton->clicked() || (settingButton->selected && getData().menuDecisionKey.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();
				uiManager.addChild(
					SettingWindow(getData().minigameUpKey, getData().minigameDownKey, getData().minigameLeftKey, getData().minigameRightKey, [=] {uiManager.addChild(createPauseUI()); getData().save(); },getData(), uiManager)
				);
				dialog->close();
			}
		}
	});
}


void MiniGameSceneBase::goPause()
{
	if (not pauseFlg)
	{
		onPauseStart();

		pauseFlg = true;
		m_settingSelectIndex = 3;

		uiManager.addChild(createPauseUI());
	}
}

void MiniGameSceneBase::EndGame(bool clear)
{
	getData().mini_clear = clear;//クリア状況保存
	if (getData().mini_mode == Stage_Mode)changeScene(U"Map");//ステージモードならステージに帰る
	else changeScene(U"MiniGameSelect");//ミニゲームセレクトモードならミニゲームセレクトに帰る
}

void MiniGameSceneBase::goGame()
{
	pauseFlg = false;
	onGameStart();
}

void MiniGameSceneBase::update()
{
	if (pause != pauseFlg)
	{
		pause = pauseFlg;
	}
	if (not pause)
	{
		gameUpdate();
		if (getData().pauseKey.down())
		{
			goPause();
		}
	}
	else pauseUpdate();
}

void MiniGameSceneBase::draw() const
{
	gameDraw();
	if (pause)pauseDraw();
}
