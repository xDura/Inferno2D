#pragma once
#include <iostream>
#include <sstream> 
#include <algorithm>
#include <cmath>
#include "types.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEGTORAD(a) (((a) * 180.0) / M_PI)
#define RADTODEG(a) (((a) * M_PI) / 180.0)

static f32 map(f32 value, f32 fromMin, f32 fromMax, f32 toMin, f32 toMax)
{
	f32 OldRange = (fromMax - fromMin);
	if (OldRange == 0.0f)
		return toMin;
	else
	{
		f32 NewRange = (toMax - toMin);
		return (((value - fromMin) * NewRange) / OldRange) + toMin;
	}
}

static f32 clamp(f32 value, f32 min, f32 max)
{
	return ((std::min)(((std::max)(value, min)), max));
}

static int clamp(int value, int min, int max)
{
	return ((std::min)(((std::max)(value, min)), max));
}

class Vec2f 
{
public:
	const static Vec2f zero;
	const static Vec2f one;

	union 
	{
		struct{f32 x, y;};
		f32 values[2];
	};

	Vec2f() : x(0), y(0) {};
	Vec2f(f32 a_x, f32 a_y) : x(a_x), y(a_y) {};
	Vec2f(f32 values[2]);

	bool equals(const Vec2f& other);
	bool equals_offset(const Vec2f& other, f32 offset);

	f32 mod() const;
	void normalize();
	Vec2f normalized() const;
	f32 dot(const Vec2f& other);
	void set(f32 a_x, f32 a_y) { x = a_x; y = a_y; };
	Vec2f operator + (const Vec2f & op) const;
	Vec2f operator - (const Vec2f & op) const;
	f32 distance(const Vec2f & other) const;
	static Vec2f random(f32 range);
	static f32 distance(const Vec2f& v1, const Vec2f& v2)  { return v1.distance(v2); }
	//@TODO:static f32 angle(const Vector2& v1, const Vector2& v2);

	std::string toString() const;
	void operator *= (f32 op);
	void operator /= (f32 op);
	Vec2f operator * (f32 op) const;
	Vec2f operator / (f32 op) const;

	Vec2f operator-()
	{
		return (*this) * -1.0f;
	}
};

Vec2f operator * (f32 f, const Vec2f& v);
Vec2f operator / (f32 f, const Vec2f& v);

class Vec3f
{
public: 
	const static Vec3f zero;
	const static Vec3f one;

	union 
	{
		struct { f32 x, y, z; };
		f32 values[3];
	};

	Vec3f() : x(0), y(0), z(0) {};
	Vec3f(f32 a_x, f32 a_y, f32 a_z) : x(a_x), y(a_y), z(a_z) {};
	Vec3f(f32 values[3]);

	bool equals(const Vec3f& other);
	bool equals_offset(const Vec3f& other, f32 offset);

	f32 mod() const;
	void normalize();
	Vec3f normalized() const;
	f32 dot(const Vec3f& other) const;
	Vec3f cross(const Vec3f& other) const;
	void set(f32 a_x, f32 a_y, f32 a_z) { x = a_x; y = a_y; z = a_z; };
	Vec3f operator + (const Vec3f & op) const;
	Vec3f operator - (const Vec3f & op) const;
	f32 distance(const Vec3f & other) const;
	static Vec3f random(f32 range);
	static Vec3f lerp(const Vec3f& b, const Vec3f& a, f32 t) { return a * t + b * (1.f - t); }
	static Vec3f slerp(const Vec3f& start, const Vec3f& end, f32 percent);

	std::string toString() const;
	void operator *= (f32 op);
	void operator /= (f32 op);
	Vec3f operator * (f32 op) const;
	Vec3f operator / (f32 op) const;

	Vec3f operator-()
	{
		return (*this) * -1.0f;
	}
};

bool operator == (const Vec3f& a, Vec3f& b);
Vec3f operator * (f32 a, Vec3f b);

class Vec4f
{
public:
	union
	{
		struct { f32 x, y, z, w; };
		f32 values[4];
	};

	Vec4f()
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}

	Vec4f(f32 a_x, f32 a_y, f32 a_z, f32 a_w)
		:x(a_x), y(a_y), z(a_z), w(a_w)
	{}
};

class Vec4s
{
public:
	union
	{
		struct { s32 i, j, k, w; };
		s32 values[4];
	};

	Vec4s()
		:i(0), j(0), k(0), w(0) 
	{}

	Vec4s(s32 a_i, s32 a_j, s32 a_k, s32 a_w)
		:i(a_i), j(a_j), k(a_k), w(a_w) 
	{}
};

//row major 3x3 matrix
class Mat33f
{
public:
	const static Mat33f identity;

	union
	{
		f32 m[3][3];
		f32 M[9];
	};

	Mat33f() { setIdentity(); }
	//Mat33(f32 f[9]){ values = f;}
	void setIdentity();
	void setTranslation(const Vec2f& translation);
	void setRotation(f32 rotX, f32 rotY, f32 rotZ);
	//void setRotation(const Vector3& axis, f32 angle);
	void setScale(const Vec2f& scale);
	void transpose();
	void inverse();
	void orthonormalize();
	void translate(const Vec2f& translation);
	//void rotate(f32 rot);
	void scale(const Vec2f& scale);

	Vec2f up() const;
	Vec2f right()const;
	//Vector2 front()const;

	void lookAt(Vec2f& v);

