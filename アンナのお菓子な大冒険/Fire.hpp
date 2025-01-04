#pragma once

class Fire {
public:

	Fire(const Vec2& pos, const ColorF& color = HSV{ 20,0.8,1 }, double size = 50, const Vec2& force = { 0, -50 }) :particleSystem{ pos,force } {

		if (not TextureAsset::IsRegistered(U"particle"))
		{
			TextureAsset::Register(U"particle", U"particle.png");
		}

		ArcEmitter2D arcEmitter;
		parameters.startColor = color;
		parameters.startSize = size;
		particleSystem.setEmitter(arcEmitter);
		particleSystem.setTexture(TextureAsset(U"particle"));
		particleSystem.setParameters(parameters);
		particleSystem.prewarm();
	}

	void update() {
		particleSystem.update();
	}

	void draw()const {
		particleSystem.draw();
	}

	void setPosition(const Vec2& pos) {
		particleSystem.setPosition(pos);
	}

	void setForce(const Vec2& force) {
		particleSystem.setForce(force);
	}

private:
	ParticleSystem2D particleSystem;
	ParticleSystem2DParameters parameters;
};
