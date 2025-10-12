#include"ChocolateButton.h"

void ChocolateButton::onUpdate()
{
	childUpdate();
}

void ChocolateButton::onDraw(const RectF& drawingArea)const
{
	if (selected)
	{
		//getRoundRect().drawFrame(0, 5, Palette::Skyblue);
		getRoundRect().drawFrame(0, 5, Palette::Lightcyan);
	}
	getRoundRect().draw(color + ColorF{ 0.1 }, color - ColorF{ 0.2 });
	childDraw(drawingArea);

	if (not clickable)
	{
		getRoundRect().draw(ColorF{ 0,0.3 });
	}
}

bool ChocolateButton::onMouseOver()const
{
	return getRoundRect().mouseOver();
}

std::shared_ptr<ChocolateButton> CreateChocolateButton(const Icon& icon, StringView text, const ColorF& color)
{
	return ChocolateButton::Create
	({
		.color = color,
		.padding = 20,
		.margine = 10,
		.width = 200,
		.child = Row::Create
		({
			.children
			{
				TextureUI::Create({.texture = Texture{icon,30},.margine = {0,5}}),
				TextUI::Create({.text = text,.color = Palette::White})
			}
		})
	});
}
