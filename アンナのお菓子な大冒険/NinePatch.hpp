#pragma once

struct NinePatchPosition
{
	NinePatchPosition(const Vec2& tl = { 0,0 }, const Vec2& br = { 1,1 })
		:tl(tl), br(br)
	{
		w1 = w2 = h1 = h2 = 1;
	}

	void setWidthRate(double w1, double w2, double w3)
	{
		auto total = w1 + w2 + w3;
		this->w1 = w1 / total;
		this->w2 = w2 / total;
	}

	void setHeightRate(double h1, double h2, double h3)
	{
		auto total = h1 + h2 + h3;
		this->h1 = h1 / total;
		this->h2 = h2 / total;
	}

	RectF tile(int pos) const
	{
		Vec2 diagonal = br - tl;

		RectF ret{ tl,diagonal.x * w1, diagonal.y * h1 };

		if (1 <= pos % 3)
		{
			ret.x += diagonal.x * w1;
			ret.w = diagonal.x * w2;
		}
		if (2 <= pos % 3)
		{
			ret.x += diagonal.x * w2;
			ret.w = diagonal.x * (1 - w1 - w2);
		}
		if (1 <= pos / 3)
		{
			ret.y += diagonal.y * h1;
			ret.h = diagonal.y * h2;
		}
		if (2 <= pos / 3)
		{
			ret.y += diagonal.y * h2;
			ret.h = diagonal.y * (1 - h1 - h2);
		}

		return ret;
	}

	void draw(const ColorF& color, Size textureSize, Vec2 pos = { 0,0 })const
	{
		for (int i = 0; i < 9; i++)
		{
			tile(i).moveBy(pos).scaledAt(pos, textureSize.x, textureSize.y).draw(color).drawFrame(1, Palette::Black);
		}
	}

	Vec2 tl;
	Vec2 br;
	double w1, w2;
	double h1, h2;
};

class MyNinePatch
{
public:
	TextureRegion tiles[9];

	Vec2 aspect{ 1,1 };

	double baseScale = 1.0;

	Size textureSize{ 0,0 };
	Vec2 pos;

	void setTexture(const Texture& texture, NinePatchPosition posInfo)
	{
		textureSize = texture.size();

		pos = posInfo.tl * textureSize;

		double w = (posInfo.br - posInfo.tl).x;
		double h = (posInfo.br - posInfo.tl).y;

		for (int i = 0; i < 9; i++)
		{
			tiles[i] = texture.uv(posInfo.tile(i));
		}
	}

	void setRect(const RectF& rect)
	{
		pos = rect.pos;
		aspect = rect.size / (SizeF{ textureSize }*baseScale);
	}

	void draw() const
	{
		double wRate = 3 * aspect.x - 2;
		double hRate = 3 * aspect.y - 2;

		double w1 = tiles[0].size.x * baseScale;
		double h1 = tiles[0].size.y * baseScale;
		double w2 = tiles[1].size.x * wRate * baseScale;
		double h2 = tiles[3].size.y * hRate * baseScale;

		for (int i = 0; i < 9; i++)
		{
			auto p = pos;

			if (1 <= i % 3)
			{
				p.x += w1;
			}
			if (2 <= i % 3)
			{
				p.x += w2;
			}
			if (1 <= i / 3)
			{
				p.y += h1;
			}
			if (2 <= i / 3)
			{
				p.y += h2;
			}

			// * o *
			// * * *
			// * o *
			if (i % 6 == 1)
			{
				tiles[i].scaled(baseScale).scaled(wRate, 1).draw(p);
			}
			// * * *
			// o * o
			// * * *
			else if (i % 2)
			{
				tiles[i].scaled(baseScale).scaled(1, hRate).draw(p);
			}
			// * * *
			// * o *
			// * * *
			else if (i == 4)
			{
				tiles[i].scaled(baseScale).scaled(wRate, hRate).draw(p);
			}
			// o * o
			// * * *
			// o * o
			else
			{
				tiles[i].scaled(baseScale).draw(p);
			}
		}
	}
};
