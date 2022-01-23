#pragma once
#include <fstream>


class Vector3 final
{
private:
	double x;
	double y;
	double z;
public:
	double get_X();
	double get_const_X() const;
	double set_X(double& val);
	double get_Y();
	double get_const_Y() const;
	double set_Y(double& val);
	double get_Z();
	double get_const_Z() const;
	double set_Z(double& val);
	Vector3(double x, double y, double z) :x(x), y(y), z(z) {}
	Vector3() :x(0.), y(0.), z(0.) {}
	double magnitude() const;
	Vector3 operator+(const Vector3& vec) const;
	void operator+=(const Vector3& vec);
	Vector3 operator-(const Vector3& vec)const;
	Vector3 operator*(const double& val) const;
	Vector3 normalized() const;
	static Vector3 right()
	{
		return Vector3(1., 0., 0.);
	}
	static Vector3 left()
	{
		return Vector3(-1., 0., 0.);
	}
	static Vector3 up()
	{
		return Vector3(0., 1., 0.);
	}
	static Vector3 down()
	{
		return Vector3(0., -1., 0.);
	}
	static Vector3 forward()
	{
		return Vector3(0., 0., 1.);
	}
	static Vector3 back()
	{
		return Vector3(0., 0., -1.);
	}
	static Vector3 zero()
	{
		return Vector3(0., 0., 0.);
	}

};

std::ostream& operator<< (std::ostream& out, const Vector3& point)
{
	out << point.get_const_X() << " " << point.get_const_Y() << " " << point.get_const_Z();
	return out;
}