#include"SweetsPanel.hpp"

void SweetsPanel::onUpdate()
{
	childUpdate();
}

void SweetsPanel::onDraw(const RectF& drawingArea)const
{
	ninePatch->setRect(getRect());
	ninePatch->draw();
	childDraw(drawingArea);
}

bool SweetsPanel::createNinePatch;
Optional<MyNinePatch> SweetsPanel::ninePatch;
NinePatchPosition SweetsPanel::pos;
