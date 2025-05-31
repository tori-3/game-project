#include"Common.h"
#include"SweetsPanel.hpp"
#include"SettingWindow.h"
#include"SoundIcon.hpp"

std::shared_ptr<UIElement> SettingWindow(const InputGroup& upInputGroup, const InputGroup& downInputGroup, const InputGroup& leftInputGroup, const InputGroup& rightInputGroup, const std::function<void()>& onClose,GameData& gameData,UIManager& manager,size_t index)
{
	auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 20,.margine = 10,.width = 200, .child = TextUI::Create({.text = U"閉じる",.color = Palette::White}) });
	auto effectVolumeSlider = SimpleSlider::Create({ .value = GlobalAudio::BusGetVolume(EffectMixBus) });
	auto BGMVolumeSlider = SimpleSlider::Create({ .value = GlobalAudio::BusGetVolume(BGMMixBus) });
	auto effectVolumeIcon = SoundIcon::Create({ .volume = effectVolumeSlider->value });
	auto BGMVolumeIcon = SoundIcon::Create({ .volume = BGMVolumeSlider->value });

	auto effectVolumeSliderPanel = RectPanel::Create
	({
		.color=Palette::Skyblue,
		.margine = 10,
		.child = Row::Create
		({
			.margine=10,
			.children
			{
				TextUI::Create({.text=U"効果音 音量",.color=Palette::White,.width = 150}),
				effectVolumeSlider,
				effectVolumeIcon
			}

		})
	});

	auto BGMVolumeSliderPanel = RectPanel::Create
	({
		.color = AlphaF(0),
		.margine = 10,
		.child = Row::Create
		({
			.margine = 10,
			.children
			{
				TextUI::Create({.text=U"BGM 音量",.color = Palette::White,.width = 150}),
				BGMVolumeSlider,
				BGMVolumeIcon
			}

		})
	});

	String text = Window::GetState().fullscreen ? U"ウィンドウにする" : U"全画面にする";
	auto windowModeButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = text,.color=Palette::White}) });


	String text2 = gameData.getIncreaseHPMode() ? U"お助けモードを\nOFFにする" : U"お助けモードを\nONにする";
	auto hpModeButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = text2,.color = Palette::White}) });

	auto infoButton = ChocolateButton::Create({ .color = Palette::White, .padding = -20,.width=50,.height=50, .child = TextUI::Create({.text = U"i",.color = Palette::Gray}) });


	static int32 selectIndex = 0;
	static LongPressInput upInput;
	static LongPressInput downInput;

	selectIndex = index;
	upInput = LongPressInput{ upInputGroup };
	downInput = LongPressInput{ downInputGroup };

	effectVolumeSliderPanel->color = selectIndex == 0 ? ColorF{ Palette::Skyblue } : AlphaF(0);
	BGMVolumeSliderPanel->color = selectIndex == 1 ? ColorF{ Palette::Skyblue } : AlphaF(0);
	windowModeButton->selected = selectIndex == 2;
	hpModeButton->selected = selectIndex == 3;
	closeButton->selected = selectIndex == 4;

	return SimpleDialog::Create
	({
		.erasable = false,
		.child = SweetsPanel::Create
		({
			.child = Column::Create
			({
				.margine = 20,
				.children
				{
					TextUI::Create({.text = U"設定",.fontSize = 40,.color = Palette::White}),
					effectVolumeSliderPanel,
					BGMVolumeSliderPanel,
					windowModeButton,
					Row::Create
					({
						.children
						{
							RectUI::Create({.size{50,50},.color=Alpha(0)}),
							hpModeButton,
							infoButton
						}
					}),
					closeButton
				}
			}),
		}),
		.updateFunc = [=](SimpleDialog* dialog)mutable
		{
			GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
			GlobalAudio::BusSetVolume(BGMMixBus, BGMVolumeSlider->value);
			effectVolumeIcon->volume = effectVolumeSlider->value;
			BGMVolumeIcon->volume = BGMVolumeSlider->value;

			if (upInput.down())
			{
				if(0<selectIndex)
				{
					AudioAsset{ U"カーソル移動" }.playOneShot();
					--selectIndex;
				}
				else
				{
					AudioAsset{ U"ビープ音" }.playOneShot();
				}
			}

			if (downInput.down())
			{
				if (selectIndex<4)
				{
					AudioAsset{ U"カーソル移動" }.playOneShot();
					++selectIndex;
				}
				else
				{
					AudioAsset{ U"ビープ音" }.playOneShot();
				}
			}

			effectVolumeSliderPanel->color = selectIndex == 0 ? ColorF{ Palette::Skyblue } : AlphaF(0);
			BGMVolumeSliderPanel->color = selectIndex == 1 ? ColorF{ Palette::Skyblue } : AlphaF(0);
			windowModeButton->selected = selectIndex == 2;
			hpModeButton->selected = selectIndex == 3;
			closeButton->selected = selectIndex == 4;

			if (selectIndex == 0)
			{
				effectVolumeSlider->value = Clamp(effectVolumeSlider->value + (rightInputGroup.pressed() - leftInputGroup.pressed()) * Scene::DeltaTime(), 0.0, 1.0);

				if (leftInputGroup.up()||rightInputGroup.up())
				{
					GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
					AudioAsset{ U"パンチヒット" }.playOneShot();
				}
			}
			else if (selectIndex == 1)
			{
				BGMVolumeSlider->value = Clamp(BGMVolumeSlider->value + (rightInputGroup.pressed() - leftInputGroup.pressed()) * Scene::DeltaTime(), 0.0, 1.0);
			}

			if (effectVolumeSlider->sliderReleased())
			{
				GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
				AudioAsset{ U"パンチヒット" }.playOneShot();
			}

			if (windowModeButton->clicked() || (selectIndex == 2 && KeyEnter.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();

				const bool isFullscreen = Window::GetState().fullscreen;
				windowModeButton->setChild(TextUI::Create({.text = isFullscreen ? U"全画面にする":U"ウィンドウにする",.color=Palette::White  }));
				Window::SetFullscreen(not isFullscreen);
			}


			if (infoButton->clicked())
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();
				auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 20,.margine = 10,.width = 200, .child = TextUI::Create({.text = U"閉じる",.color = Palette::White}) });

				dialog->close();

				manager.addChild
				({
					SimpleDialog::Create
					({
						.erasable = false,
						.child = SweetsPanel::Create
						({
							.child=Column::Create
							({
								.children
								{
									TextUI::Create({.text = U"お助けモードとは",.fontSize = 40,.color = Palette::White}),
									TextUI::Create({.text = U"失敗するとそのステージのHPが増加する機能です。",.fontSize = 20,.color = Palette::White}),
									closeButton
								}
							})
						}),
						.updateFunc=[=](SimpleDialog* dialog)mutable
						{
							if(closeButton->clicked()||KeyEnter.down())
							{
								dialog->close();
								manager.addChild(SettingWindow(upInputGroup, downInputGroup, leftInputGroup, rightInputGroup, onClose, gameData, manager, selectIndex));
							}
						}
					})
				});
			}




			if(hpModeButton->clicked() || (selectIndex == 3 && KeyEnter.down()))
			{
				dialog->close();

				auto yesButton = ChocolateButton::Create({ .color = Palette::Darkred, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = U"OFFにする",.color = Palette::White})});
				auto noButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = U"キャンセル",.color = Palette::White})});
				noButton->selected = true;

				manager.addChild
				({
					SimpleDialog::Create
					({
						.erasable = false,
						.child = SweetsPanel::Create
						({
							.child = Column::Create
							({
								.margine = 20,
								.children
								{
									TextUI::Create({.text = U"お助けモードをOFFにしますか？",.fontSize = 40,.color = Palette::White}),
									TextUI::Create({.text = U"いままでに増加したHPも5HPにリセットされます。\n※お助けモード：失敗するとそのステージのHPが増加する機能です。",.fontSize = 20,.color = Palette::White}),
									Row::Create
									({
										.children
										{
											yesButton,
											noButton
										}
									}),
								}
							}),
						}),
						.updateFunc = [=](SimpleDialog* dialog)mutable
						{
							if(leftInputGroup.down())
							{
								if (noButton->selected)
								{
									yesButton->selected = true;
									noButton->selected = false;
									AudioAsset{ U"カーソル移動" }.playOneShot();
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}

							if (rightInputGroup.down())
							{
								if (yesButton->selected)
								{
									yesButton->selected = false;
									noButton->selected = true;
									AudioAsset{ U"カーソル移動" }.playOneShot();
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}

							if((yesButton->selected && KeyEnter.down())|| yesButton->clicked())
							{
								gameData.setIncreaseHPMode(false);
								AudioAsset{ U"決定ボタン" }.playOneShot();
							}

							if ((noButton->selected && KeyEnter.down()) || noButton->clicked())
							{
								AudioAsset{ U"キャンセル" }.playOneShot();

								dialog->close();
								manager.addChild(SettingWindow(upInputGroup,downInputGroup,leftInputGroup,rightInputGroup,onClose,gameData,manager, selectIndex));
							}

						}
					})
				});

			}

			if (closeButton->clicked() || (selectIndex == 4 && KeyEnter.down())||KeyQ.down())
			{
				AudioAsset{ U"キャンセル" }.playOneShot();

				dialog->close();
				if (onClose)
				{
					onClose();
				}
			}
		}
	});
}
