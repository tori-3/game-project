#pragma once

class BeltConveyorButton
{
public:

	Vec2 pos;
	int32 num;
	double blockSize;

	bool update()const
	{
		return RectF{ pos,num * blockSize,blockSize }.leftClicked();
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

		if (star)
		{
			Shape2D::Star(blockSize * 0.3, { rect.x + blockSize * 0.5,rect.center().y }).draw(Palette::Yellow);
		}
		else
		{
			Shape2D::Star(blockSize * 0.3, { rect.x + blockSize * 0.5,rect.center().y }).drawFrame(3, Palette::White);
		}

		font(text).drawAt(RectF{ pos + Vec2{blockSize * 0.8,0},(num - 1) * blockSize,blockSize }.center());
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

};

