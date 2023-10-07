#pragma once


inline BlendState MakeBlendState()
{
	BlendState blendState = BlendState::Additive;
	blendState.srcAlpha = Blend::SrcAlpha;
	blendState.dstAlpha = Blend::DestAlpha;
	blendState.opAlpha = BlendOp::Max;
	return blendState;
}

struct ExplosionEffect : IEffect
{
	ParticleSystem2D particleSystem;
	ParticleSystem2DParameters parameters;

	//Vec2 位置　double 大きさ　ColorF 色
	explicit ExplosionEffect(const Vec2& pos, double size = 100, const ColorF& color = HSV{ 20,0.8,1 }) {
		if (not TextureAsset::IsRegistered(U"particle"))
		{
			TextureAsset::Register(U"particle", U"example/particle.png");
		}
		CircleEmitter2D emitter;
		emitter.r = size * 0.35;
		parameters.startSize = size;
		parameters.startSpeed = size;
		parameters.startColor = color;

		parameters.blendState = MakeBlendState();

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




//https://zenn.dev/reputeless/books/siv3d-documentation/viewer/tutorial-effect#22.7-%EF%BC%88%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%EF%BC%89%E9%A3%9B%E3%81%B3%E6%95%A3%E3%82%8B%E6%98%9F

struct StarEffect : IEffect
{
	static constexpr Vec2 Gravity{ 0, 160 };

	struct Star
	{
		Vec2 start;
		Vec2 velocity;
		ColorF color;
	};

	Array<Star> m_stars;

	StarEffect(const Vec2& pos, double baseHue)
	{
		for (int32 i = 0; i < 6; ++i)
		{
			const Vec2 velocity = RandomVec2(Circle{ 60 });
			Star star{
				.start = (pos + velocity),
				.velocity = velocity,
				.color = HSV{ baseHue + Random(-20.0, 20.0) },
			};
			m_stars << star;
		}
	}

	bool update(double t) override
	{
		t /= 0.4;

		for (auto& star : m_stars)
		{
			const Vec2 pos = star.start
				+ star.velocity * t + 0.5 * t * t * Gravity;

			const double angle = (pos.x * 3_deg);

			Shape2D::Star((30 * (1.0 - t)), pos, angle)
				.draw(star.color);
		}

		return (t < 1.0);
	}
};
