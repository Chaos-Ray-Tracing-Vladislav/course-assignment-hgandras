#pragma once
#include <vector>

//TODO: Write xz,xy,xyz,yz ... getters to vector 3 and 4
struct Vector2 {
	float x, y;

	Vector2() : x(0), y(0)
	{

	}

	Vector2(float x, float y) : x(x), y(y)
	{

	}

	//Scalar operations
	Vector2 operator*(float a) const { return Vector2(a * x, a * y); }
	Vector2 operator/(float a) const { return Vector2(x / a, y / a); }

	//Elementwise operations
	Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2 operator*(const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	Vector2 operator/(const Vector2& v) const { return Vector2(x / v.x, y / v.y); }
	Vector2 operator-() const { return Vector2(-x, -y); }

	//Properties
	float length() { return sqrtf(x*x+y*y); }
	Vector2 norm() { return Vector2(x / length(), y / length()); }

	static Vector2 zero() { return Vector2(0, 0); }
};

inline Vector2 operator*(float a, Vector2& vec) { return Vector2(a * vec.x, a * vec.y); }
inline float Dot(const Vector2& a,const Vector2& b) { return a.x * b.x + a.y * b.y; }
inline Vector2 Reflect(Vector2& incoming, Vector2& n) { return incoming - 2 * Dot(incoming, n) * n; }

struct Vector3 {

	float x, y,z;

	Vector3() : x(0), y(0),z(0)
	{

	}

	Vector3(float x, float y,float z) : x(x), y(y),z(z)
	{

	}

	Vector3(Vector2 vec) : x(vec.x), y(vec.y), z(0)
	{

	}

	//Scalar operations
	Vector3 operator*(float a) const { return Vector3(a * x, a * y,a*z); }
	Vector3 operator/(float a) const { return Vector3(x / a, y / a,z/a); }

	//Elementwise operations
	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y,z+v.z); }
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y,z - v.z); }
	Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y,z * v.z); }
	Vector3 operator/(const Vector3& v) const { return Vector3(x / v.x, y / v.y,z/v.z); }
	Vector3 operator-() const { return Vector3(-x, -y,-z); }

	//Properties
	float length() { return sqrtf(x * x + y * y+z*z); }
	Vector3 norm() { return Vector3(x / length(), y / length(),z/length()); }

	static Vector3 zero() { return Vector3(0, 0, 0); }
	static Vector3 up() { return Vector3(0, 1, 0); }

};

std::ostream& operator<<(std::ostream& os,const Vector3& vec)
{
	std::cout << std::format("Vector3({},{},{})", vec.x, vec.y, vec.z);
	return os;
}

inline Vector3 operator*(const float a,const Vector3& vec) { return Vector3(a * vec.x, a * vec.y,a*vec.z); }
inline float Dot(const Vector3& a,const Vector3& b) { return a.x * b.x + a.y * b.y+a.z*b.z; }
inline Vector3 Cross(const Vector3& a,const Vector3& b) {
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
inline Vector3 Reflect(Vector3& incoming, Vector3& n) { return incoming - 2 * Dot(incoming, n) * n; }

struct Vector4 {

	float x, y, z, w;

	Vector4() : x(0), y(0), z(0),w(0)
	{

	}

	Vector4(float x, float y, float z,float w) : x(x), y(y), z(z),w(w)
	{

	}

	Vector4(Vector3 vec) : x(vec.x), y(vec.y), z(vec.z), w(1)
	{

	}

	//Scalar operations
	Vector4 operator*(float a) const { return Vector4(a * x, a * y, a * z,a*w); }
	Vector4 operator/(float a) const { return Vector4(x / a, y / a, z / a,a/w); }

	//Elementwise operations
	Vector4 operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z,w+v.w); }
	Vector4 operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z,w-v.w); }
	Vector4 operator*(const Vector4& v) const { return Vector4(x * v.x, y * v.y, z * v.z,w-v.w); }
	Vector4 operator/(const Vector4& v) const { return Vector4(x / v.x, y / v.y, z / v.z,w/v.w); }
	Vector4 operator-() const { return Vector4(-x, -y, -z,-w); }

	//Properties
	float length() { return sqrtf(x * x + y * y+z*z+w*w); }
	Vector4 norm() { return Vector4(x / length(), y / length(), z / length(),w/length()); }

	static Vector4 zero() { return Vector4(0, 0, 0,0); }
	
	Vector3 xyz() {
		return Vector3(x, y, z);
	}
};


