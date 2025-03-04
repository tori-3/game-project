#include"SettingWindow.h"
#include"SweetsPanel.hpp"

std::shared_ptr<UIElement> SettingWindow(const InputGroup& upInputGroup, const InputGroup& downInputGroup, const InputGroup& leftInputGroup, const InputGroup& rightInputGroup, const std::function<void()>& onClose)
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
				TextUI::Create({.text=U"効果音 音量",.color=Palette::White}),
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
				TextUI::Create({.text=U"BGM 音量",.color = Palette::White}),
				BGMVolumeSlider,
				BGMVolumeIcon
			}

		})
	});

	String text = Window::GetState().fullscreen ? U"ウィンドウ" : U"全画面";
	auto windowModeButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 200, .child = TextUI::Create({.text = text,.color=Palette::White}) });

	static int32 selectIndex = 0;
	static LongPressInput upInput;
	static LongPressInput downInput;

	selectIndex = 0;
	upInput = LongPressInput{ upInputGroup };
	downInput = LongPressInput{ downInputGroup };

	return SimpleDialog::Create
	({
		.child = SweetsPanel::Create
		({
			.child = Column::Create
			({
				.margine = 20,
				.children
				{
					TextUI::Create({.text=U"設定",.fontSize = 40,.color=Palette::White}),
					effectVolumeSliderPanel,
					BGMVolumeSliderPanel,
					windowModeButton,
					closeButton
				}
			}),
		}),
		.updateFunc = [=](SimpleDialog* dialog)
		{
			GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
			GlobalAudio::BusSetVolume(BGMMixBus, BGMVolumeSlider->value);
			effectVolumeIcon->volume = effectVolumeSlider->value;
			BGMVolumeIcon->volume = BGMVolumeSlider->value;

			if (upInput.down())
			{
				selectIndex = Max(selectIndex - 1, 0);
			}

			if (downInput.down())
			{
				selectIndex = Min(selectIndex + 1, 3);
			}

			effectVolumeSliderPanel->color = selectIndex == 0 ? ColorF{ Palette::Skyblue } : AlphaF(0);
			BGMVolumeSliderPanel->color = selectIndex == 1 ? ColorF{ Palette::Skyblue } : AlphaF(0);
			windowModeButton->selected = selectIndex == 2;
			closeButton->selected = selectIndex == 3;

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
				const bool isFullscreen = Window::GetState().fullscreen;
				windowModeButton->setChild(TextUI::Create({.text = isFullscreen ? U"全画面":U"ウィンドウ",.color=Palette::White  }));
				Window::SetFullscreen(not isFullscreen);
			}

			if (closeButton->clicked() || (selectIndex == 3 && KeyEnter.down())||KeyQ.down())
			{
				dialog->close();
				if (onClose)
				{
					onClose();
				}
			}
		}
	});
}
