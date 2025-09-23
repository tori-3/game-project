#pragma once
#include "Lisence.h"

namespace Lisence
{
	void Init()
	{
		LicenseManager::DisableDefaultTrigger();

		LicenseManager::AddLicense
		({
			.title = U"効果音ラボ",
			.copyright = U"Copyright (c) 効果音ラボ",
			.text = U"https://soundeffect-lab.info"
		});

		LicenseManager::AddLicense
		({
			.title = U"On-Jin ～音人～",
			.copyright = U"Copyright (c) On-Jin ～音人～",
			.text = U"https://on-jin.com"
		});

		LicenseManager::AddLicense
		({
			.title = U"ザ・マッチメイカァズ",
			.copyright = U"Copyright (c) ザ・マッチメイカァズ",
			.text = U"https://osabisi.sakura.ne.jp/m2/index.html"
		});

		LicenseManager::AddLicense
		({
			.title = U"OtoLogic",
			.copyright = U"Copyright (c) OtoLogic",
			.text = U"License: CC BY 4.0\nhttps://otologic.jp"
		});

		LicenseManager::AddLicense({
			.title = U"夢にみた緑",
			.copyright = U"Copyright (c) 遊句",
			.text = U"https://yougou.sonnabakana.com"
		});

		LicenseManager::AddLicense({
			.title = U"DotGothic16",
			.copyright = U"Copyright (c) 2020 The DotGothic16 Project Authors",
			.text = U"https://github.com/fontworks-fonts/DotGothic16\nThis Font Software is licensed under the SIL Open Font License, Version 1.1.\nThis license is copied below, and is also available with a FAQ at :\nhttp://scripts.sil.org/OFL"
		});
	}

	std::shared_ptr<UIElement> LicenseDisplay()
	{
		auto col = Column::Create({});

		col->addChild(TextUI::Create({ .text = U"License",.fontSize = 30,.color = ColorF{1},.margine = 20 }));

		for (const LicenseInfo& info : LicenseManager::EnumLicenses())
		{
			col->addChild(TextUI::Create({ .text = info.title,.fontSize = 20,.color = ColorF{1},.margine{30,0,10,0} }));

			for (const auto& copyright : info.copyright.split_lines())
			{
				col->addChild(TextUI::Create({ .text = copyright,.fontSize = 13,.color = ColorF{0.9} }));
			}
			col->addChild(TextUI::Create({ .fontSize = 5 }));

			for (const auto& text : info.text.split_lines())
			{
				col->addChild(TextUI::Create({ .text = text,.fontSize = 10,.color = ColorF{0.7} }));
			}
		}

		return col;
	}
}
