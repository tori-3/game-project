#pragma once

//図形クラス(Circle, RectF, Quad, Triangle, Ellipse, Polygon)をまとめて扱えるクラス

namespace FigureIndex
{
	enum { Circle, RectF, Quad, Triangle, Ellipse, Polygon };
}

class Figure
{
public:

	using Shape = std::variant<Circle, RectF, Quad, Triangle, Ellipse, Polygon>;

	Shape shape;

	Figure() {}

	template<class Shape2DType>constexpr Figure(const Shape2DType& s)noexcept :shape{ s } {}

	template<class Shape2DType>constexpr Figure& set(const Shape2DType& s)noexcept
	{
		shape = s;
		return *this;
	}

	template<class Shape2DType>constexpr Figure& operator=(const Shape2DType& s)noexcept
	{
		shape = s;
		return *this;
	}

	[[nodiscard]]
	const Circle& getCircle()const
	{
		return std::get<Circle>(shape);
	}

	[[nodiscard]]
	const RectF& getRectF()const
	{
		return std::get<RectF>(shape);
	}

	[[nodiscard]]
	const Quad& getQuad()const
	{
		return std::get<Quad>(shape);
	}

	[[nodiscard]]
	const Triangle& getTriangle()const
	{
		return std::get<Triangle>(shape);
	}

	[[nodiscard]]
	const Ellipse& getEllipse()const
	{
		return std::get<Ellipse>(shape);
	}

	[[nodiscard]]
	const Polygon& getPolygon()const
	{
		return std::get<Polygon>(shape);
	}

	const Figure& draw(const ColorF& color = Palette::White)const
	{
		std::visit([&color](auto& x) {x.draw(color); }, shape);
		return *this;
	}

	const Figure& drawFrame(double thickness = 1.0, const ColorF& color = Palette::White)const
	{
		std::visit([&thickness, &color](auto& x) {x.drawFrame(thickness, color); }, shape);
		return *this;
	}

	[[nodiscard]]
	bool mouseOver()const noexcept
	{
		return std::visit([](auto& x) {return x.mouseOver(); }, shape);
	}

	[[nodiscard]]
	bool rightReleased()const noexcept
	{
		return std::visit([](auto& x) {return x.rightReleased(); }, shape);
	}

	[[nodiscard]]
	bool rightPressed()const noexcept
	{
		return std::visit([](auto& x) {return x.rightPressed(); }, shape);
	}

	[[nodiscard]]
	bool rightClicked()const noexcept
	{
		return std::visit([](auto& x) {return x.rightClicked(); }, shape);
	}

	[[nodiscard]]
	bool leftReleased()const noexcept
	{
		return std::visit([](auto& x) {return x.leftReleased(); }, shape);
	}

	[[nodiscard]]
	bool leftPressed()const noexcept
	{
		return std::visit([](auto& x) {return x.leftPressed(); }, shape);
	}

	[[nodiscard]]
	bool leftClicked()const noexcept
	{
		return std::visit([](auto& x) {return x.leftClicked(); }, shape);
	}

	template<class Shape2DType>
	[[nodiscard]]
	constexpr bool intersects(const Shape2DType& other)const
	{
		return std::visit([&other](auto& x) {return x.intersects(other); }, shape);
	}

	[[nodiscard]]
	constexpr bool intersects(const Figure& other)const
	{
		return std::visit([this](auto& x) {return this->intersects(x); }, other.shape);
	}

	template<class Shape2DType>
	[[nodiscard]]
	Optional<Array<Vec2>> intersectsAt(const Shape2DType& other)const
	{
		return std::visit([&other](auto& x) {return x.intersectsAt(other); }, shape);
	}

	[[nodiscard]]
	Optional<Array<Vec2>> intersectsAt(const Figure& other)const
	{
		switch (other.shape.index())
		{
		case 0:return intersectsAt(std::get<0>(other.shape));
		case 1:return intersectsAt(std::get<1>(other.shape));
		case 2:return intersectsAt(std::get<2>(other.shape));
		case 3:return intersectsAt(std::get<3>(other.shape));
		case 4:return intersectsAt(std::get<4>(other.shape));
		case 5:
			if (shape.index() != 5)
			{
				return other.intersectsAt(*this);
			}
			else return {};
		}
	}