	Mat33f operator * (f32 val);
	Mat33f operator * (const Mat33f& other);
};

//row major 4x4 matrix
class Mat44f
{
public:
	const static Mat44f identity;

	union
	{
		f32 m[4][4];
		f32 values[16];
	};

	Mat44f() { setIdentity(); }
	Mat44f(const Mat44f& other);

	void setIdentity();
	void setTranslation(const Vec3f& translation);
	void setTranslation(f32 tx, f32 ty, f32 tz);
	Vec3f getPosition() const;
	Vec3f getScale() const;
	std::string toString() const;
	void setRotation(f32 angleX, f32 angleY, f32 angleZ);
	void setRotation(f32 angle, const Vec3f& axis);
	void setScale(const Vec3f& scale);
	void transpose();
	bool inverse();
	//void orthonormalize();
	void translate(const Vec3f& translation);
	void translate(f32 tx, f32 ty, f32 tz);
	void translateLocal(const Vec3f& translation);
	void translateLocal(f32 tx, f32 ty, f32 tz);
	void rotateLocal(f32 rotX, f32 rotY, f32 rotZ);
	void rotate(f32 rotX, f32 rotY, f32 rotZ);
	void scale(const Vec3f& scale);
	void replaceScale(const Vec3f& scale);
	void setLookAt(const Vec3f& position, const Vec3f& targetPoint, const Vec3f& up);
	void setPerspective(f32 fov, f32 aspect, f32 near, f32 far);
	void setOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

	Vec3f front() const { return Vec3f(values[8], values[9], values[10]); }
	Vec3f up() const { return Vec3f(values[4], values[5], values[6]); }
	Vec3f right() const { return Vec3f(values[0], values[1], values[2]); }

	Mat44f getRotationOnly();
	Vec3f rotateVector(const Vec3f& v);

	Mat44f operator * (f32 val);
	Mat44f operator * (const Mat44f& other);
	//@TODO: Check if there should be Vector4 for this is a bit confusing
	Vec3f operator * (const Vec3f& other);
};

//This quaternions are taken from Javi Agenjo's code
//https://www.dtic.upf.edu/~jagenjo/?page_id=11
class Quaternion
{
public:

	union
	{
		struct { f32 x; f32 y; f32 z; f32 w; };
		f32 q[4];
	};

public:
	Quaternion();
	Quaternion(const f32* q);
	Quaternion(const Quaternion& q);
	Quaternion(const f32 X, const f32 Y, const f32 Z, const f32 W);
	Quaternion(const Vec3f& axis, f32 angle);

	void identity();
	Quaternion invert() const;
	Quaternion conjugate() const;

	void set(const f32 X, const f32 Y, const f32 Z, const f32 W);
	void slerp(const Quaternion& b, f32 t);
	void slerp(const Quaternion& q2, f32 t, Quaternion &q3) const;

	void lerp(const Quaternion& b, f32 t);
	void lerp(const Quaternion& q2, f32 t, Quaternion &q3) const;

public:
	void setAxisAngle(const Vec3f& axis, const f32 angle);
	void setAxisAngle(f32 x, f32 y, f32 z, f32 angle);
	void getAxisAngle(Vec3f &v, f32 &angle) const;

	Vec3f rotate(const Vec3f& v) const;

	void operator *= (const Vec3f& v);
	void operator *= (const Quaternion &q);
	void operator += (const Quaternion &q);

	friend Quaternion operator + (const Quaternion &q1, const Quaternion& q2);
	friend Quaternion operator * (const Quaternion &q1, const Quaternion& q2);

	friend Quaternion operator * (const Quaternion &q, const Vec3f& v);

	friend Quaternion operator * (f32 f, const Quaternion &q);
	friend Quaternion operator * (const Quaternion &q, f32 f);

	Quaternion& operator -();

	friend bool operator==(const Quaternion& q1, const Quaternion& q2);
	friend bool operator!=(const Quaternion& q1, const Quaternion& q2);

	void operator *= (f32 f);

	void computeMinimumRotation(const Vec3f& rotateFrom, const Vec3f& rotateTo);

	void normalize();
	f32 squaredLength() const;
	f32 length() const;
	void toMatrix(Mat44f &) const;
	Mat44f toMatrix() const;
	void fromMatrix(const Mat44f& mat);

	void toEulerAngles(Vec3f &euler) const;
	static Quaternion Euler(const Vec3f& eulerAngles);
	f32& operator[] (u32 i) { return q[i]; }
};


f32 DotProduct(const Quaternion &q1, const Quaternion &q2);
Quaternion Qlerp(const Quaternion &q1, const Quaternion &q2, f32 t);
Quaternion Qslerp(const Quaternion &q1, const Quaternion &q2, f32 t);
//Quaternion Qsquad(const Quaternion &q1, const Quaternion &q2, const Quaternion &a, const Quaternion &b, f32 t);
//Quaternion Qsquad(const Quaternion &q1, const Quaternion &q2, const Quaternion &a, f32 t);
//Quaternion Qspline(const Quaternion &q1, const Quaternion &q2, const Quaternion &q3);
//Quaternion QslerpNoInvert(const Quaternion &q1, const Quaternion &q2, f32 t);
Quaternion Qexp(const Quaternion &q);
Quaternion Qlog(const Quaternion &q);
Quaternion SimpleRotation(const Vec3f &a, const Vec3f &b);