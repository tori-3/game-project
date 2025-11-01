#pragma once
#include "Lisence.h"

namespace Lisence
{
	void Init()
	{
		//LicenseManager::DisableDefaultTrigger();

		//LicenseManager::AddLicense
		//({
		//	.title = U"効果音ラボ",
		//	.copyright = U"Copyright (c) 効果音ラボ",
		//	.text = U"https://soundeffect-lab.info"
		//});

		//LicenseManager::AddLicense
		//({
		//	.title = U"On-Jin ～音人～",
		//	.copyright = U"Copyright (c) On-Jin ～音人～",
		//	.text = U"https://on-jin.com"
		//});

		//LicenseManager::AddLicense
		//({
		//	.title = U"ザ・マッチメイカァズ",
		//	.copyright = U"Copyright (c) ザ・マッチメイカァズ",
		//	.text = U"https://osabisi.sakura.ne.jp/m2/index.html"
		//});

		//LicenseManager::AddLicense
		//({
		//	.title = U"OtoLogic",
		//	.copyright = U"Copyright (c) OtoLogic",
		//	.text = U"License: CC BY 4.0\nhttps://otologic.jp"
		//});

		//LicenseManager::AddLicense({
		//	.title = U"夢にみた緑",
		//	.copyright = U"Copyright (c) 遊句",
		//	.text = U"https://yougou.sonnabakana.com"
		//});

		//LicenseManager::AddLicense({
		//	.title = U"Kenney Input Prompts",
		//	.copyright = U"",
		//	.text = U"License: CC0 1.0\nhttps://kenney.nl/assets/input-prompts"
		//});

		LicenseManager::AddLicense({
			.title = U"DotGothic16",
			.copyright = U"Copyright (c) 2020 The DotGothic16 Project Authors",
			.text = U"https://github.com/fontworks-fonts/DotGothic16\nThis Font Software is licensed under the SIL Open Font License, Version 1.1.\nThis license is copied below, and is also available with a FAQ at :\nhttp://scripts.sil.org/OFL"
		});
	}

	std::shared_ptr<UIElement> LicenseDisplay()
	{
		//auto col = Column::Create({});

		//col->addChild(TextUI::Create({ .text = U"License",.fontSize = 30,.color = ColorF{1},.margine = 20 }));

		//for (const LicenseInfo& info : LicenseManager::EnumLicenses())
		//{
		//	col->addChild(TextUI::Create({ .text = info.title,.fontSize = 20,.color = ColorF{1},.margine{30,0,10,0} }));

		//	for (const auto& copyright : info.copyright.split_lines())
		//	{
		//		col->addChild(TextUI::Create({ .text = copyright,.fontSize = 13,.color = ColorF{0.9} }));
		//	}
		//	col->addChild(TextUI::Create({ .fontSize = 5 }));

		//	for (const auto& text : info.text.split_lines())
		//	{
		//		col->addChild(TextUI::Create({ .text = text,.fontSize = 10,.color = ColorF{0.7} }));
		//	}
		//}

		//return col;

		const Array<std::pair<String, String>>texts
		{
			{U"・とりさん  ",U"🚩プロジェクト進行　💻プログラミング　🔨ステージ"},
			{U"・やっさん  ",U"💡企画　💻プログラミング　🎨イラスト(背景、ステージ素材、キャラクター)　💫モーション"},
			{U"・まる  ",U"🎵BGM/効果音　🎨イラスト(ステージ素材、キャラクター)　🔨ステージ　🕹️ まんじゅうラッシュ　💫モーション"},
			{U"・With Ball  ",U"👤ディレクター　🕹️ ふぉーりんぐアンナちゃん"},
			{U"・Sco Ttishfold  ",U"🎨イラスト(ステージ素材)"},
			{U"・ひー  ",U"🕹️ まっくら迷路"},
			{U"・なかこう  ",U"🕹️ ぐるぐる迷路"},
			{U"・ゆーれい  ",U"🕹 リズムDEアンナちゃん"},
			{U"・ぺってぃー  ",U"🕹️ 大鷹跳び　💡発案者"},
			{U"・たわし  ",U"🎨イラスト(ポップコーン)"}
		};

		const Array<std::pair<String,String>>texts2
		{
			{U"・効果音ラボ (https://soundeffect-lab.info)  ",U"効果音素材"},
			{U"・On-Jin ～音人～ (https://on-jin.com)  ",U"効果音素材"},
			{U"・ザ・マッチメイカァズ (https://osabisi.sakura.ne.jp/m2/index.html)  ",U"効果音素材"},
			{U"・OtoLogic (https://otologic.jp)  ",U"効果音素材　🕹️ まっくら迷路 BGM"},
			{U"・夢にみた緑 (https://yougou.sonnabakana.com)  ",U"🕹️ ふぉーりんぐアンナちゃん BGM"},
			{U"・TAM Music Factory (https://www.tam-music.com)  ",U"効果音素材"},
			{U"・エスフェクツ (https://esffects.net)  ",U"効果音素材"},
			{U"・いらすとや (https://www.irasutoya.com)  ",U"一部ミニゲーム イラスト素材"},
			{U"・Input Prompts · Kenney (https://kenney.nl/assets/input-prompts)  ",U"キーの画像"},
		};

		auto col = Column::Create({.crossAxis=CrossAxis::start,.margine=18});

		//col->addChild(TextUI::Create({ .text = U"詳細情報",.fontSize = 30,.color = ColorF{1},.margine = 20,.relative=Relative::Center()}));

		//col->addChild(TextUI::Create({ .text = U"制作",.fontSize = 30,.color = ColorF{1} }));

		col->addChild(TextUI::Create({ .text = U"使用させていただいた素材",.fontSize = 30,.color = ColorF{1} }));

		for (const auto& text : texts2)
		{
			auto row = Row::Create({ .mainAxis = MainAxis::start });
			row->addChild(TextUI::Create({ .text = text.first,.fontSize = 20,.color = ColorF{1} }));
			row->addChild(TextUI::Create({ .text = text.second,.fontSize = 17,.color = ColorF{0.7} }));
			col->addChild(row);
		}

		col->addChild(TextUI::Create({ .fontSize = 20 }));

		col->addChild(TextUI::Create({ .text = U"制作：東京都市大学コンピュータ技術研究会",.fontSize = 30,.color = ColorF{1} }));

		for(const auto& text:texts)
		{
			auto row = Row::Create({.mainAxis=MainAxis::start});
			row->addChild(TextUI::Create({ .text = text.first,.fontSize = 20,.color = ColorF{1} }));
			row->addChild(TextUI::Create({ .text = text.second,.fontSize = 17,.color = ColorF{0.7} }));
			col->addChild(row);
		}

		col->addChild(TextUI::Create({ .fontSize = 20 }));

		col->addChild(TextUI::Create({ .text = U"ライセンス",.fontSize = 30,.color = ColorF{1} }));

		for (const LicenseInfo& info : LicenseManager::EnumLicenses())
		{
			col->addChild(TextUI::Create({ .text = info.title,.fontSize = 20,.color = ColorF{1} }));

			for (const auto& copyright : info.copyright.split_lines())
			{
				col->addChild(TextUI::Create({ .text = copyright,.fontSize = 13,.color = ColorF{0.9} }));
			}
			col->addChild(TextUI::Create({ .fontSize = 5 }));

			for (const auto& text : info.text.split_lines())
			{
				col->addChild(TextUI::Create({ .text = text,.fontSize = 10,.color = ColorF{0.7} }));
			}

			col->addChild(TextUI::Create({ .fontSize = 20 }));
		}

		return col;



	}
}
