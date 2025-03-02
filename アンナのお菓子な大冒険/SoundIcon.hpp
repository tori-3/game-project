#pragma once
#include"BunchoUI.hpp"

using namespace BunchoUI;

/// @brief 音量アイコン
class SoundIcon :public UIElement
{
public:

	/// @brief アイコンの色
	ColorF color;

	/// @brief 音量
	double volume;

	struct Parameter
	{
		double size = 50;/// @brief アイコンの大きさ
		double volume = 1;/// @brief 音量
		ColorF color = Palette::White;/// @brief アイコンの色
		Padding padding;/// @brief 内側のUIとの間隔
		Margin margine;/// @brief 周りのUIとの間隔
		Optional<double> width;/// @brief 横幅(設定しないと自動計算)
		Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
		double flex = 0;/// @brief RowやColumn内での比率
		bool clickable = false;/// @brief クリック可能か
		Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief 子供のUI
	};

	SoundIcon(const Parameter& para)
		: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative }
		, color{ para.color }
		, m_size{ para.size }
		, volume{ para.volume }
	{
	}

	[[nodiscard]]
	static std::shared_ptr<SoundIcon>Create(const Parameter& para)
	{
		return std::make_shared<SoundIcon>(para);
	}

	SizeF onGetSize()const override;

protected:

	void onDraw(const RectF&)const override;

private:

	double m_size;
};
