#pragma once

class Fire
{
public:

	Fire(const Vec2& pos, const ColorF& color = HSV{ 20,0.8,1 }, double size = 50, const Vec2& force = { 0, -50 })
		:m_particleSystem{ pos,force }
	{

		if (not TextureAsset::IsRegistered(U"particle"))
		{
			TextureAsset::Register(U"particle", U"particle.png");
		}

		ArcEmitter2D arcEmitter;
		m_parameters.startColor = color;
		m_parameters.startSize = size;
		m_particleSystem.setEmitter(arcEmitter);
		m_particleSystem.setTexture(TextureAsset(U"particle"));
		m_particleSystem.setParameters(m_parameters);
		m_particleSystem.prewarm();
	}

	void update()
	{
		m_particleSystem.update();
	}

	void draw()const
	{
		m_particleSystem.draw();
	}

	void setPosition(const Vec2& pos)
	{
		m_particleSystem.setPosition(pos);
	}

	void setForce(const Vec2& force)
	{
		m_particleSystem.setForce(force);
	}

private:
	ParticleSystem2D m_particleSystem;
	ParticleSystem2DParameters m_parameters;
};
