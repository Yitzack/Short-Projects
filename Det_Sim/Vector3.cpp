#include<array>
#include<cmath>
#include"Vector3.h"
using namespace std;

inline vector3 vector3::operator=(const vector3 A)
{
	data[0] = A.data[0];
	data[1] = A.data[1];
	data[2] = A.data[2];
	return(*this);
}

inline vector3 vector3::operator+(const vector3 A) const
{
	vector3 B;
	B.data[0] = data[0] + A.data[0];
	B.data[1] = data[1] + A.data[1];
	B.data[2] = data[2] + A.data[2];
	return(B);
}

inline vector3 vector3::operator+=(const vector3 A)
{
	data[0] += A.data[0];
	data[1] += A.data[1];
	data[2] += A.data[2];
	return(*this);
}

inline vector3 vector3::operator-(const vector3 A) const
{
	vector3 B;
	B.data[0] = data[0] - A.data[0];
	B.data[1] = data[1] - A.data[1];
	B.data[2] = data[2] - A.data[2];
	return(B);
}

inline vector3 vector3::operator-=(const vector3 A)
{
	data[0] -= A.data[0];
	data[1] -= A.data[1];
	data[2] -= A.data[2];
	return(*this);
}

inline vector3 vector3::operator*(const float A) const
{
	vector3 B;
	B.data[0] = data[0] * A;
	B.data[1] = data[1] * A;
	B.data[2] = data[2] * A;
	return(B);
}

inline vector3 operator*(float A, const vector3& B)
{
	return(B*A);
}

inline vector3 vector3::operator*=(const float A)
{
	data[0] *= A;
	data[1] *= A;
	data[2] *= A;
	return(*this);
}

inline vector3 vector3::operator/(const float A) const
{
	vector3 B;
	B.data[0] = data[0] / A;
	B.data[1] = data[1] / A;
	B.data[2] = data[2] / A;
	return(B);
}

inline vector3 vector3::operator/=(const float A)
{
	data[0] /= A;
	data[1] /= A;
	data[2] /= A;
	return(*this);
}

inline float vector3::operator[](const int index) const
{
	return(data[index]);
}

inline float& vector3::operator[](const int index)
{
	return(data[index]);
}

inline float vector3::dot_product(const vector3 A) const
{
	float ans = data[0] * A.data[0];
	ans += data[1] * A.data[1];
	ans += data[2] * A.data[2];
	return(ans);
}

inline float vector3::scalar_product(const vector3 A) const
{
	return(dot_product(A));
}

inline vector3 vector3::vector_product(const vector3 A) const
{
	vector3 B;
	B.data[0] = data[1]*A.data[2] - data[2]*A.data[1];
	B.data[1] = data[2]*A.data[0] - data[0]*A.data[2];
	B.data[2] = data[0]*A.data[1] - data[1]*A.data[0];
	return(B);
}

inline vector3 vector3::cross_product(const vector3 A) const
{
	return(vector_product(A));
}

inline float vector3::length() const
{
	float ans = pow(data[0],2);
	ans += pow(data[1],2);
	ans += pow(data[2],2);
	return(sqrt(ans));
}

inline vector3 vector3::normalize() const
{
	return(*this/this->length());
}

