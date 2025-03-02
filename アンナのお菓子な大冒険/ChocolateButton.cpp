#include"ChocolateButton.h"

void ChocolateButton::onUpdate()
{
	childUpdate();
}

void ChocolateButton::onDraw(const RectF& drawingArea)const
{
	if (selected)
	{
		getRoundRect().drawFrame(0, 5, Palette::Skyblue);
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
