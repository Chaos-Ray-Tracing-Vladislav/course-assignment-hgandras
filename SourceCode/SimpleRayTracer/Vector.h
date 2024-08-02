#pragma once
#include <vector>
#include <iostream>
#include <format>

//TODO: REFACTOR
//TODO: Write xz,xy,xyz,yz ... getters to vector 3 and 4
struct Vector2 {
	float x, y;

	Vector2();

	Vector2(float x, float y);

	//Scalar operations
	Vector2 operator*(float a) const;
	Vector2 operator/(float a) const;

	//Elementwise operations
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator/(const Vector2& v) const;
	Vector2 operator-() const;

	//Properties
	float length();
	Vector2 norm();

	static Vector2 zero();
};

Vector2 operator*(float a, Vector2& vec);
float Dot(const Vector2& a, const Vector2& b);
Vector2 Reflect(Vector2& incoming, Vector2& n);

struct Vector3 {

	float x, y,z;

	Vector3();

	Vector3(float x, float y, float z);

	Vector3(Vector2 vec);

	//Scalar operations
	Vector3 operator*(float a) const;
	Vector3 operator/(float a) const;

	//Elementwise operations
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator/(const Vector3& v) const;
	Vector3 operator-() const;

	//Properties
	float length();
	Vector3 norm();

	static Vector3 zero();
	static Vector3 up();

};

std::ostream& operator<<(std::ostream& os, const Vector3& vec);

Vector3 operator*(const float a, const Vector3& vec);
float Dot(const Vector3& a, const Vector3& b);
Vector3 Cross(const Vector3& a, const Vector3& b);
Vector3 Reflect(const Vector3& incoming, const Vector3& n);
Vector3 Min(const Vector3& x, const Vector3& y);

struct Vector4 {

	float x, y, z, w;

	Vector4();

	

	Vector4(float x, float y, float z, float w);

	Vector4(Vector3 vec);

	//Scalar operations
	Vector4 operator*(float a) const;
	Vector4 operator/(float a) const;

	//Elementwise operations
	Vector4 operator+(const Vector4& v) const;
	Vector4 operator-(const Vector4& v) const;
	Vector4 operator*(const Vector4& v) const;
	Vector4 operator/(const Vector4& v) const;
	Vector4 operator-() const;

	//Properties
	float length();
	Vector4 norm();

	static Vector4 zero();
	
	Vector3 xyz();
};


inline Vector4 operator*(float a, Vector4& vec);
inline float Dot(const Vector4& a, const Vector4& b);

//Matrices to respresent transformations
//TODO: Write set row/col functions
class Matrix3 {
private:
	std::vector<float> data; //Row major storage
public:
	
	Matrix3();

	Matrix3(std::vector<float>& data);

	Matrix3(const Vector3& c1, const Vector3& c2, const Vector3& c3);

	Vector3 row1();
	Vector3 row2();
	Vector3 row3();

	Vector3 col1();
	Vector3 col2();
	Vector3 col3();

	Matrix3 T();

	//Rotation in radians
	static Matrix3 CreateRotation(float x, float y, float z);

	static Matrix3 Identity();

	//Matrix multiplication
	Matrix3 operator*(Matrix3& other);

	//Scalar multiplication
	Matrix3 operator*(float a);
};

class Matrix4 {
private:
	std::vector<float> data; //Row major storage
public:

	Matrix4();

	Matrix4(std::vector<float>& data);

	//Constructor takes columns 
	Matrix4(const Vector4& c1, const Vector4& c2, const Vector4& c3, const Vector4& c4);

	Matrix4(Matrix3& mat);

	//Useful when wanting to define translation
	Matrix4(Matrix3 mat,Vector3 vec);

	Vector4 row1();
	Vector4 row2();
	Vector4 row3();
	Vector4 row4();

	Vector4 col1();
	Vector4 col2();
	Vector4 col3();
	Vector4 col4();

	static Matrix4 CreateTranslation(Vector3 delta);

	static Matrix4 CreateRotation(float x, float y, float z);

	static Matrix4 Identity();

	Matrix4 T();

	//Matrix multiplication
	Matrix4 operator*(Matrix4& other);
};

Vector4 operator*(Matrix4& mat, Vector4& vec);

