#pragma once

class BeltConveyorButton
{
public:

	Vec2 pos;
	int32 num;
	double blockSize;

	bool firstClearMode = false;
	double starScale = 0.0;

	bool updateFirstClearMode()
	{
		if (firstClearMode)
		{
			starScale = Min(1.0, starScale + Scene::DeltaTime()*2);

			if (1.0 <= starScale)
			{
				AudioAsset{ U"キラキラ" }.playOneShot();
				firstClearMode = false;
			}
		}

		return firstClearMode;
	}

	bool update()
	{
		return RectF{ pos,num * blockSize,blockSize }.leftClicked();
	}

	Vec2 getStarPos()const
	{
		RectF rect{ pos,num * blockSize,blockSize };
		return { rect.x + blockSize * 0.5,rect.center().y };
	}

	void draw(const Texture& texture, const Font& font, StringView text, const ColorF& color, bool star, bool selected)const
	{
		RectF rect{ pos,num * blockSize,blockSize };
		const bool mouseOver = rect.mouseOver();
		for (int32 i = 0; i < num; ++i)
		{
			beltConveyorLeft(texture, pos + Vec2{ i * blockSize,0 }, blockSize, mouseOver || selected);
		}

		rect.drawFrame(selected ? 8 : 2, color);

		

		if (firstClearMode)
		{
			Shape2D::Star(blockSize * 0.3, getStarPos()).drawFrame(3, Palette::White);
			Shape2D::Star(blockSize * 0.3 * MyEase(starScale), getStarPos()).draw(Palette::Yellow);
		}
		else if (star)
		{
			Shape2D::Star(blockSize * 0.3, getStarPos()).draw(Palette::Yellow);
		}
		else
		{
			Shape2D::Star(blockSize * 0.3, getStarPos()).drawFrame(3, Palette::White);
		}

		font(text).drawAt(30,RectF{ pos + Vec2{blockSize * 0.8,0},(num - 1) * blockSize,blockSize }.center());
	}

	static void beltConveyorLeft(const Texture& texture, const Vec2& pos, double blockSize, bool move)
	{
		const double size = texture.size().x;
		double d = 1;
		if (move)
		{
			d = 1 - Periodic::Sawtooth0_1(2s);
		}
		texture(size - size * d, 0, size * d, size).resized(blockSize * d, blockSize).draw(pos);
		texture(0, 0, size - size * d, size).resized(blockSize * (1 - d), blockSize).draw((pos + Vec2{ d * blockSize,0 }));
	}

	static double MyEase(double t)
	{
		constexpr double a = 0.6;
		constexpr double max = 1.5;

		if (t < a)
		{
			return t / a * max;
		}
		else
		{
			return 1.0 - (t - a) / (1 - a) * (max - 1);
		}
	}

};
