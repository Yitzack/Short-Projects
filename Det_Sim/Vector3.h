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
		inline vector3 operator=(const vector3);
		inline vector3 operator+(const vector3) const;
		inline vector3 operator+=(const vector3);
		inline vector3 operator-(const vector3) const;
		inline vector3 operator-=(const vector3);
		inline vector3 operator*(const float) const;
		inline vector3 operator*=(const float);
		inline vector3 operator/(const float) const;
		inline vector3 operator/=(const float);
		inline float operator[](const int) const;
		inline float& operator[](const int);
		inline float dot_product(const vector3) const;
		inline float scalar_product(const vector3) const;
		inline vector3 vector_product(const vector3) const;
		inline vector3 cross_product(const vector3) const;
		inline float length() const;
		inline vector3 normalize() const;
	private:
		array<float,3> data;
};

#endif
