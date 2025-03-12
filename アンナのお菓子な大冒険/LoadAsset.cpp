#include"LoadAsset.h"

void LoadAsset::LoadAudio()
{

	for (auto& path : FileSystem::DirectoryContents(U"Audio"))
	{
		String name = FileSystem::BaseName(path);
		AudioAsset::Register(name, path);
		AudioAsset::LoadAsync(name);
	}

	AudioAsset{ U"ゲームオーバー" }.setVolume(10);

	AudioAsset{ U"足音" }.setVolume(3);

	AudioAsset{ U"ビーム" }.setVolume(3);

	AudioAsset{ U"雪玉壊れる" }.setVolume(5);
}

void LoadAsset::RegisterTexture(StringView path)
{
	if (not TextureAsset::IsRegistered(path))
	{
		TextureAsset::Register(path, path);
	}
}

void LoadAsset::RegisterAudio(StringView path,Loop loop)
{
	if (not AudioAsset::IsRegistered(path))
	{
		AudioAsset::Register(path, path,loop);
	}
}