	const Figure& paint(Image& dst, const Color& color)const
	{
		std::visit([&dst, &color](auto& x) {x.paint(dst, color); }, shape);
		return *this;
	}

	const Figure& overwrite(Image& dst, const Color& color)const
	{
		std::visit([&dst, &color](auto& x) {x.overwrite(dst, color); }, shape);
		return *this;
	}

	constexpr Figure& moveBy(const Vec2& vec)noexcept
	{
		std::visit([&vec](auto& x) {x.moveBy(vec); }, shape);
		return *this;
	}

	constexpr Figure& moveBy(double x, double y)noexcept
	{
		return moveBy(Vec2{ x,y });
	}

	[[nodiscard]]
	constexpr Figure movedBy(const Vec2& vec)const noexcept
	{
		return Figure{ shape }.moveBy(vec);
	}

	[[nodiscard]]
	constexpr Figure movedBy(double x, double y)const noexcept
	{
		return movedBy(Vec2{ x,y });
	}

	[[nodiscard]]
	Polygon asPolygon()const
	{
		switch (shape.index())
		{
		case 0:return std::get<0>(shape).asPolygon();
		case 1:return std::get<1>(shape).asPolygon();
		case 2:return std::get<2>(shape).asPolygon();
		case 3:return std::get<3>(shape).asPolygon();
		case 4:return std::get<4>(shape).asPolygon();
		case 5:return std::get<5>(shape);
		}
	}

	constexpr Figure& scaleAt(const Vec2& pos, double x, double y)noexcept
	{
		switch (shape.index())
		{
		case 0:
		{
			const Vec2 vec = center() - pos;
			setCenter(vec * Vec2{ x,y } + pos);
			if (x == y) shape = std::get<0>(shape).scaled(x);
			else shape = std::get<0>(shape).scaled(x, y);
			break;
		}
		case 1:
			shape = std::get<1>(shape).scaledAt(pos, x, y);
			break;
		case 2:
			std::get<2>(shape).scaleAt(pos, x, y);
			break;
		case 3:
			std::get<3>(shape).scaleAt(pos, x, y);
			break;
		case 4:
		{
			const Vec2 vec = center() - pos;
			setCenter(vec * Vec2{ x,y } + pos);
			shape = std::get<4>(shape).scaled(x, y);
			break;
		}
		case 5:
			std::get<5>(shape).scaleAt(pos, x, y);
		}
		return *this;
	}

	constexpr Figure& scaleAt(const Vec2& pos, double s)noexcept
	{
		return scaleAt(pos, s, s);
	}

	[[nodiscard]]
	constexpr Figure scaledAt(const Vec2& pos, double x, double y)const noexcept
	{
		return Figure{ shape }.scaleAt(pos, x, y);
	}

	[[nodiscard]]
	constexpr Figure scaledAt(const Vec2& pos, double s)const noexcept
	{
		return scaledAt(pos, s, s);
	}

	constexpr Figure& scale(double x, double y)noexcept
	{
		return scaleAt(center(), x, y);
	}

	constexpr Figure& scale(double s)noexcept
	{
		return scaleAt(center(), s);
	}

	[[nodiscard]]
	constexpr Figure scaled(double x, double y)const noexcept
	{
		return Figure{ shape }.scale(x, y);
	}

	[[nodiscard]]
	constexpr Figure scaled(double s)const noexcept
	{
		return scaled(s, s);
	}

	constexpr Figure& rotateAt(const Vec2& pos, double angle)noexcept
	{
		switch (shape.index())
		{
		case 0:
			std::get<0>(shape).center.rotateAt(pos, angle);
			break;
		case 1:
			shape = std::get<1>(shape).rotatedAt(pos, angle);
			break;
		case 2:
			shape = std::get<2>(shape).rotatedAt(pos, angle);
			break;
		case 3:
			shape = std::get<3>(shape).rotatedAt(pos, angle);
			break;
		case 4:
			shape = std::get<4>(shape).asPolygon().rotatedAt(pos, angle);
			break;
		case 5: std::get<5>(shape).rotateAt(pos, angle);
		}
		return *this;
	}

