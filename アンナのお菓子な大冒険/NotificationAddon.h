#pragma once

class NotificationAddon : public IAddon
{
public:

	/// @brief 通知の種類
	enum class Type
	{
		Normal,

		Save,

		Minigame,

		Heart,

		Screenshot,

		Garelly
	};

	/// @brief 通知のスタイル
	struct Style
	{
		/// @brief 通知の幅
		double width = 380.0;

		/// @brief 通知の背景色
		ColorF backgroundColor{ 0.0, 0.8 };

		/// @brief 通知の文字色
		ColorF textColor{ 1.0 };

		ColorF minigameColor = Palette::Hotpink;

		ColorF saveColor{ 0.0, 0.78, 0.33 };

		ColorF heartColor = Palette::Red;

		ColorF screenShotColor = Palette::Lightblue;

		ColorF garellyColor = Palette::Mediumpurple;
	};

	/// @brief 通知を表示します。
	/// @param message メッセージ
	/// @param type 通知の種類
	static void Show(const StringView message, const StringView detail, const Type type = NotificationAddon::Type::Normal)
	{
		if (auto p = Addon::GetAddon<NotificationAddon>(U"NotificationAddon"))
		{
			p->show(message, detail, type);
		}
	}

	/// @brief 通知の表示時間を設定します。
	/// @param lifeTime 表示時間（秒）
	static void SetLifeTime(const double lifeTime)
	{
		if (auto p = Addon::GetAddon<NotificationAddon>(U"NotificationAddon"))
		{
			p->m_lifeTime = lifeTime;
		}
	}

	/// @brief 通知のスタイルを設定します。
	/// @param style スタイル
	static void SetStyle(const Style& style)
	{
		if (auto p = Addon::GetAddon<NotificationAddon>(U"NotificationAddon"))
		{
			p->m_style = style;
		}
	}

private:

	static constexpr StringView Icons = U" \U000F0E1E\U000F0E33\U000F02D1\U000F0E51\U000F00BE";

	struct Notification
	{
		String message;

		String detail;

		double time = 0.0;

		double currentIndex = 0.0;

		double velocity = 0.0;

		Type type = Type::Normal;
	};

	Style m_style;

	Array<Notification> m_notifications;

	double m_lifeTime = 10.0;

	bool update() override
	{
		const double deltaTime = Scene::DeltaTime();

		for (auto& notification : m_notifications)
		{
			notification.time += deltaTime;
		}

		m_notifications.remove_if([lifeTime = m_lifeTime](const Notification& notification) { return (lifeTime < notification.time); });

		for (size_t i = 0; i < m_notifications.size(); ++i)
		{
			auto& notification = m_notifications[i];
			notification.currentIndex = Math::SmoothDamp(notification.currentIndex,
				static_cast<double>(i), notification.velocity, 0.15, 9999.0, deltaTime);
		}

		return true;
	}

	void draw() const override
	{
		const Font& font = FontAsset{ U"NormalFont" };

		for (const auto& notification : m_notifications)
		{
			double xScale = 1.0;
			double alpha = 1.0;

			if (notification.time < 0.2)
			{
				xScale = alpha = (notification.time / 0.2);
			}
			else if ((m_lifeTime - 0.4) < notification.time)
			{
				alpha = ((m_lifeTime - notification.time) / 0.4);
			}

			alpha = EaseOutExpo(alpha);
			xScale = EaseOutExpo(xScale);

			ColorF backgroundColor = m_style.backgroundColor;
			backgroundColor.a *= alpha;

			ColorF textColor = m_style.textColor;
			textColor.a *= alpha;

			constexpr double height = 80;
			const RectF rect{ 10, (Scene::Height() - height) - (10 + notification.currentIndex * height), (m_style.width * xScale), height - 1 };
			rect.rounded(3).draw(backgroundColor);

			if (notification.type != Type::Normal)
			{
				ColorF color = notification.type == Type::Save ? m_style.saveColor
					: notification.type == Type::Minigame ? m_style.minigameColor
					: notification.type == Type::Heart ? m_style.heartColor
					: notification.type == Type::Screenshot ? m_style.screenShotColor
					: m_style.garellyColor;
				color.a *= alpha;

				font(Icons[FromEnum(notification.type)]).draw(height * 0.8, Arg::leftCenter = rect.leftCenter().movedBy(8, -1), color);
			}

			font(notification.message).draw(23, rect.pos.movedBy(height * 0.8 + 15, 0), textColor);
			font(notification.detail).draw(15, rect.pos.movedBy(height * 0.8 + 15, 30), textColor * 0.8);
		}
	}

	void show(const StringView message, const StringView detail, const Type type)
	{
		const double currentIndex = (m_notifications.empty() ? 0.0 : m_notifications.back().currentIndex + 1.0);
		const double velocity = (m_notifications.empty() ? 0.0 : m_notifications.back().velocity);

		m_notifications << Notification{
			.message = String{ message },
			.detail = String{detail},
			.time = 0.0,
			.currentIndex = currentIndex,
			.velocity = velocity,
			.type = type };
	}
};
