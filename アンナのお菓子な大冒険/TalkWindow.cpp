#include"TalkWindow.h"

void TalkWindow::update()
{
	const bool backFlg = KeyA.down() || KeyLeft.down();
	const bool nextFlg = KeyD.down() || KeyRight.down() || KeyEnter.down();

	//確認
	if (not isContinue())return;

	m_accumlater += Scene::DeltaTime() / 0.05;

	const size_t length = static_cast<size_t>(m_accumlater);
	const String& text = talks[index].text;

	m_lastLength = length;

	if (nextFlg)
	{
		if (text.size() < length)
		{
			if (isContinue())
			{
				next();
				m_lastLength = 0;
			}
		}
		else
		{
			m_accumlater = (double)text.size();
		}
	}
	else if (backFlg)
	{
		back();
	}
}

void TalkWindow::draw(const RectF& rect, const SizeF& nameSize)const
{
	if (not isContinue())return;

	const TalkInfo& talk = talks[index];

	constexpr double thickness = 5;

	ColorF frameColor = Palette::White;

	if (talk.name == U"アンナ")
	{
		frameColor = Palette::Pink;
	}
	else if (talk.name == U"妖精")
	{
		frameColor = Palette::Greenyellow;
	}

	const RoundRect window{ rect,20 };
	const RoundRect nameWindow{ rect.pos - Vec2::Down(nameSize.y - thickness),nameSize ,20 };
	window.draw({ Palette::Black, 0.8 }).drawFrame(thickness, 0, frameColor);
	nameWindow.draw({ Palette::Black ,0.8 }).drawFrame(thickness, 0, frameColor);

	//確認


	const size_t length = static_cast<size_t>(m_accumlater);
	FontAsset(U"WindowFont")(talk.text.substr(0, length)).draw(window.rect.stretched(-20));
	FontAsset(U"WindowFont")(talk.name).drawAt(nameWindow.rect.center());

	if (talk.text.size() < length)
	{
		Triangle{ window.rect.br().movedBy(-40, -40), 20, 180_deg }.draw(ColorF{ 1, Periodic::Sine0_1(2.0s) });
	}

	FontAsset(U"WindowFont")(U"←A 戻る　　次へ D→").drawBase(20, window.rect.bl() + Vec2{ 20,-20 });
}

