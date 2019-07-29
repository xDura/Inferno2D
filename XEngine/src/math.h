#pragma once
#include <iostream>
#include <sstream> 
#include <algorithm>
#include <cmath>

#define M_PI 3.14159265358979323846

#define DEGTORAD(a) (((a) * 180.0) / M_PI)
#define RADTODEG(a) (((a) * M_PI) / 180.0)

static float map(float value, float fromMin, float fromMax, float toMin, float toMax)
{
	float OldRange = (fromMax - fromMin);
	if (OldRange == 0.0f)
		return toMin;
	else
	{
		float NewRange = (toMax - toMin);
		return (((value - fromMin) * NewRange) / OldRange) + toMin;
	}
}

static float clamp(float value, float min, float max)
{
	return ((std::min)(((std::max)(value, min)), max));
}

static int clamp(int value, int min, int max)
{
	return ((std::min)(((std::max)(value, min)), max));
}

class Vector2 
{
public:
	const static Vector2 zero;
	const static Vector2 one;

	union 
	{
		struct{float x, y;};
		float values[2];
	};

	Vector2() : x(0), y(0) {};
	Vector2(float a_x, float a_y) : x(a_x), y(a_y) {};
	Vector2(float values[2]);

	bool equals(const Vector2& other);
	bool equals_offset(const Vector2& other, float offset);

	float mod() const;
	void normalize();
	Vector2 normalized() const;
	float dot(const Vector2& other);
	void set(float a_x, float a_y) { x = a_x; y = a_y; };
	Vector2 operator + (const Vector2 & op) const;
	Vector2 operator - (const Vector2 & op) const;
	float distance(const Vector2 & other) const;
	static Vector2 random(float range);
	static float distance(const Vector2& v1, const Vector2& v2)  { return v1.distance(v2); }
	//@TODO:static float angle(const Vector2& v1, const Vector2& v2);

	std::string toString() const;
	void operator *= (float op);
	void operator /= (float op);
	Vector2 operator * (float op) const;
	Vector2 operator / (float op) const;

	Vector2 operator-()
	{
		return (*this) * -1.0f;
	}
};

Vector2 operator * (float f, const Vector2& v);
Vector2 operator / (float f, const Vector2& v);

class Vector3
{
public: 
	const static Vector3 zero;
	const static Vector3 one;

	union 
	{
		struct { float x, y, z; };
		float values[3];
	};

	Vector3() : x(0), y(0), z(0) {};
	Vector3(float a_x, float a_y, float a_z) : x(a_x), y(a_y), z(a_z) {};
	Vector3(float values[3]);

	bool equals(const Vector3& other);
	bool equals_offset(const Vector3& other, float offset);

	float mod() const;
	void normalize();
	Vector3 normalized() const;
	float dot(const Vector3& other) const;
	Vector3 cross(const Vector3& other) const;
	void set(float a_x, float a_y, float a_z) { x = a_x; y = a_y; z = a_z; };
	Vector3 operator + (const Vector3 & op) const;
	Vector3 operator - (const Vector3 & op) const;
	float distance(const Vector3 & other) const;
	static Vector3 random(float range);
	static Vector3 lerp(const Vector3& b, const Vector3& a, float t) { return a * t + b * (1.f - t); }
	static Vector3 slerp(const Vector3& start, const Vector3& end, float percent);

	std::string toString() const;
	void operator *= (float op);
	void operator /= (float op);
	Vector3 operator * (float op) const;
	Vector3 operator / (float op) const;

	Vector3 operator-()
	{
		return (*this) * -1.0f;
	}
};

bool operator == (const Vector3& a, Vector3& b);
Vector3 operator * (float a, Vector3 b);

class Vector4
{
public:
	union
	{
		struct { float x, y, z, w; };
		float values[4];
	};

	Vector4()
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}

	Vector4(float a_x, float a_y, float a_z, float a_w)
		:x(a_x), y(a_y), z(a_z), w(a_w)
	{}
};

class IVector4
{
public:
	union
	{
		struct { int i, j, k, w; };
		int32_t values[4];
	};

	IVector4()
		:i(0), j(0), k(0), w(0) 
	{}

	IVector4(int a_i, int a_j, int a_k, int a_w)
		:i(a_i), j(a_j), k(a_k), w(a_w) 
	{}
};

//row major 3x3 matrix
class Mat33
{
public:
	const static Mat33 identity;

	union
	{
		float m[3][3];
		float M[9];
	};

	Mat33() { setIdentity(); }
	//Mat33(float f[9]){ values = f;}
	void setIdentity();
	void setTranslation(const Vector2& translation);
	void setRotation(float rotX, float rotY, float rotZ);
	void setRotation(const Vector3& axis, float angle);
	void setScale(const Vector2& scale);
	void transpose();
	void inverse();
	void orthonormalize();
	void translate(const Vector2& translation);
	void rotate(float rot);
	void scale(const Vector2& scale);

	Vector2 up() const;
	Vector2 right()const;
	Vector2 front()const;

	void lookAt(Vector2& v);

