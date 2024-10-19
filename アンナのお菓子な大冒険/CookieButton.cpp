#include"CookieButton.h"

bool CookieButton::update() {

	//マウスの形を変える
	if (rect.mouseOver() && active)Cursor::RequestStyle(CursorStyle::Hand);

	if (rect.leftClicked() && active)
	{
		AudioAsset{ U"ビスケットクリック" }.playOneShot();
		return true;
	}
	else
	{
		return false;
	}
}

void CookieButton::draw()const {

	double scale = 1.0;

	if (active) {

		if (rect.mouseOver())
		{
			if (MouseL.pressed())
			{
				scale = 0.9;
			}
			else
			{
				scale = 1.1;
			}
		}
	}

	ColorF color = active ? cookieColor : cookieColor * 0.5;
	ColorF textColor = active ? ColorF{ 1.0 } : ColorF{ 0.5 };

	{
		Transformer2D target{ Mat3x2::Scale(scale,rect.center()) };


		for (int32 i = 0; i < rect.w / (circleR * 2); ++i)
		{
			Circle{ rect.leftX() + i * circleR * 2,rect.topY(),circleR }.draw(color);
			Circle{ rect.rightX() - i * circleR * 2,rect.bottomY(),circleR }.draw(color);
		}

		for (int32 i = 0; i < rect.h / (circleR * 2); ++i)
		{
			Circle{ rect.rightX(),rect.topY() + i * circleR * 2,circleR }.draw(color);
			Circle{ rect.leftX(), rect.bottomY() - i * circleR * 2,circleR }.draw(color);
		}

		rect.draw(color);

		for (auto& p : step(Size{ int32(rect.w / (circleR * 2)) - 1,int32(rect.h / (circleR * 2)) - 1 }))
		{
			Circle{ rect.pos + SizeF{circleR * 2,circleR * 2} + p * (circleR * 2) ,circleR / 2.0 }.draw(ColorF{ 0,0.3 });
		}

		//文字を描画
		FontAsset(U"ButtonFont")(text).drawAt(TextStyle::Outline(0.3, Palette::Black), rect.h * 0.7, rect.center(), textColor);
	}
}