	constexpr Figure& rotateAt(double x, double y, double angle)noexcept
	{
		return rotateAt(Vec2{ x,y }, angle);
	}

	[[nodiscard]]
	constexpr Figure rotatedAt(const Vec2& pos, double angle)const noexcept
	{
		return Figure{ shape }.rotateAt(pos, angle);
	}

	[[nodiscard]]
	constexpr Figure rotatedAt(double x, double y, double angle)const noexcept
	{
		return rotatedAt(Vec2{ x,y }, angle);
	}

	constexpr Figure& rotate(double angle)noexcept
	{
		return rotateAt(center(), angle);
	}

	[[nodiscard]]
	constexpr Figure rotated(double angle)const noexcept
	{
		return Figure{ shape }.rotate(angle);
	}

	[[nodiscard]]
	constexpr LineString outline()const
	{
		switch (shape.index())
		{
		case 0:return std::get<0>(shape).asPolygon().outline();
		case 1:return std::get<1>(shape).outline();
		case 2:return std::get<2>(shape).outline();
		case 3:return std::get<3>(shape).outline();
		case 4:return std::get<4>(shape).asPolygon().outline();
		case 5:return std::get<5>(shape).outline();
		}
	}

	[[nodiscard]]
	constexpr Vec2 center()const noexcept
	{
		switch (shape.index())
		{
		case 0:return std::get<0>(shape).center;
		case 1:return std::get<1>(shape).center();
		case 2:return (std::get<2>(shape).triangle(0).centroid() + std::get<2>(shape).triangle(1).centroid()) / 2.0;
		case 3:return std::get<3>(shape).centroid();
		case 4:return std::get<4>(shape).center;
		case 5:return std::get<5>(shape).centroid();
		}
	}

	constexpr Figure& setCenter(const Vec2& vec)noexcept
	{
		switch (shape.index())
		{
		case 0:
			std::get<0>(shape).setCenter(vec);
			break;
		case 1:
			std::get<1>(shape).setCenter(vec);
			break;
		case 2:
			std::get<2>(shape).moveBy(vec - center());
			break;
		case 3:
			std::get<3>(shape).setCentroid(vec);
			break;
		case 4:
			std::get<4>(shape).setCenter(vec);
			break;
		case 5:
			std::get<5>(shape).moveBy(vec - center());
		}
		return *this;
	}

	constexpr Figure& setCenter(double x, double y)noexcept
	{
		return setCenter(Vec2{ x,y });
	}

	[[nodiscard]]
	String getName()const noexcept
	{
		switch (shape.index())
		{
		case 0:return U"Circle";
		case 1:return U"RectF";
		case 2:return U"Quad";
		case 3:return U"Triangle";
		case 4:return U"Ellipse";
		case 5:return U"Polygon";
		}
	}

	[[nodiscard]]
	constexpr bool isCircle()const noexcept
	{
		return std::holds_alternative<Circle>(shape);
	}

	[[nodiscard]]
	constexpr bool isRectF()const noexcept
	{
		return std::holds_alternative<RectF>(shape);
	}

	[[nodiscard]]
	constexpr bool isQuad()const noexcept
	{
		return std::holds_alternative<Quad>(shape);
	}

	[[nodiscard]]
	constexpr bool isTriangle()const noexcept
	{
		return std::holds_alternative<Triangle>(shape);
	}

	[[nodiscard]]
	constexpr bool isEllipse()const noexcept
	{
		return std::holds_alternative<Ellipse>(shape);
	}

	[[nodiscard]]
	constexpr bool isPolygon()const noexcept
	{
		return std::holds_alternative<Polygon>(shape);
	}

	//変更
	[[nodiscard]]
	constexpr size_t getIndex()const noexcept
	{
		return shape.index();
	}


	constexpr RectF boundingRect()const noexcept {

		switch (shape.index())
		{
		case 0:return std::get<0>(shape).boundingRect();
		case 1:return std::get<1>(shape);
		case 2:return std::get<2>(shape).boundingRect();
		case 3:return std::get<3>(shape).boundingRect();
		case 4:return std::get<4>(shape).boundingRect();
		case 5:return std::get<5>(shape).boundingRect();
		}
	}

	double area()const noexcept {
		return std::visit([](auto& x) {return x.area(); }, shape);
	}

};
