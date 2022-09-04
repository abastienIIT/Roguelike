#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = y = 0;
}

Vector2D::Vector2D(int x, int y)
{
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D& Vector2D::sub(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D& Vector2D::mul(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D& Vector2D::div(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.add(v2);
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.sub(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.mul(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.div(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->sub(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->mul(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->div(vec);
}

Vector2D& Vector2D::operator*(const int& i)
{
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2D& Vector2D::operator/(const int& i)
{
	this->x /= i;
	this->y /= i;

	return *this;
}

Vector2D& Vector2D::zero()
{
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}