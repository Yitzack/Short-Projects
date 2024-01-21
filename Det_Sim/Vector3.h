#include<array>
using namespace std;

#ifndef VECTOR3_H
#define VECTOR3_H

class vector3
{
	public:
		vector3()
		{
			data[0] = data[1] = data[2] = 0;
		}
		vector3(const float A, const float B, const float C)
		{
			data[0] = A;
			data[1] = B;
			data[2] = C;
		}
		vector3(const float A[3])
		{
			data[0] = A[0];
			data[1] = A[1];
			data[2] = A[2];
		}
		vector3(const vector3& A)
		{
			data[0] = A.data[0];
			data[1] = A.data[1];
			data[2] = A.data[2];
		}
		vector3 operator=(const vector3);
		vector3 operator+(const vector3) const;
		vector3 operator+=(const vector3);
		vector3 operator-(const vector3) const;
		vector3 operator-() const;
		vector3 operator-=(const vector3);
		vector3 operator*(const float) const;
		vector3 operator*=(const float);
		vector3 operator/(const float) const;
		vector3 operator/=(const float);
		float operator[](const int) const;
		float& operator[](const int);
		float dot_product(const vector3) const;
		float scalar_product(const vector3) const;
		vector3 vector_product(const vector3) const;
		vector3 cross_product(const vector3) const;
		float length() const;
		vector3 normalize() const;
		vector3 abs() const;	//takes the abs of each element
	private:
		array<float,3> data;
};

inline vector3 operator*(float A, const vector3& B)
{
	return(B*A);
}

#endif
