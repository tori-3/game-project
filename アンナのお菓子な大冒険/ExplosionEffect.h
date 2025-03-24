#pragma once
#include"MyBlendState.h"

struct ExplosionEffect : IEffect
{
	ParticleSystem2D particleSystem;
	ParticleSystem2DParameters parameters;

	//Vec2 位置　double 大きさ　ColorF 色
	explicit ExplosionEffect(const Vec2& pos, double size = 100, const ColorF& color = HSV{ 20,0.8,1 }) {
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

	bool update(double t) override
	{
		parameters.rate = t < 0.2 ? 100 : 0;
		particleSystem.setParameters(parameters);
		particleSystem.update();
		particleSystem.draw();
		return (t < 1.0);
	}
};