inline Vector4 operator*(float a, Vector4& vec) { return Vector4(a * vec.x, a * vec.y, a * vec.z,a*vec.w); }
inline float Dot(const Vector4& a,const Vector4& b) { return a.x * b.x + a.y * b.y + a.z * b.z+a.w*b.w; }

//Matrices to respresent transformations
//TODO: Write set row/col functions
class Matrix3 {
private:
	std::vector<float> data; //Row major storage
public:
	
	Matrix3()
	{
		for (int i = 0; i < 9; i++)
			data.push_back(0);
	}

	Matrix3(const Vector3& c1,const Vector3& c2,const Vector3& c3)
	{
		//row1
		data.push_back(c1.x);
		data.push_back(c2.x);
		data.push_back(c3.x);
		//row2
		data.push_back(c1.y);
		data.push_back(c2.y);
		data.push_back(c3.y);
		//row3
		data.push_back(c1.z);
		data.push_back(c2.z);
		data.push_back(c3.z);

	}

	Vector3 row1() { return Vector3(data[0],data[1],data[2]); }
	Vector3 row2() { return Vector3(data[3], data[4], data[5]); }
	Vector3 row3() { return Vector3(data[6], data[7], data[8]); }

	Vector3 col1() { return Vector3(data[0], data[3], data[6]); }
	Vector3 col2() { return Vector3(data[1], data[4], data[7]); }
	Vector3 col3() { return Vector3(data[2], data[5], data[8]); }

	Matrix3 T() {		
		return Matrix3(row1(), row2(), row3());
	}

	//Rotation in radians
	static Matrix3 CreateRotation(float x,float y, float z)
	{
		//X rotation
		Matrix3 rotX(Vector3(1, 0, 0),
					Vector3(0, cosf(x), sinf(x)),
					Vector3(0, -sinf(x), cosf(x)));
		
		//Y rotation
		Matrix3 rotY(Vector3(cosf(y), 0, -sinf(y)), 
					Vector3(0, 1, 0), 
					Vector3(-sinf(y), 0, cosf(y)));

		//Z rotation
		Matrix3 rotZ(Vector3(cosf(z), sinf(z), 0),
					Vector3(-sinf(z), cos(z), 0), 
					Vector3(0, 0, 1));

		return rotZ * rotY * rotX;
	}

	static Matrix3 Identity()
	{
		return Matrix3(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1));
	}

	//Matrix multiplication
	Matrix3 operator*(Matrix3& other){

		Vector3 r1new(Dot(row1(), other.col1()), Dot(row1(), other.col2()), Dot(row1(),other.col3()));
		Vector3 r2new(Dot(row2(), other.col1()), Dot(row2(), other.col2()), Dot(row2(), other.col3()));
		Vector3 r3new(Dot(row3(), other.col1()), Dot(row3(), other.col2()), Dot(row3(), other.col3()));
		return Matrix3(r1new,r2new,r3new).T();
	}

	//Scalar multiplication
	Matrix3 operator*(float a)
	{
		return Matrix3(col1() * a, col2() * a,col3() * a);
	}
};

class Matrix4 {
private:
	std::vector<float> data; //Row major storage
public:

	Matrix4()
	{
		for (int i = 0; i < 16; i++)
			data.push_back(0);
	}

