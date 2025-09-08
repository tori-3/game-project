#include "GalleryScene.h"

GalleryScene::GalleryScene(const InitData& init) :
	IScene{ init }
{

}

constexpr double textureWidth = 250;
constexpr double margineX = 180;
constexpr double margineY = 300;

void GalleryScene::update()
{
	manager.update();

	if (0 == manager.getChildren().size())
	{
		if (getData().minigameUpKey.down())
		{
			if (0 < selectPoint.y)
			{
				--selectPoint.y;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (getData().minigameDownKey.down())
		{
			if (selectPoint.y < textureGrid.height() - 1)
			{
				++selectPoint.y;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (leftInput.down())
		{
			if (0 < selectPoint.x)
			{
				--selectPoint.x;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (rightInput.down())
		{
			if (selectPoint.x < textureGrid.width() - 1)
			{
				++selectPoint.x;
				AudioAsset{ U"カーソル移動" }.playOneShot();
			}
			else
			{
				AudioAsset{ U"ビープ音" }.playOneShot();
			}
		}

		if (getData().menuBackKey.down()|| backButton.leftClicked())
		{
			changeScene(U"TitleScene");
		}

		bool clicked = false;

		if (KeyEnter.down())
		{
			clicked = true;
		}

		for (int32 y = 0; y < textureGrid.height(); ++y)
		{
			for (int32 x = 0; x < textureGrid.width(); ++x)
			{
				Vec2 center{ margineX + x * (Scene::Width() - margineX * 2) / (textureGrid.width() - 1), margineY + y * (Scene::Height() - margineY * 2) / (textureGrid.height() - 1) };
				const RectF rect = textureGrid[y][x].resized(textureWidth).regionAt(center);

				if (rect.leftClicked())
				{
					clicked = true;
					selectPoint = { x,y };
				}
			}
		}

		if (clicked)
		{
			AudioAsset{ U"決定ボタン" }.playOneShot();

			if (selectPoint == Point{ 3,1 })
			{
				changeScene(U"EndRollScene");
			}
			else
			{
				auto closeButton = ChocolateButton::Create({ .color = Palette::Hotpink, .padding = 10,.margine = 10,.width = 150, .child = TextUI::Create({.text = U"\U000F05AD閉じる",.color = Palette::White}) });
				closeButton->selected = true;

				manager.addChild
				({
					SimpleDialog::Create
					({
						.erasable = false,
						.child = SweetsPanel::Create
						({
							.margine = 15,
							.child = Column::Create
							({
								.children
								{
									TextureUI::Create
									({
										.texture = textureGrid[selectPoint],
										.flex = 1
									}),
									Row::Create
									({
										.children
										{
											TextUI::Create
											({
												.text = textCsv[selectPoint.x + selectPoint.y * textureGrid.width()][0],
												.fontSize = 20,
												.color = Palette::White,
												.margine = 10,
												.height = 140,
												.flex=1,
											}),
											closeButton
										}
									})
								}
							}),
						}),
						.updateFunc = [=](SimpleDialog* dialog)
						{
							if (getData().menuBackKey.down() || closeButton->pressed() || getData().menuDecisionKey.down())
							{
								dialog->close();
								AudioAsset{ U"キャンセル" }.playOneShot();
							}
						}
					})
				});
			}
		}
	}
}

void GalleryScene::draw() const
{
	TextureAsset{ U"BackGroundTexture/宇宙背景.png" }.draw();

	for(int32 y=0;y<textureGrid.height();++y)
	{
		for (int32 x = 0; x < textureGrid.width(); ++x)
		{
			Vec2 center{ margineX + x * (Scene::Width() - margineX * 2) / (textureGrid.width() - 1), margineY + y * (Scene::Height() - margineY * 2) / (textureGrid.height() - 1) };
			const RectF rect = textureGrid[y][x].resized(textureWidth).regionAt(center);

			rect.drawShadow(Vec2{ 0, 0 }, 20, 10 + (Point{ x,y } == selectPoint) * 5,Point{x,y}==selectPoint?Palette::Yellow:Palette::White);
			textureGrid[y][x].resized(textureWidth).drawAt(center);

			if (x == 3 && y == 1)
			{
				rect.draw(ColorF{ 0,0.5 });
				Triangle{ center, 50 }.rotated(90_deg).draw(Palette::White);
				FontAsset{ U"NormalFont" }(U"End roll").drawAt(rect.center()+Vec2{0,40});
			}
		}
	}

	homeIcon.drawAt(backButton.center, backButton.mouseOver() ? Palette::Gray : Palette::White);
	FontAsset{ U"NormalFont" }(U"[Q]タイトルに戻る").draw(Arg::leftCenter = backButton.center + Vec2{ 30,0 });



	manager.draw();
}
