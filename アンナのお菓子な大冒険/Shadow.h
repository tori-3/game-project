#pragma once

class Shadow
{
public:

	const RenderTexture shadowTexture;
	const RenderTexture blur4;
	const RenderTexture internal4;
	const RenderTexture internal1;

	Shadow(const Size& size = Scene::Size())
		: shadowTexture{ size,ColorF{ 1.0, 0.0 } }
		, blur4{ shadowTexture.size() / 4 }
		, internal4{ shadowTexture.size() / 4 }
		, internal1{ shadowTexture.size() }
	{
	}

	void draw()const
	{
		Shader::GaussianBlur(shadowTexture, internal1, shadowTexture);
		Shader::Downsample(shadowTexture, blur4);
		Shader::GaussianBlur(blur4, internal4, blur4);
		blur4.resized(Scene::Size()).draw(ColorF{ 0.0, 0.5 });
	}
};

class ScopedShadow
{
public:
	ScopedShadow(const Shadow& shadow, const Vec2& delta = { 2,2 })
		: target{ shadow.shadowTexture.clear(ColorF{ 1.0, 0.0 }) }
		, transform{ Mat3x2::Translate(delta) }
	{
	}

private:
	const ScopedRenderTarget2D target;
	const ScopedRenderStates2D blend{ BlendState::MaxAlpha };
	const Transformer2D transform;
};