	//Constructor takes columns 
	Matrix4(const Vector4& c1,const Vector4& c2,const Vector4& c3,const Vector4& c4)
	{
		//row1
		data.push_back(c1.x);
		data.push_back(c2.x);
		data.push_back(c3.x);
		data.push_back(c4.x);
		//row2
		data.push_back(c1.y);
		data.push_back(c2.y);
		data.push_back(c3.y);
		data.push_back(c4.y);
		//row3
		data.push_back(c1.z);
		data.push_back(c2.z);
		data.push_back(c3.z);
		data.push_back(c4.z);
		//row4
		data.push_back(c1.w);
		data.push_back(c2.w);
		data.push_back(c3.w);
		data.push_back(c4.w);

	}

	Matrix4(Matrix3& mat)
	{
		Vector4 r1 = Vector4(mat.row1());
		Vector4 r2 = Vector4(mat.row2());
		Vector4 r3 = Vector4(mat.row3());
		Vector4 r4 = Vector4(0, 0, 0, 1);

		r1.w = 0;
		r2.w = 0;
		r3.w = 0;

		data.push_back(r1.x);
		data.push_back(r1.y);
		data.push_back(r1.z);
		data.push_back(r1.w);

		data.push_back(r2.x);
		data.push_back(r2.y);
		data.push_back(r2.z);
		data.push_back(r2.w);

		data.push_back(r3.x);
		data.push_back(r3.y);
		data.push_back(r3.z);
		data.push_back(r3.w);

		data.push_back(r4.x);
		data.push_back(r4.y);
		data.push_back(r4.z);
		data.push_back(r4.w);
	}

	//Useful when wanting to define translation
	Matrix4(Matrix3 mat, Vector3 vec) : Matrix4(mat)
	{
		data[3] = vec.x;
		data[7] = vec.y;
		data[11] = vec.z;
	}

	Vector4 row1() { return Vector4(data[0], data[1], data[2], data[3]); }
	Vector4 row2() { return Vector4(data[4], data[5], data[6],data[7]); }
	Vector4 row3() { return Vector4(data[8], data[9], data[10],data[11]); }
	Vector4 row4() { return Vector4(data[12], data[13], data[14], data[15]); }

	Vector4 col1() { return Vector4(data[0], data[4], data[8], data[12]); }
	Vector4 col2() { return Vector4(data[1], data[5], data[9], data[13]); }
	Vector4 col3() { return Vector4(data[2], data[6], data[10], data[14]); }
	Vector4 col4() { return Vector4(data[3], data[7], data[11], data[15]); }

	static Matrix4 CreateTranslation(Vector3 delta)
	{	
		Matrix3 id = Matrix3::Identity();
		return Matrix4(id,delta);
	}

	static Matrix4 CreateRotation(float x, float y, float z)
	{
		Matrix3 rot = Matrix3::CreateRotation(x, y, z);
		return Matrix4(rot);
	}

	static Matrix4 Identity()
	{
		return Matrix4(Vector4(1,0,0,0),
			Vector4(0,1,0,0),
			Vector4(0,0,1,0),
			Vector4(0,0,0,1));
	}

	Matrix4 T() {
		return Matrix4(row1(), row2(), row3(), row4());
	}

	//Matrix multiplication
	Matrix4 operator*(Matrix4& other) {

		Vector4 r1new(Dot(row1(), other.col1()), Dot(row1(), other.col2()), Dot(row1(), other.col3()),Dot(row1(),other.col4()));
		Vector4 r2new(Dot(row2(), other.col1()), Dot(row2(), other.col2()), Dot(row2(), other.col3()), Dot(row2(), other.col4()));
		Vector4 r3new(Dot(row3(), other.col1()), Dot(row3(), other.col2()), Dot(row3(), other.col3()), Dot(row3(), other.col4()));
		Vector4 r4new(Dot(row4(), other.col1()), Dot(row4(), other.col4()), Dot(row4(), other.col3()), Dot(row4(), other.col4()));

		return Matrix4(r1new, r2new, r3new,r4new).T();
	}
};

Vector4 operator*(Matrix4& mat, Vector4& vec) {
	return Vector4(Dot(mat.row1(), vec),
		Dot(mat.row2(),vec),
		Dot(mat.row3(),vec),
		Dot(mat.row4(), vec));
}

