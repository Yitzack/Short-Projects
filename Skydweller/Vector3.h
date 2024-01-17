#include<array>
#include<iostream>
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
		vector3(const double A, const double B, const double C)
		{
			data[0] = A;
			data[1] = B;
			data[2] = C;
		}
		vector3(const double A[3])
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
		vector3 operator*(const double) const;
		vector3 operator*=(const double);
		vector3 operator/(const double) const;
		vector3 operator/=(const double);
		double operator[](const int) const;
		double& operator[](const int);
		double dot_product(const vector3) const;
		double scalar_product(const vector3) const;
		vector3 vector_product(const vector3) const;
		vector3 cross_product(const vector3) const;
		double length() const;
		vector3 normalize() const;
		vector3 abs() const;	//takes the abs of each element
		ostream& print(ostream&) const;
		friend ostream& operator<<(ostream&, const vector3);
	private:
		array<double,3> data;
};

#endif
