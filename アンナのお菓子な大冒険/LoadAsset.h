#pragma once

namespace LoadAsset
{
	void Init();

	void LoadStageTexture();

	void LoadAudio();

	void LoadFont();

	void Wait();

	void RegisterTexture(StringView path);

	void RegisterAudio(StringView path,Loop loop=Loop::No);
}
