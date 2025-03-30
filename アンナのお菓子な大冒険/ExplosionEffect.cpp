#include"ExplosionEffect.h"

ExplosionEffect::ExplosionEffect(const Vec2& pos, double size, const ColorF& color)
{
	if (not TextureAsset::IsRegistered(U"particle"))
	{
		TextureAsset::Register(U"particle", U"particle.png");
	}
	CircleEmitter2D emitter;
	emitter.r = size * 0.35;
	parameters.startSize = size;
	parameters.startSpeed = size;
	parameters.startColor = color;

	parameters.blendState = MakeTransparentBlendState();

	particleSystem.setEmitter(emitter);
	particleSystem.setTexture(TextureAsset(U"particle"));
	particleSystem.setParameters(parameters);
	particleSystem.setPosition(pos);
	particleSystem.prewarm();
}

bool ExplosionEffect::update(double t)
{
	parameters.rate = t < 0.2 ? 100 : 0;
	particleSystem.setParameters(parameters);
	particleSystem.update();
	particleSystem.draw();
	return (t < 1.0);
}
