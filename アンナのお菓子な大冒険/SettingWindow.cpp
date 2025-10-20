#include"Common.h"
#include"SweetsPanel.hpp"
#include"SettingWindow.h"
#include"SoundIcon.hpp"
#include"ControllerInput.h"
#include"KeyInfo.h"
#include"ControllerManager.h"

class KeyConfigInfo
{
public:

	static constexpr size_t maxButton = 3;

	static constexpr ColorF normalTextColor = Palette::White;
	static constexpr ColorF unconfiguredTextColor = Palette::Lightgray;

	String text;
	Array<Optional<Input>> inputs;
	Array<std::shared_ptr<ChocolateButton>>buttons;

	std::shared_ptr<TextUI>textUI = TextUI::Create({ .text = text,.color = Palette::White, });

	KeyConfigInfo(StringView text, const InputGroup& inputGroup)
		:text{ text }, inputs( maxButton, none )
	{
		for (size_t i = 0; i < inputGroup.inputs().size(); ++i)
		{
			inputs[i] = inputGroup.inputs()[i];
		}

		for (size_t i = 0; i < maxButton; ++i)
		{
			buttons << ChocolateButton::Create
			({
				.color=Palette::Chocolate,
				.margine = {5,8},
				.width = 150,
				.relative = Relative::Center(),
				.child
				{
					TextUI::Create({.text = inputs[i] ? (GetKeyName(inputs[i].value())) : U"未設定",.fontSize = 23,.color = inputs[i] ? normalTextColor : unconfiguredTextColor,.height=35})
				}
			});
		}
	}

	InputGroup getInputGroup()const
	{
		InputGroup group;

		for (auto& input : inputs)
		{
			if (input)
			{
				group = group | input.value();
			}
		}
		return group;
	}

	bool isInputting()const
	{
		return inputIndex.has_value();
	}

	bool update(bool inputDown,bool inputPressed,bool otherInputting)
	{
		if(inputIndex)
		{
			const Array<Input> keys = Keyboard::GetAllInputs();

			Optional<Input>newKey = GetDownXInput();

			for(const Input& key:keys)
			{
				if(key.down())
				{
					newKey = key;
					break;
				}
			}

			if(newKey)
			{
				inputs[*inputIndex] = newKey.value();
				buttons[*inputIndex]->color = Palette::Chocolate;
				buttons[*inputIndex]->setChild(TextUI::Create({ .text = GetKeyName(newKey.value()),.fontSize = 23,.color = normalTextColor,.height = 35 }));
				inputIndex = none;
			}

			if(inputPressed || MouseL.pressed())
			{
				inputTimer += Scene::DeltaTime();
			}
			else
			{
				inputTimer = 0;
			}

			if(2<inputTimer)
			{
				AudioAsset{ U"キャンセル" }.playOneShot();
				inputs[*inputIndex] = none;
				inputIndex = none;
				buttons[*inputIndex]->setChild(TextUI::Create({ .text = U"未設定",.fontSize = 23,.color = unconfiguredTextColor,.height = 35 }));
				buttons[*inputIndex]->color = Palette::Chocolate;
			}

		}
		else
		{
			inputTimer = 0;
			for(size_t i=0;i<buttons.size();++i)
			{
				if (buttons[i]->clicked() || (buttons[i]->selected && inputDown))
				{
					if(not otherInputting)
					{
						AudioAsset{ U"決定ボタン" }.playOneShot();

						buttons[i]->color = Palette::Orange;
						buttons[i]->setChild(TextUI::Create({ .text = U"キーを入力",.fontSize = 23,.color = normalTextColor,.height = 35 }));

						inputIndex = i;
					}
					break;
				}
			}
		}

		if(isEmpty())
		{
			textUI->color = Palette::Red;
		}
		else
		{
			textUI->color = Palette::White;
		}


		return inputIndex.has_value();
	}

	bool isEmpty()const
	{
		return getInputGroup().inputs().empty();

	}