	Mat33 operator * (float val);
	Mat33 operator * (const Mat33& other);
};

//row major 4x4 matrix
class Mat44
{
public:
	const static Mat44 identity;

	union
	{
		float m[4][4];
		float values[16];
	};

	Mat44() { setIdentity(); }
	Mat44(const Mat44& other);

	void setIdentity();
	void setTranslation(const Vector3& translation);
	void setTranslation(float tx, float ty, float tz);
	Vector3 getPosition() const;
	Vector3 getScale() const;
	std::string toString() const;
	void setRotation(float angleX, float angleY, float angleZ);
	void setRotation(float angle, const Vector3& axis);
	void setScale(const Vector3& scale);
	void transpose();
	bool inverse();
	void orthonormalize();
	void translate(const Vector3& translation);
	void translate(float tx, float ty, float tz);
	void translateLocal(const Vector3& translation);
	void translateLocal(float tx, float ty, float tz);
	void rotateLocal(float rotX, float rotY, float rotZ);
	void rotate(float rotX, float rotY, float rotZ);
	void scale(const Vector3& scale);
	void replaceScale(const Vector3& scale);
	void setLookAt(const Vector3& position, const Vector3& targetPoint, const Vector3& up);
	void setPerspective(float fov, float aspect, float near, float far);
	void setOrthographic(float left, float right, float bottom, float top, float near, float far);

	Vector3 front() const { return Vector3(values[8], values[9], values[10]); }
	Vector3 up() const { return Vector3(values[4], values[5], values[6]); }
	Vector3 right() const { return Vector3(values[0], values[1], values[2]); }

	Mat44 getRotationOnly();
	Vector3 rotateVector(const Vector3& v);

	Mat44 operator * (float val);
	Mat44 operator * (const Mat44& other);
	//@TODO: Check if there should be Vector4 for this is a bit confusing
	Vector3 operator * (const Vector3& other);
};

//This quaternions are taken from Javi Agenjo's code
//https://www.dtic.upf.edu/~jagenjo/?page_id=11
class Quaternion
{
public:

	union
	{
		struct { float x; float y; float z; float w; };
		float q[4];
	};

public:
	Quaternion();
	Quaternion(const float* q);
	Quaternion(const Quaternion& q);
	Quaternion(const float X, const float Y, const float Z, const float W);
	Quaternion(const Vector3& axis, float angle);

	void identity();
	Quaternion invert() const;
	Quaternion conjugate() const;

	void set(const float X, const float Y, const float Z, const float W);
	void slerp(const Quaternion& b, float t);
	void slerp(const Quaternion& q2, float t, Quaternion &q3) const;

	void lerp(const Quaternion& b, float t);
	void lerp(const Quaternion& q2, float t, Quaternion &q3) const;

public:
	void setAxisAngle(const Vector3& axis, const float angle);
	void setAxisAngle(float x, float y, float z, float angle);
	void getAxisAngle(Vector3 &v, float &angle) const;

	Vector3 rotate(const Vector3& v) const;

	void operator *= (const Vector3& v);
	void operator *= (const Quaternion &q);
	void operator += (const Quaternion &q);

	friend Quaternion operator + (const Quaternion &q1, const Quaternion& q2);
	friend Quaternion operator * (const Quaternion &q1, const Quaternion& q2);

	friend Quaternion operator * (const Quaternion &q, const Vector3& v);

	friend Quaternion operator * (float f, const Quaternion &q);
	friend Quaternion operator * (const Quaternion &q, float f);

	Quaternion& operator -();

	friend bool operator==(const Quaternion& q1, const Quaternion& q2);
	friend bool operator!=(const Quaternion& q1, const Quaternion& q2);

	void operator *= (float f);

	void computeMinimumRotation(const Vector3& rotateFrom, const Vector3& rotateTo);

	void normalize();
	float squaredLength() const;
	float length() const;
	void toMatrix(Mat44 &) const;
	Mat44 toMatrix() const;
	void fromMatrix(const Mat44& mat);

	void toEulerAngles(Vector3 &euler) const;
	static Quaternion Euler(const Vector3& eulerAngles);
	float& operator[] (unsigned int i) { return q[i]; }
};


float DotProduct(const Quaternion &q1, const Quaternion &q2);
Quaternion Qlerp(const Quaternion &q1, const Quaternion &q2, float t);
Quaternion Qslerp(const Quaternion &q1, const Quaternion &q2, float t);
Quaternion Qsquad(const Quaternion &q1, const Quaternion &q2, const Quaternion &a, const Quaternion &b, float t);
Quaternion Qsquad(const Quaternion &q1, const Quaternion &q2, const Quaternion &a, float t);
Quaternion Qspline(const Quaternion &q1, const Quaternion &q2, const Quaternion &q3);
Quaternion QslerpNoInvert(const Quaternion &q1, const Quaternion &q2, float t);
Quaternion Qexp(const Quaternion &q);
Quaternion Qlog(const Quaternion &q);
Quaternion SimpleRotation(const Vector3 &a, const Vector3 &b);