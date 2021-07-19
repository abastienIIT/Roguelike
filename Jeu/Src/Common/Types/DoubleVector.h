#pragma once

#include <iostream>

class DoubleVector
{
public:
	double x;
	double y;

	DoubleVector();
	DoubleVector(double x, double y);

	DoubleVector& add(const DoubleVector& vec);
	DoubleVector& sub(const DoubleVector& vec);
	DoubleVector& mul(const DoubleVector& vec);
	DoubleVector& div(const DoubleVector& vec);

	friend DoubleVector& operator+(DoubleVector& v1, const DoubleVector& v2);
	friend DoubleVector& operator-(DoubleVector& v1, const DoubleVector& v2);
	friend DoubleVector& operator*(DoubleVector& v1, const DoubleVector& v2);
	friend DoubleVector& operator/(DoubleVector& v1, const DoubleVector& v2);

	DoubleVector& operator+=(const DoubleVector& vec);
	DoubleVector& operator-=(const DoubleVector& vec);
	DoubleVector& operator*=(const DoubleVector& vec);
	DoubleVector& operator/=(const DoubleVector& vec);

	DoubleVector& operator*(const double& i);
	DoubleVector& zero();

	friend std::ostream& operator<<(std::ostream& stream, const DoubleVector& vec);
};

