#include "DoubleVector.h"

DoubleVector::DoubleVector()
{
	x = y = 0;
}

DoubleVector::DoubleVector(double x, double y)
{
	this->x = x;
	this->y = y;
}

DoubleVector& DoubleVector::add(const DoubleVector& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

DoubleVector& DoubleVector::sub(const DoubleVector& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

DoubleVector& DoubleVector::mul(const DoubleVector& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

DoubleVector& DoubleVector::div(const DoubleVector& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

DoubleVector& operator+(DoubleVector& v1, const DoubleVector& v2)
{
	return v1.add(v2);
}

DoubleVector& operator-(DoubleVector& v1, const DoubleVector& v2)
{
	return v1.sub(v2);
}

DoubleVector& operator*(DoubleVector& v1, const DoubleVector& v2)
{
	return v1.mul(v2);
}

DoubleVector& operator/(DoubleVector& v1, const DoubleVector& v2)
{
	return v1.div(v2);
}

DoubleVector& DoubleVector::operator+=(const DoubleVector& vec)
{
	return this->add(vec);
}

DoubleVector& DoubleVector::operator-=(const DoubleVector& vec)
{
	return this->sub(vec);
}

DoubleVector& DoubleVector::operator*=(const DoubleVector& vec)
{
	return this->mul(vec);
}

DoubleVector& DoubleVector::operator/=(const DoubleVector& vec)
{
	return this->div(vec);
}

DoubleVector& DoubleVector::operator*(const double& i)
{
	this->x *= i;
	this->y *= i;

	return *this;
}

DoubleVector& DoubleVector::zero()
{
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const DoubleVector& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}