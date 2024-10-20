#pragma once

namespace LoadAsset
{

	void LoadAudio();

	void RegisterTexture(StringView path);

	void RegisterAudio(StringView path,Loop loop=Loop::No);
}
