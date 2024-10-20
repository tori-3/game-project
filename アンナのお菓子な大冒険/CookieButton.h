#pragma once

class CookieButton {
public:
	RectF rect;
	String text;
	bool active;

	double circleR;

	static constexpr ColorF cookieColor = Color{ 255,180,100 };

	CookieButton(RectF rect, String text, bool active = true, double circleR = 10) :rect{ rect }, text{ text }, active{ active }, circleR{ circleR }
	{
		//フォントを登録
		FontAsset::Register(U"ButtonFont", FontMethod::MSDF, 50, Typeface::Bold);




	}

	bool update();

	void draw()const;
};
