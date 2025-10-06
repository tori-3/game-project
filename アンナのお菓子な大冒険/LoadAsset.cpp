#include"LoadAsset.h"
#include"BunchoUI.hpp"

void LoadAsset::Init()
{
	AudioAsset::Register(U"MiniGameBGM", U"BGM/MiniGameBGM.wav", Loop::Yes);

	LoadAsset::RegisterTexture(U"BackGroundTexture/雪原背景.png");
	LoadAsset::RegisterTexture(U"BackGroundTexture/雲背景.png");
	LoadAsset::RegisterTexture(U"BackGroundTexture/ラスボス背景.png");
	LoadAsset::RegisterTexture(U"BackGroundTexture/洞窟背景.png");
	LoadAsset::RegisterTexture(U"BackGroundTexture/宇宙背景.png");

	LoadAsset::LoadStageTexture();

	LoadAsset::LoadAudio();

	LoadAsset::LoadFont();
}

void LoadAsset::LoadStageTexture()
{
	//ステージの画像ファイルをTextureAssetに登録
	for (const auto& path : FileSystem::DirectoryContents(U"StageTexture"))
	{
		TextureAsset::Register(FileSystem::BaseName(path), path, TextureDesc::Mipped);
	}
}

void LoadAsset::LoadFont()
{
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 60, Typeface::Heavy);

	FontAsset::Register(U"NormalFont", FontMethod::MSDF, 25);

	FontAsset::Register(U"IconFont", FontMethod::MSDF, 25, Typeface::Icon_MaterialDesign);

	FontAsset::Register(U"EmojiFont", FontMethod::MSDF, 25, Typeface::ColorEmoji);


	// アイコンを表示するためのフォントを追加
	const Font iconFont = FontAsset{ U"IconFont" };
	const Font emojiFont = FontAsset{ U"EmojiFont" };

	FontAsset{ U"TitleFont" }.addFallback(iconFont);
	FontAsset{ U"NormalFont" }.addFallback(iconFont);

	FontAsset{ U"TitleFont" }.addFallback(emojiFont);
	FontAsset{ U"NormalFont" }.addFallback(emojiFont);


	BunchoUI::TextUI::DefaultFontName = U"NormalFont";

	FontAsset::LoadAsync(U"TitleFont");
	FontAsset::LoadAsync(U"NormalFont", U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	FontAsset::LoadAsync(U"IconFont");
}

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

	AudioAsset{ U"アンナちゃん" }.setVolume(3);

	AudioAsset{ U"妖精" }.setVolume(3);

	AudioAsset{ U"ドーナツ" }.setVolume(3);
	//AudioAsset{ U"ドーナツ" }.setLoop(true);

	AudioAsset{ U"ドーナツ衝突" }.setVolume(3);

	AudioAsset{ U"看板" }.setVolume(20);

	AudioAsset::Wait(U"キャンセル2");
}

void LoadAsset::Wait()
{
	for (auto&& [name, info] : AudioAsset::Enumerate())
	{
		AudioAsset::Wait(name);
	}

	for (auto&& [name, info] : TextureAsset::Enumerate())
	{
		TextureAsset::Wait(name);
	}

	for (auto&& [name, info] : FontAsset::Enumerate())
	{
		FontAsset::Wait(name);
	}
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
