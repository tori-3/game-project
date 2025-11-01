#pragma once
#include"BunchoUI.hpp"

using namespace BunchoUI;

/// @brief トグルボタン
class ToggleButton :public UIElement
{
public:

	/// @brief 値
	bool value;

	/// @brief トグルボタンの色
	ColorF color{ 0.3, 0.5, 1.0 };

	/// @brief オフのときの色
	ColorF offColor = Palette::Black;

	struct Parameter
	{
		bool value = false;/// @brief 値
		double size = 60;/// @brief 大きさ
		ColorF color{ 0.3, 0.5, 1.0 };/// @brief トグルボタンの色
		ColorF offColor = Palette::Black;/// @brief オフのときの色
		Margin margine;/// @brief 周りのUIとの間隔
		Optional<double> width;/// @brief 横幅(設定しないと自動計算)
		Optional<double> height;/// @brief 縦幅(設定しないと自動計算)
		double flex = 0;/// @brief RowやColumn内での比率
		bool clickable = true;/// @brief クリック可能か
		Optional<Relative> relative = none;/// @brief 相対座標(右寄せ、中央寄せなどを指定)
	};

	ToggleButton(const Parameter& para)
		: UIElement{ para.margine,para.width,para.height,para.flex,para.clickable,para.relative }
		, m_size{ para.size }
		, value{ para.value }
		, color{ para.color }
		, offColor{ para.offColor }
		, m_transition{ 0.3s, 0.3s, double(para.value) } {
	}

	[[nodiscard]]
	static std::shared_ptr<ToggleButton>Create(const Parameter& para)
	{
		return std::make_shared<ToggleButton>(para);
	}

	void setToggleSize(double size)noexcept
	{
		m_size = size;
		changeSize();
	}

	[[nodiscard]]
	double getToggleSize()const noexcept
	{
		return m_size;
	}

	[[nodiscard]]
	double getTransitionValue()const noexcept
	{
		return m_transition.value();
	}
protected:

	void onUpdate()override
	{
		m_transition.update(value);
		if (clicked())value = not value;
	}

	void onDraw(const RectF&)const override
	{
		const RoundRect button = getRoundRect();

		button.draw(offColor.lerp(color, m_transition.value()));
		button.drawFrame(m_size / 30.0);

		Circle{ button.center() + Vec2{m_size * (m_transition.value() - 0.5) / 2.0,0},m_size * 0.2 }.draw(Palette::Lightgray);

		if (not clickable)
		{
			button.draw(ColorF{ 0.5,0.7 });
		}
	}

	bool onMouseOver()const override
	{
		return getRoundRect().mouseOver();
	}

	SizeF onGetSize()const override
	{
		return { m_size,m_size / 2.0 };
	}

private:

	RoundRect getRoundRect()const noexcept
	{
		return RoundRect{ Arg::center = getRect().center(),m_size,m_size / 2.0,m_size / 4.0 };
	}

	double m_size;
	Transition m_transition;
};
