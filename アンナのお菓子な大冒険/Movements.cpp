#include "Movements.h"
#include"MotionLoader.h"


//MotionLoaderとの相互参照を避けるため、定義と実装を分離

void StartMotion::start(Character* character)
{
	TimeMove::start(character);
	motion = MotionLoader{ csv }.LoadMotion(motionName);
	motion.loop = loop;
	motion.start(character);
}

void StartMotion::update(Character* character, double dt)
{
	dt = calTime(dt * timeScale);
	motion.update(character, dt);
}

bool StartMotion::isActive()
{
	return TimeMove::isActive() and motion.isActive();
}
