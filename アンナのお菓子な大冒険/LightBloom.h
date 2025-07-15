#pragma once

class LightBloom
{
public:
	const RenderTexture gaussianA1, gaussianB1;
	const RenderTexture gaussianA4, gaussianB4;
	const RenderTexture gaussianA8, gaussianB8;
	const Size sceneSize;

	LightBloom(const Size& size)
		:sceneSize{ size }
		,gaussianA1{ size }, gaussianB1{ size }
		,gaussianA4{ size / 4 }, gaussianB4{ size / 4 }
		,gaussianA8{ size / 8 }, gaussianB8{ size / 8 }
	{}

	void draw(const Vec2& pos = { 0,0 })const
	{
		Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
		Shader::Downsample(gaussianA1, gaussianA4);
		Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
		Shader::Downsample(gaussianA4, gaussianA8);
		Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
		const ScopedRenderStates2D blend{ BlendState::Additive };
		gaussianA1.resized(sceneSize).draw(pos, ColorF{ 0.1 });
		gaussianA4.resized(sceneSize).draw(pos, ColorF{ 0.4 });
		gaussianA8.resized(sceneSize).draw(pos, ColorF{ 0.8 });
	}
};

class ScopedLightBloom
{
public:
	ScopedLightBloom(const LightBloom& lightBloom) :target{ lightBloom.gaussianA1.clear(ColorF{ 0.0 }) } {}
	const ScopedRenderTarget2D target;
	const ScopedRenderStates2D blend{ BlendState::Additive };
};