	std::shared_ptr<UIElement>getUI()const
	{
		auto row = Row::Create
		({
			.mainAxis=MainAxis::end,
			//.width=1000,
			.children
			{
				textUI
			}
		});

		bool firstFlg = true;

		for(auto& button: buttons)
		{
			if (firstFlg)
			{
				firstFlg = false;
			}
			else
			{
				row->addChild(TextUI::Create({ .text = U"or",.fontSize = 18,.color = normalTextColor }));
			}
			row->addChild(button);
		}
		
		return row;
	}

	void setIndex(const Optional<size_t>& index)
	{
		if(not index)
		{
			if(selectedIndex)
			{
				buttons[selectedIndex.value()]->selected = false;
				selectedIndex = none;
			}
			return;
		}

		if(selectedIndex)
		{
			buttons[selectedIndex.value()]->selected = false;
		}
		selectedIndex = index;

		buttons[index.value()]->selected = true;
	}

private:

	Optional<size_t>selectedIndex;

	Optional<size_t>inputIndex;

	double inputTimer = 0;
};

std::shared_ptr<UIElement> SettingWindow(const InputGroup& upInputGroup, const InputGroup& downInputGroup, const InputGroup& leftInputGroup, const InputGroup& rightInputGroup, const std::function<void()>& onClose,GameData& gameData,UIManager& manager,size_t index)
{
	auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding =20,.margine = {10,0,0,0},.width = 200, .child = TextUI::Create({.text = U"\U000F05AD閉じる",.color = Palette::White})});
	auto effectVolumeSlider = SimpleSlider::Create({ .value = GlobalAudio::BusGetVolume(EffectMixBus) });
	auto BGMVolumeSlider = SimpleSlider::Create({ .value = GlobalAudio::BusGetVolume(BGMMixBus) });
	auto effectVolumeIcon = SoundIcon::Create({ .volume = effectVolumeSlider->value });
	auto BGMVolumeIcon = SoundIcon::Create({ .volume = BGMVolumeSlider->value });

	auto keyConfigButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 200, .child = TextUI::Create({.text = U"\U000F030C キー設定",.color = Palette::White}) });


	auto vibrationSlider = SimpleSlider::Create({ .value = ControllerManager::get().vibration });


	auto effectVolumeSliderPanel = RectPanel::Create
	({
		.color=Palette::Skyblue,
		.margine = {10,0,0,0},
		.child = Row::Create
		({
			.margine = {0,10},
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
		.margine = 0,
		.child = Row::Create
		({
			.margine = {0,10},
			.children
			{
				TextUI::Create({.text=U"BGM 音量",.color = Palette::White,.width = 150}),
				BGMVolumeSlider,
				BGMVolumeIcon
			}
		})
	});


	auto vibrationSliderPanel = RectPanel::Create
	({
		.color = AlphaF(0),
		.margine = 0,
		.child = Row::Create
		({
			.margine = {0,10},
			.children
			{
				TextUI::Create({.text = U"振動 \U000F02B4",.color = Palette::White,.width = 150}),
				vibrationSlider,
				RectUI::Create({.size{50,50},.color = AlphaF(0)})
			}
		})
	});


	String text = Window::GetState().fullscreen ? U"ウィンドウにする" : U"全画面にする";
	auto windowModeButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = text,.color=Palette::White}) });


	String text2 = gameData.getIncreaseHPMode() ? U"❤HP増加をOFFにする" : U"❤HP増加をONにする";
	auto hpModeText = TextUI::Create({ .text = text2,.color = Palette::White });
	auto hpModeButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 340, .child = hpModeText });

	//auto infoButton = ChocolateButton::Create({ .color = Palette::White, .padding = -20,.width=50,.height=50, .child = TextUI::Create({.text = U"i",.color = Palette::Gray}) });


	static int32 selectIndex = 0;
	static LongPressInput upInput;
	static LongPressInput downInput;
	static LongPressInput leftInput;
	static LongPressInput rightInput;

	selectIndex = index;
	//upInput = LongPressInput{ upInputGroup };
	//downInput = LongPressInput{ downInputGroup };

	upInput = LongPressInput{ gameData.minigameUpKey,ControllerManager::Direction::Up };
	downInput = LongPressInput{ gameData.minigameDownKey,ControllerManager::Direction::Down };
	leftInput = LongPressInput{ gameData.minigameLeftKey,ControllerManager::Direction::Left };
	rightInput = LongPressInput{ gameData.minigameRightKey,ControllerManager::Direction::Right };

	effectVolumeSliderPanel->color = selectIndex == 0 ? ColorF{ Palette::Skyblue } : AlphaF(0);
	BGMVolumeSliderPanel->color = selectIndex == 1 ? ColorF{ Palette::Skyblue } : AlphaF(0);
	vibrationSliderPanel->color = selectIndex == 2 ? ColorF{ Palette::Skyblue } : AlphaF(0);
	windowModeButton->selected = selectIndex == 3;
	hpModeButton->selected = selectIndex == 4;
	keyConfigButton->selected = selectIndex == 5;
	closeButton->selected = selectIndex == 6;

	return SimpleDialog::Create
	({
		.erasable = false,
		.child = SweetsPanel::Create
		({
			.child = Column::Create
			({
				.margine = {10,20},
				
				.children
				{
					TextUI::Create({.text = U"\U000F0493 設定",.fontSize = 40,.color = Palette::White}),
					effectVolumeSliderPanel,
					BGMVolumeSliderPanel,
					vibrationSliderPanel,
					windowModeButton,
					hpModeButton,
					//Row::Create
					//({
					//	.children
					//	{
					//		RectUI::Create({.size{50,50},.color=Alpha(0)}),
					//		hpModeButton,
					//		infoButton
					//	}
					//}),
					keyConfigButton,
					closeButton
				}
			}),
		}),
		.updateFunc = [=,&gameData](SimpleDialog* dialog)mutable
		{
			GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
			GlobalAudio::BusSetVolume(BGMMixBus, BGMVolumeSlider->value);
			ControllerManager::get().vibration = vibrationSlider->value;
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
				if (selectIndex<6)
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
			vibrationSliderPanel->color = selectIndex == 2 ? ColorF{ Palette::Skyblue } : AlphaF(0);
			windowModeButton->selected = selectIndex == 3;
			hpModeButton->selected = selectIndex == 4;
			keyConfigButton->selected = selectIndex == 5;
			closeButton->selected = selectIndex == 6;

			if (selectIndex == 0)
			{
				effectVolumeSlider->value = Clamp(effectVolumeSlider->value + ((gameData.minigameRightKey.pressed() || ControllerManager::RightPressed()) - (gameData.minigameLeftKey.pressed() || ControllerManager::LeftPressed())) * Scene::DeltaTime(), 0.0, 1.0);

				if (gameData.minigameLeftKey.up()|| gameData.minigameRightKey.up()|| ControllerManager::LeftUp()|| ControllerManager::RightUp())
				{
					GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
					AudioAsset{ U"パンチヒット" }.playOneShot();
				}
			}
			else if (selectIndex == 1)
			{
				BGMVolumeSlider->value = Clamp(BGMVolumeSlider->value + ((gameData.minigameRightKey.pressed() || ControllerManager::RightPressed()) - (gameData.minigameLeftKey.pressed() || ControllerManager::LeftPressed())) * Scene::DeltaTime(), 0.0, 1.0);
			}
			else if (selectIndex == 2)
			{
				vibrationSlider->value = Clamp(vibrationSlider->value + ((gameData.minigameRightKey.pressed() || ControllerManager::RightPressed()) - (gameData.minigameLeftKey.pressed() || ControllerManager::LeftPressed())) * Scene::DeltaTime(), 0.0, 1.0);

				if (gameData.minigameLeftKey.up() || gameData.minigameRightKey.up() || ControllerManager::LeftUp() || ControllerManager::RightUp())
				{
					ControllerManager::get().vibration = vibrationSlider->value;
					ControllerManager::get().setVibration(0.3);
				}
			}

			if (effectVolumeSlider->sliderReleased())
			{
				GlobalAudio::BusSetVolume(EffectMixBus, effectVolumeSlider->value);
				AudioAsset{ U"パンチヒット" }.playOneShot();
			}

			if(vibrationSlider->sliderReleased())
			{
				ControllerManager::get().vibration = vibrationSlider->value;
				ControllerManager::get().setVibration(0.3);
			}


			if (windowModeButton->clicked() || (selectIndex == 3 && gameData.menuDecisionKey.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();

				const bool isFullscreen = Window::GetState().fullscreen;
				windowModeButton->setChild(TextUI::Create({.text = isFullscreen ? U"全画面にする":U"ウィンドウにする",.color=Palette::White  }));
				Window::SetFullscreen(not isFullscreen);

				gameData.save();
			}

			if(hpModeButton->clicked() || (selectIndex == 4 && gameData.menuDecisionKey.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();

				if (gameData.getIncreaseHPMode())
				{
					dialog->close();

					auto yesButton = ChocolateButton::Create({ .color = Palette::Darkred, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = U"OFFにする",.color = Palette::White}) });
					auto noButton = ChocolateButton::Create({ .color = Palette::Chocolate, .padding = 20,.margine = 10,.width = 240, .child = TextUI::Create({.text = U"キャンセル",.color = Palette::White}) });
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
										TextUI::Create({.text = U"HP増加をOFFにしますか？",.fontSize = 40,.color = Palette::White}),
										TextUI::Create({.text = U"いままでに増加したHPも5HPにリセットされます。\n※HP増加：失敗するとそのステージのHPが2つ増加する機能です。",.fontSize = 20,.color = Palette::White}),
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
							.updateFunc = [=,&gameData](SimpleDialog* dialog)mutable
							{
								if (gameData.minigameLeftKey.down() || ControllerManager::LeftDown())
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

								if (gameData.minigameRightKey.down() || ControllerManager::RightDown())
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

								if ((yesButton->selected && gameData.menuDecisionKey.down()) || yesButton->clicked())
								{
									gameData.setIncreaseHPMode(false);
									gameData.save();

									AudioAsset{ U"決定ボタン" }.playOneShot();

									dialog->close();
									manager.addChild(SettingWindow(upInputGroup, downInputGroup, leftInputGroup, rightInputGroup, onClose, gameData, manager, selectIndex));
								}

								if ((noButton->selected && gameData.menuDecisionKey.down()) || noButton->clicked()||gameData.menuBackKey.down())
								{
									AudioAsset{ U"キャンセル" }.playOneShot();

									dialog->close();
									manager.addChild(SettingWindow(upInputGroup,downInputGroup,leftInputGroup,rightInputGroup,onClose,gameData,manager, selectIndex));
								}

							}
						})
					});
				}
				else
				{
					gameData.setIncreaseHPMode(true);
					gameData.save();

					hpModeText->setText(U"❤HP増加をOFFにする");
				}
			}

			if(keyConfigButton->clicked()|| (selectIndex == 5 && gameData.menuDecisionKey.down()))
			{
				AudioAsset{ U"決定ボタン" }.playOneShot();

				dialog->close();

				auto col = Column::Create({});

				std::shared_ptr<Array<KeyConfigInfo>>table
				{
					new Array<KeyConfigInfo>
					{
						KeyConfigInfo{U"ジャンプ",gameData.jumpKey},
						KeyConfigInfo{U"攻撃",gameData.attackKey},
						//KeyConfigInfo{U"左移動",gameData.leftKey},
						//KeyConfigInfo{U"右移動",gameData.rightKey},
						KeyConfigInfo{U"しゃがむ(↓)",gameData.downKey},
						KeyConfigInfo{U"ポーズ",gameData.pauseKey},
						KeyConfigInfo{U"上移動↑",gameData.minigameUpKey},
						KeyConfigInfo{U"下移動↓",gameData.minigameDownKey},
						KeyConfigInfo{U"左移動←",gameData.minigameLeftKey},
						KeyConfigInfo{U"右移動→",gameData.minigameRightKey},
						KeyConfigInfo{U"メニュー：決定",gameData.menuDecisionKey},
						KeyConfigInfo{U"メニュー：戻る",gameData.menuBackKey},
						KeyConfigInfo{U"スクリーンショット",gameData.screenshotKey},
					}
				};

				for(auto& keyConfig:*table.get())
				{
					col->addChild(keyConfig.getUI());
				}

				auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 10,.margine = 0,.width = 200, .child = TextUI::Create({.text = U"\U000F05AD閉じる",.color = Palette::White}) });

				(*table.get())[0].setIndex(0);

				static Point pos{ 0,0 };
				upInput = LongPressInput{ gameData.minigameUpKey,ControllerManager::Direction::Up };
				downInput = LongPressInput{ gameData.minigameDownKey,ControllerManager::Direction::Down };
				leftInput = LongPressInput{ gameData.minigameLeftKey,ControllerManager::Direction::Left };
				rightInput = LongPressInput{ gameData.minigameRightKey,ControllerManager::Direction::Right };
				pos = { 0,0 };

				auto initializeButon = ChocolateButton::Create({ .color = Palette::Darkred, .padding = 10,.margine = 0,.width = 200, .child = TextUI::Create({.text = U"初期化",.color = Palette::White}) });


				manager.addChild
				({
					SimpleDialog::Create
					({
						.erasable = false,
						.child = SweetsPanel::Create
						({
							.margine=5,
							.child = Column::Create
							({
								.children
								{
									TextUI::Create({.text = U"\U000F030C キー設定",.fontSize = 40,.color = Palette::White}),
									col,
									Row::Create
									({
										.mainAxis = MainAxis::spaceEvenly,
										.margine = 10,
										.children
										{
											//TextUI::Create({.text = U"　　　　　　",.fontSize = 30,.color = Palette::White}),
											initializeButon,
											closeButton,
											TextUI::Create({.text = U"長押しで削除",.fontSize = 30,.color = Palette::White}),
										}
									}),
								TextUI::Create({.text = U"※上移動↑でもジャンプができます  ※左スティックでも移動ができます",.fontSize = 20,.color = ColorF{0.8}})
								}
							})
						}),
						.updateFunc = [=,&gameData](SimpleDialog* dialog)mutable
						{
							bool isInputting = false;

							for (auto& keyConfig : *table.get())
							{
								if (keyConfig.isInputting())
								{
									isInputting = true;
								}
							}

							for (auto& keyConfig : *table.get())
							{
								if (keyConfig.update(gameData.menuDecisionKey.down(), gameData.menuDecisionKey.pressed(), isInputting))
								{
									isInputting = true;
								}
							}
							if (isInputting)
							{
								return;
							}

							if (leftInput.down())
							{
								if (pos.y == table->size())
								{
									if(closeButton->selected)
									{
										closeButton->selected = false;
										initializeButon->selected = true;
										AudioAsset{ U"カーソル移動" }.playOneShot();
									}
									else
									{
										AudioAsset{ U"ビープ音" }.playOneShot();
									}
								}
								else if (0 < pos.x)
								{
									AudioAsset{ U"カーソル移動" }.playOneShot();
									(*table)[pos.y].setIndex(--pos.x);
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}

							if (rightInput.down())
							{
								if (pos.y == table->size())
								{
									if (initializeButon->selected)
									{
										closeButton->selected = true;
										initializeButon->selected = false;
										AudioAsset{ U"カーソル移動" }.playOneShot();
									}
									else
									{
										AudioAsset{ U"ビープ音" }.playOneShot();
									}
								}
								else if (pos.x < KeyConfigInfo::maxButton - 1)
								{
									AudioAsset{ U"カーソル移動" }.playOneShot();
									(*table)[pos.y].setIndex(++pos.x);
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}

							if (upInput.down())
							{
								if (0 < pos.y)
								{
									AudioAsset{ U"カーソル移動" }.playOneShot();

									if (pos.y != table->size())
									{
										(*table)[pos.y].setIndex(none);
									}
									else
									{
										initializeButon -> selected = false;
										closeButton->selected = false;
									}

									(*table)[--pos.y].setIndex(pos.x);
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}
						
							if (downInput.down())
							{
								if (pos.y < table->size())
								{
									AudioAsset{ U"カーソル移動" }.playOneShot();
									(*table)[pos.y].setIndex(none);

									if (pos.y == table->size()-1)
									{
										++pos.y;
										closeButton->selected = true;
									}
									else
									{
										(*table)[++pos.y].setIndex(pos.x);
									}
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}

							bool empty = false;
							for (auto& key : (*table))
							{
								if(key.isEmpty())
								{
									empty = true;
									break;
								}
							}

							closeButton->clickable = not empty;

							if (closeButton->clicked() || (closeButton->selected && gameData.menuDecisionKey.down()) || gameData.menuBackKey.down())
							{
								if (not empty)
								{
									AudioAsset{ U"キャンセル" }.playOneShot();

									gameData.jumpKey = (*table)[0].getInputGroup();
									gameData.attackKey = (*table)[1].getInputGroup();
									gameData.downKey = (*table)[2].getInputGroup();
									gameData.pauseKey = (*table)[3].getInputGroup();
									gameData.minigameUpKey = (*table)[4].getInputGroup();
									gameData.minigameDownKey = (*table)[5].getInputGroup();
									gameData.minigameLeftKey = (*table)[6].getInputGroup();
									gameData.minigameRightKey = (*table)[7].getInputGroup();
									gameData.menuDecisionKey = (*table)[8].getInputGroup();
									gameData.menuBackKey = (*table)[9].getInputGroup();
									gameData.screenshotKey = (*table)[10].getInputGroup();

									gameData.save();

									dialog->close();
									manager.addChild(SettingWindow(upInputGroup, downInputGroup, leftInputGroup, rightInputGroup, onClose, gameData, manager, selectIndex));
								}
								else
								{
									AudioAsset{ U"ビープ音" }.playOneShot();
								}
							}

							if (initializeButon->clicked() || (initializeButon->selected && gameData.menuDecisionKey.down()))
							{
								AudioAsset{ U"決定ボタン" }.playOneShot();
								gameData.initKeyConfig();

								col->setChildren({});

								table = std::shared_ptr<Array<KeyConfigInfo>>{
									new Array<KeyConfigInfo>
									{
										KeyConfigInfo{U"ジャンプ",gameData.jumpKey},
										KeyConfigInfo{U"攻撃",gameData.attackKey},
										KeyConfigInfo{U"しゃがむ(↓)",gameData.downKey},
										KeyConfigInfo{U"ポーズ",gameData.pauseKey},
										KeyConfigInfo{U"上移動↑",gameData.minigameUpKey},
										KeyConfigInfo{U"下移動↓",gameData.minigameDownKey},
										KeyConfigInfo{U"左移動←",gameData.minigameLeftKey},
										KeyConfigInfo{U"右移動→",gameData.minigameRightKey},
										KeyConfigInfo{U"メニュー：決定",gameData.menuDecisionKey},
										KeyConfigInfo{U"メニュー：戻る",gameData.menuBackKey},
										KeyConfigInfo{U"スクリーンショット",gameData.screenshotKey},
									}
								};

								for (auto& keyConfig : *table.get())
								{
									col->addChild(keyConfig.getUI());
								}

								if (pos.y != table->size())
								{
									(*table)[pos.y].setIndex(pos.x);
								}
							}
						}
					})
				});
			}
			
			if (closeButton->clicked() || (selectIndex == 6 && gameData.menuDecisionKey.down())|| gameData.menuBackKey.down())
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
