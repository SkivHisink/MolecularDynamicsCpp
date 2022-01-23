#include "Vector3.h"

double Vector3::get_X()
{
	return x;
}

double Vector3::get_const_X() const
{
	return x;
}

double Vector3::set_X(double& val)
{
	x = val;
}

double Vector3::get_Y()
{
	return y;
}

double Vector3::get_const_Y() const
{
	return y;
}

double Vector3::set_Y(double& val)
{
	y = val;
}

double Vector3::get_Z()
{
	return z;
}

double Vector3::get_const_Z() const
{
	return z;
}

double Vector3::set_Z(double& val)
{
	z = val;
}

Vector3::Vector3(double x, double y, double z) :x(x), y(y), z(z) {}

Vector3::Vector3() : x(0.), y(0.), z(0.) {}

double Vector3::magnitude() const
{
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
	return { this->x + vec.x, this->y + vec.y, this->z + vec.z };
}

void Vector3::operator+=(const Vector3& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
}

Vector3 Vector3::operator-(const Vector3& vec)const
{
	return { this->x - vec.x, this->y - vec.y, this->z - vec.z };
}

Vector3 Vector3::operator*(const double& val) const
{
	return { this->x * val, this->y * val, this->z * val };
}

Vector3 Vector3::normalized() const
{
	double norm_val = this->magnitude();
	norm_val = 1. / norm_val;
	return Vector3(this->x * norm_val, this->y * norm_val, this->z * norm_val);
}