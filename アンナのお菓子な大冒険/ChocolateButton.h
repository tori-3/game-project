#pragma once
#include"BunchoUI.hpp"

using  namespace BunchoUI;

/// @brief チョコレートボタン
class ChocolateButton :public BunchoUI::PanelBase
{
public:

	/// @brief ボタンの色
	ColorF color;

	bool selected = false;

	struct Parameter
	{
		ColorF color = Palette::White;/// @brief ボタンの色
		Padding padding;/// @brief 内側のUIとの間隔
		Margin margine;/// @brief 周りのUIとの間隔
		Optional<double> width;/// @brief 横幅(設定しないと自動計算)
		Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
		double flex = 0;/// @brief RowやColumn内での比率
		bool clickable = true;/// @brief クリック可能か
		Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief 子供のUI
	};

	ChocolateButton(const Parameter& para)
		: PanelBase{ para.padding,para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.child }
		, color{ para.color } {
	}

	[[nodiscard]]
	static std::shared_ptr<ChocolateButton>Create(const Parameter& para)
	{
		return std::make_shared<ChocolateButton>(para);
	}

protected:

	void onUpdate()override;

	void onDraw(const RectF& drawingArea)const override;

	bool onMouseOver()const override;

private:

	RoundRect getRoundRect()const noexcept
	{
		const RectF rect = getRect();
		return RoundRect{ rect,rect.h / 2.0 };
	}
};
