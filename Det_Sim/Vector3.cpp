#include<array>
#include<cmath>
#include"Vector3.h"
using namespace std;

vector3 vector3::operator=(const vector3 A)
{
	data[0] = A.data[0];
	data[1] = A.data[1];
	data[2] = A.data[2];
	return(*this);
}

vector3 vector3::operator+(const vector3 A) const
{
	vector3 B;
	B.data[0] = data[0] + A.data[0];
	B.data[1] = data[1] + A.data[1];
	B.data[2] = data[2] + A.data[2];
	return(B);
}

vector3 vector3::operator+=(const vector3 A)
{
	data[0] += A.data[0];
	data[1] += A.data[1];
	data[2] += A.data[2];
	return(*this);
}

vector3 vector3::operator-(const vector3 A) const
{
	vector3 B;
	B.data[0] = data[0] - A.data[0];
	B.data[1] = data[1] - A.data[1];
	B.data[2] = data[2] - A.data[2];
	return(B);
}

vector3 vector3::operator-() const
{
	vector3 B;
	B.data[0] = -data[0];
	B.data[1] = -data[1];
	B.data[2] = -data[2];
	return(B);
}

vector3 vector3::operator-=(const vector3 A)
{
	data[0] -= A.data[0];
	data[1] -= A.data[1];
	data[2] -= A.data[2];
	return(*this);
}

vector3 vector3::operator*(const float A) const
{
	vector3 B;
	B.data[0] = data[0] * A;
	B.data[1] = data[1] * A;
	B.data[2] = data[2] * A;
	return(B);
}

vector3 vector3::operator*=(const float A)
{
	data[0] *= A;
	data[1] *= A;
	data[2] *= A;
	return(*this);
}

vector3 vector3::operator/(const float A) const
{
	vector3 B;
	B.data[0] = data[0] / A;
	B.data[1] = data[1] / A;
	B.data[2] = data[2] / A;
	return(B);
}

vector3 vector3::operator/=(const float A)
{
	data[0] /= A;
	data[1] /= A;
	data[2] /= A;
	return(*this);
}

float vector3::operator[](const int index) const
{
	return(data[index]);
}

float& vector3::operator[](const int index)
{
	return(data[index]);
}

float vector3::dot_product(const vector3 A) const
{
	float ans = data[0] * A.data[0];
	ans += data[1] * A.data[1];
	ans += data[2] * A.data[2];
	return(ans);
}

float vector3::scalar_product(const vector3 A) const
{
	return(dot_product(A));
}

vector3 vector3::vector_product(const vector3 A) const
{
	vector3 B;
	B.data[0] = data[1]*A.data[2] - data[2]*A.data[1];
	B.data[1] = data[2]*A.data[0] - data[0]*A.data[2];
	B.data[2] = data[0]*A.data[1] - data[1]*A.data[0];
	return(B);
}

vector3 vector3::cross_product(const vector3 A) const
{
	return(vector_product(A));
}

float vector3::length() const
{
	float ans = pow(data[0],2);
	ans += pow(data[1],2);
	ans += pow(data[2],2);
	return(sqrt(ans));
}

vector3 vector3::normalize() const
{
	return(*this/this->length());
}

vector3 vector3::abs() const
{
	vector3 ans;
	ans.data[0] = std::abs(data[0]);
	ans.data[1] = std::abs(data[1]);
	ans.data[2] = std::abs(data[2]);
	return(ans);
}

