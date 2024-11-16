class Spotlight
{
public:
	const MSRenderTexture renderTexture;

	Spotlight(const Size& size = Scene::Size())
		:renderTexture{ size } {}

	void draw()const
	{
		const ScopedRenderStates2D blend{ BlendState::Multiplicative };
		Graphics2D::Flush();
		renderTexture.resolve();
		renderTexture.draw();
	}
};

class ScopedSpotlight
{
public:
	ScopedSpotlight(const Spotlight& spotlight, const ColorF& color = ColorF{ 0 })
		:target{ spotlight.renderTexture.clear(color) } {}
private:
	const ScopedRenderTarget2D target;
	const ScopedRenderStates2D blend{ BlendState::Additive };
};
