#pragma once
#include"MyBlendState.h"

struct ExplosionEffect : IEffect
{
	ParticleSystem2D particleSystem;
	ParticleSystem2DParameters parameters;

	//Vec2 位置　double 大きさ　ColorF 色
	explicit ExplosionEffect(const Vec2& pos, double size = 100, const ColorF& color = HSV{ 20,0.8,1 });

	bool update(double t) override;
};
