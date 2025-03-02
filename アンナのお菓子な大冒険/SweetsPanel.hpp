#pragma once
#include"BunchoUI.hpp"
#include"NinePatch.hpp"

using namespace BunchoUI;

/// @brief お菓子のパネル
class SweetsPanel :public PanelBase
{
public:

	struct Parameter
	{
		Padding padding = 80;/// @brief 内側のUIとの間隔
		Margin margine;/// @brief 周りのUIとの間隔
		Optional<double> width;/// @brief 横幅(設定しないと自動計算)
		Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
		double flex = 0;/// @brief RowやColumn内での比率
		bool clickable = false;/// @brief クリック可能か
		Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
		std::shared_ptr<UIElement>child = NoneUI::Create();/// @brief 子供のUI
	};

	SweetsPanel(const Parameter& para)
		: PanelBase{ para.padding,para.margine,para.width,para.height,para.flex,para.clickable,para.relative,para.child }
	{
		if(not createNinePatch)
		{
			ninePatch = MyNinePatch{};
			pos.setWidthRate(1, 1, 1);
			pos.setHeightRate(1, 1, 1);
			ninePatch->setTexture(Texture{ U"9-slice.png" }, pos);
			ninePatch->baseScale = 0.8;
			ninePatch->aspect = { 1,1 };
			createNinePatch = true;
		}
	}

	[[nodiscard]]
	static std::shared_ptr<SweetsPanel>Create(const Parameter& para)
	{
		return std::make_shared<SweetsPanel>(para);
	}

protected:

	void onUpdate()override;

	void onDraw(const RectF& drawingArea)const override;

private:
	static bool createNinePatch;
	static Optional<MyNinePatch> ninePatch;
	static NinePatchPosition pos;
};



