#include "math.h"

f32 randomf32(f32 range)
{
	return (rand() / (f32)RAND_MAX) * 2.0f * range - range;
}

#pragma region VECTOR2

const Vec2f Vec2f::zero = Vec2f();
const Vec2f Vec2f::one = Vec2f(1.0f, 1.0f);

Vec2f::Vec2f(f32 values[2])
{
	x = values[0];
	y = values[1];
}

f32 Vec2f::dot(const Vec2f& other)
{
	return (x * other.x + y * other.y);
}

f32 Vec2f::mod() const
{
	return sqrt(x*x + y*y);
}

std::string Vec2f::toString() const
{
	std::stringstream ss;
	ss << x << ", " << y;
	return ss.str();
}

Vec2f Vec2f::random(f32 range)
{
	return Vec2f(randomf32(range), randomf32(range));
}

f32 Vec2f::distance(const Vec2f& other) const
{
	Vec2f to = *this - other;
	return to.mod();
}

void Vec2f::normalize() 
{
	f32 a_mod = mod();
	x /= a_mod;
	y /= a_mod;
}

bool Vec2f::equals(const Vec2f& other)
{
	return (x == other.x && y == other.y);
}

bool Vec2f::equals_offset(const Vec2f& other, f32 offset)
{
	f32 a_x = abs(x - other.x);
	f32 a_y = abs(y - other.y);

	return (a_x < offset && a_y < offset);
}

Vec2f Vec2f::normalized() const
{
	f32 a_mod = mod();
	return Vec2f(x / a_mod, y / a_mod);
}

Vec2f Vec2f::operator + (const Vec2f & op) const
{
	return Vec2f(x+op.x, y+op.y);
}

Vec2f Vec2f::operator - (const Vec2f & op) const
{
	return Vec2f(x-op.x, y-op.y);
}

Vec2f Vec2f::operator * (f32 op) const
{
	return Vec2f(x*op, y*op);
}

Vec2f Vec2f::operator / (f32 op) const
{
	return Vec2f(x/op, y/op);
}

void Vec2f::operator *= (f32 op)
{
	x = x*op;
	y = y*op;
}

void Vec2f::operator /= (f32 op)
{
	x = x / op;
	y = y / op;
}

Vec2f operator * (f32 f, const Vec2f & op)
{
	return op * f;
}


Vec2f operator / (f32 f, const Vec2f & op)
{
	return op / f;
}
#pragma endregion

#pragma region MAT33
const Mat33f identity = Mat33f();

void Mat33f::setIdentity()
{
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;

	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
}

void Mat33f::setTranslation(const Vec2f& translation)
{
	setIdentity();
	m[2][0] = translation.x;
	m[2][1] = translation.y;
}

void Mat33f::setRotation(f32 rotX, f32 rotY, f32 rotZ)
{

}

void Mat33f::setScale(const Vec2f& scale)
{
	setIdentity();
	m[0][0] = scale.x;
	m[1][1] = scale.y;
}

void Mat33f::transpose()
{
	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[2][1], m[1][2]);
}

void Mat33f::inverse()
{

}

void Mat33f::orthonormalize()
{

}

void Mat33f::translate(const Vec2f& translation)
{
	m[2][0] += translation.x;
	m[2][0] += translation.y;
}

void Mat33f::scale(const Vec2f& scale)
{
	m[0][0] += scale.x;
	m[1][1] += scale.y;
}

Vec2f Mat33f::up() const
{
	return Vec2f();
}

Vec2f Mat33f::right() const
{
	return Vec2f();
}

void Mat33f::lookAt(Vec2f& v)
{

}

Mat33f Mat33f::operator * (f32 val)
{
	for (s32 i = 0; i < 8; i++)
		M[i] = M[i] * val;

	return *this;
}

Mat33f Mat33f::operator * (const Mat33f& other)
{
	Mat33f result;
	for (s32 i = 0; i < 2; i++)
	{
		for (s32 j = 0; j < 2; j++)
			result.m[i][j] = m[i][j] * other.m[j][i];
	}
	return result;
}

//void operator *= (const Mat33& one, const Mat33& other)
//{
//	one = one * other;
//}
#pragma endregion

#pragma region VECTOR3

const Vec3f Vec3f::zero = Vec3f();
const Vec3f Vec3f::one = Vec3f(1.0, 1.0, 1.0);

Vec3f::Vec3f(f32 values[3])
{
	x = values[0];
	y = values[1];
	y = values[2];
}

bool Vec3f::equals(const Vec3f& other) 
{
	return ((other.x == x) && (other.y == y) && (other.z == z));
}

bool Vec3f::equals_offset(const Vec3f& other, f32 offset)
{
	return (this->distance(other) < offset);
}

f32 Vec3f::mod() const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

void Vec3f::normalize() 
{
	f32 mod = this->mod();

	if (mod == 0)
		return;

	x = x / mod; y = y / mod; z = z / mod;
}

Vec3f Vec3f::normalized() const 
{
	f32 mod = this->mod();
	return Vec3f(x/mod, y/mod, z/mod);
}

f32 Vec3f::dot(const Vec3f& other) const 
{
	return (other.x*x) + (other.y*y) + (other.z*z);
}

f32 dot(const Vec3f& one, const Vec3f& other)
{
	return (other.x * one.x) + (other.y * one.y) + (other.z * one.z);
}

Vec3f Vec3f::cross(const Vec3f& other) const
{
	return Vec3f((y*other.z) - (z * other.y), (z*other.x) - (x * other.z), (x*other.y - y * other.x));
}

Vec3f cross(const Vec3f& one, const Vec3f& other)
{
	return Vec3f(one.y*other.z - one.z * other.y, one.z*other.x - one.x * other.z, one.x*other.y - one.y * other.x);
}

Vec3f Vec3f::operator + (const Vec3f & op) const
{
	return Vec3f(x + op.x, y + op.y, z + op.z);
}

Vec3f Vec3f::operator - (const Vec3f & op) const
{
	return Vec3f(x - op.x, y - op.y, z - op.z);
}

f32 Vec3f::distance(const Vec3f & op) const
{
	f32 x1 = x - op.x;
	f32 y1 = y - op.y;
	f32 z1 = z - op.z;

	return sqrt((x1 * x1) + (y1 * y1) + (z1 * z1));
}

std::string Vec3f::toString() const
{
	std::stringstream ss;
	ss << x << ", " << y << ", " << z;
	return ss.str();
}

Vec3f Vec3f::random(f32 range)
{
	return Vec3f(randomf32(range), randomf32(range), randomf32(range));
}

void Vec3f::operator *= (f32 op) 
{
	x = x * op;
	y = y * op;
	z = z * op;
}

void Vec3f::operator /= (f32 op)
{
	x = x / op;
	y = y / op;
	z = z / op;
}

Vec3f Vec3f::operator * (f32 op) const
{
	return Vec3f(x * op, y * op, z * op);
}

Vec3f Vec3f::operator / (f32 op) const
{
	return Vec3f(x / op, y / op, z / op);
}

Vec3f Vec3f::slerp(const Vec3f& start, const Vec3f& end, f32 percent)
{
	f32 dot = start.dot(end);
	dot = fminf(1.0f, fmaxf(dot, -1.0f));
	f32 theta = acos(dot) * percent;
	Vec3f RelativeVec = end - (start * dot);
	RelativeVec.normalize();
	return ((start* cos(theta)) + (RelativeVec * sin(theta)));
}

bool operator == (const Vec3f& a, Vec3f& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

Vec3f operator * (f32 a, Vec3f b)
{
	return Vec3f(a*b.x, a*b.y, a*b.z);
}

#pragma endregion

#pragma region MAT44
const Mat44f Mat44f::identity = Mat44f();

Mat44f::Mat44f(const Mat44f& other)
{
	memcpy((void*)&values, (void*)&other.values, sizeof(f32) * 16);
};

inline void Mat44f::setIdentity()
{
	m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
	m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
	m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
	m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
}

void Mat44f::setTranslation(const Vec3f& translation)
{
	setTranslation(translation.x, translation.y, translation.z);
}

void Mat44f::setTranslation(f32 tx, f32 ty, f32 tz)
{
	setIdentity();
	m[3][0] = tx;
	m[3][1] = ty;
	m[3][2] = tz;
}

Vec3f Mat44f::getPosition() const
{
	return Vec3f(m[3][0], m[3][1], m[3][2]);
}

Vec3f Mat44f::getScale() const
{
	Vec3f aux;
	aux.x = m[0][0];
	aux.y = m[0][1];
	aux.z = m[0][2];
	f32 scaleX = aux.mod();

	aux.x = m[1][0];
	aux.y = m[1][1];
	aux.z = m[1][2];
	f32 scaleY = aux.mod();

	aux.x = m[2][0];
	aux.y = m[2][1];
	aux.z = m[2][2];
	f32 scaleZ = aux.mod();

	return Vec3f(scaleX, scaleY, scaleZ);
}

std::string Mat44f::toString() const
{
	std::stringstream ss;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
			ss << m[i][j] << " ";

		ss << std::endl;
	}
	return ss.str();
}

void Mat44f::setRotation(f32 angleX, f32 angleY, f32 angleZ)
{
	setIdentity();

	Mat44f rx, ry, rz;
	//TODO: remove this 3 matrix concats?
	rx.setRotation(angleX, Vec3f(1.0, 0.0, 0.0));
	ry.setRotation(angleY, Vec3f(0.0, 1.0, 0.0));
	rz.setRotation(angleZ, Vec3f(0.0, 0.0, 1.0));

	(*this) = (*this) * rx * ry * rz;
}

void Mat44f::setRotation(f32 angle, const Vec3f & axis)
{
	setIdentity();
	Vec3f axis_n = axis;
	axis_n.normalize();

	f32 c = (f32)cos((f32)DEGTORAD(angle));
	f32 s = (f32)sin((f32)DEGTORAD(angle));
	f32 t = (f32)1 - c;

	m[0][0] = t * axis_n.x * axis_n.x + c;
	m[0][1] = t * axis_n.x * axis_n.y - s * axis_n.z;
	m[0][2] = t * axis_n.x * axis_n.z + s * axis_n.y;

	m[1][0] = t * axis_n.x * axis_n.y + s * axis_n.z;
	m[1][1] = t * axis_n.y * axis_n.y + c;
	m[1][2] = t * axis_n.y * axis_n.z - s * axis_n.x;

	m[2][0] = t * axis_n.x * axis_n.z - s * axis_n.y;
	m[2][1] = t * axis_n.y * axis_n.z + s * axis_n.x;
	m[2][2] = t * axis_n.z * axis_n.z + c;

	m[3][3] = 1.0f;
}

void Mat44f::setScale(const Vec3f & scale)
{
	setIdentity();
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void Mat44f::transpose()
{
	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[0][3], m[3][0]);

	std::swap(m[1][2], m[2][1]);
	std::swap(m[1][3], m[3][1]);

	std::swap(m[2][3], m[3][2]);
}

bool Mat44f::inverse()
{
	u32 i, j, k, swap;
	f32 t;
	Mat44f temp, final;
	final.setIdentity();

	temp = (*this);

	u32 m, n;
	m = n = 4;

	for (i = 0; i < m; i++)
	{
		// Look for largest element in column

		swap = i;
		for (j = i + 1; j < m; j++)// m or n
		{
			if (fabs(temp.m[j][i]) > fabs(temp.m[swap][i]))
				swap = j;
		}

		if (swap != i)
		{
			// Swap rows.
			for (k = 0; k < n; k++)
			{
				std::swap(temp.m[i][k], temp.m[swap][k]);
				std::swap(final.m[i][k], final.m[swap][k]);
			}
		}

		// No non-zero pivot.  The CMatrix is singular, which shouldn't
		// happen.  This means the user gave us a bad CMatrix.


#define MATRIX_SINGULAR_THRESHOLD 0.00001 //change this if you experience problems with matrices

		if (fabsf(temp.m[i][i]) <= MATRIX_SINGULAR_THRESHOLD)
		{
			final.setIdentity();
			return false;
		}
#undef MATRIX_SINGULAR_THRESHOLD

		t = 1.0f / temp.m[i][i];

		for (k = 0; k < n; k++)//m or n
		{
			temp.m[i][k] *= t;
			final.m[i][k] *= t;
		}

		for (j = 0; j < m; j++) // m or n
		{
			if (j != i)
			{
				t = temp.m[j][i];
				for (k = 0; k < n; k++)//m or n
				{
					temp.m[j][k] -= (temp.m[i][k] * t);
					final.m[j][k] -= (final.m[i][k] * t);
				}
			}
		}
	}

	*this = final;

	return true;
}

void Mat44f::translate(const Vec3f & translation)
{
	Mat44f T;
	T.setTranslation(translation);
	*this = (*this) * T;
}

void Mat44f::translate(f32 tx, f32 ty, f32 tz)
{
	Mat44f T;
	T.setTranslation(tx, ty, tz);
	*this = (*this) * T;
}

void Mat44f::translateLocal(const Vec3f & translation)
{
	Mat44f T;
	T.setTranslation(translation);
	*this = T * (*this);
}

void Mat44f::translateLocal(f32 tx, f32 ty, f32 tz)
{
	Mat44f T;
	T.setTranslation(tx, ty, tz);
	*this = T * (*this);
}

void Mat44f::rotateLocal(f32 rotX, f32 rotY, f32 rotZ)
{
	Mat44f rotation;
	rotation.setRotation(rotX, rotY, rotZ);
	*this = rotation * *this;
}

void Mat44f::rotate(f32 rotX, f32 rotY, f32 rotZ)
{
	Mat44f rotation;
	rotation.setRotation(rotX, rotY, rotZ);

	(*this) = (*this) * rotation;
}

void Mat44f::scale(const Vec3f & scale)
{
	m[0][0] *= scale.x;
	m[1][1] *= scale.y;
	m[2][2] *= scale.z;
}

void Mat44f::replaceScale(const Vec3f & scale)
{
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void Mat44f::setLookAt(const Vec3f & position, const Vec3f & targetPos32, const Vec3f & up)
{
	Vec3f front = (targetPos32 - position);
	front.normalize();
	Vec3f right = front.cross(up);
	right.normalize();
	Vec3f top = right.cross(front);
	top.normalize();

	setIdentity();

	m[0][0] = right.x; m[0][1] = top.x; m[0][2] = -front.x;
	m[1][0] = right.y; m[1][1] = top.y; m[1][2] = -front.y;
	m[2][0] = right.z; m[2][1] = top.z; m[2][2] = -front.z;

	translateLocal(-position.x, -position.y, -position.z);
}

void Mat44f::setPerspective(f32 fov, f32 aspect, f32 near, f32 far)
{
	setIdentity();

	f32 f = 1.0f / tan((f32)DEGTORAD(fov) * 0.5f);

	m[0][0] = f / aspect;
	m[1][1] = f;
	m[2][2] = (far + near) / (near - far);
	m[2][3] = -1.0f;
	m[3][2] = 2.0f * (far * near) / (near - far);
	m[3][3] = 0.0f;
}

void Mat44f::setOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
	m[0][0] = 2.0f / (right - left);
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 2.0f / (top - bottom);
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = -2.0f / (far - near);
	m[2][3] = 0.0f;

	m[3][0] = -(right + left) / (right - left);
	m[3][1] = -(top + bottom) / (top - bottom);
	m[3][2] = -(far + near) / (far - near);
	m[3][3] = 1.0f;
}

Mat44f Mat44f::getRotationOnly()
{
	Vec3f scale = getScale();
	Mat44f rot;
	rot = *this;

	rot.m[3][0] = 0.0f;
	rot.m[3][1] = 0.0f;
	rot.m[3][2] = 0.0f;

	rot.m[0][3] = 0.0f;
	rot.m[1][3] = 0.0f;
	rot.m[2][3] = 0.0f;

	rot.m[3][3] = 1.0f;

	rot.m[0][0] = rot.m[0][0] / scale.x;
	rot.m[0][1] = rot.m[0][1] / scale.x;
	rot.m[0][2] = rot.m[0][2] / scale.x;

	rot.m[1][0] = rot.m[1][0] / scale.y;
	rot.m[1][1] = rot.m[1][1] / scale.y;
	rot.m[1][2] = rot.m[1][2] / scale.y;

	rot.m[2][0] = rot.m[2][0] / scale.z;
	rot.m[2][1] = rot.m[2][1] / scale.z;
	rot.m[2][2] = rot.m[2][2] / scale.z;

	return rot;
}

Vec3f Mat44f::rotateVector(const Vec3f & v)
{
	return (*this) * v;
}

Mat44f Mat44f::operator * (f32 val)
{
	Mat44f result;
	for (size_t i = 0; i < 16; i++)
		result.values[i] = values[i] * val;

	return result;
}

Mat44f Mat44f::operator * (const Mat44f& other)
{
	Mat44f result;
	size_t aux_row;
	size_t aux_col;

	for (size_t i = 0; i < 16; i++)
	{
		aux_row = i / 4;
		aux_col = i % 4;
		result.values[i] =
			m[aux_row][0] * other.m[0][aux_col] +
			m[aux_row][1] * other.m[1][aux_col] +
			m[aux_row][2] * other.m[2][aux_col] +
			m[aux_row][3] * other.m[3][aux_col];
	}

	return result;
}

Vec3f Mat44f::operator * (const Vec3f& other)
{
	Vec3f result;
	for (size_t i = 0; i < 3; i++)
	{
		result.values[i] =
			m[0][i] * other.x +
			m[1][i] * other.y +
			m[2][i] * other.z +
			m[3][i] * 0;
	}

	return result;
}

#pragma endregion


#pragma region QUATERNION
Quaternion::Quaternion()
{
}

Quaternion::Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w)
{
}

Quaternion::Quaternion(const f32 X, const f32 Y, const f32 Z, const f32 W) : x(X), y(Y), z(Z), w(W)
{
}

Quaternion Quaternion::invert() const
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

void Quaternion::set(const f32 X, const f32 Y, const f32 Z, const f32 W)
{
	x = X;  y = Y; z = Z; w = W;
}

Quaternion::Quaternion(const f32* q)
{
	x = q[0];
	y = q[1];
	z = q[2];
	w = q[3];
}

void Quaternion::identity()
{
	x = y = z = 0.0f; w = 1.0f;
}

void Quaternion::slerp(const Quaternion& b, f32 t)
{
	//quaternion a(*this);
	*this = Qslerp(*this, b, t);
}

void Quaternion::slerp(const Quaternion& q2, f32 t, Quaternion &q3) const
{
	q3 = Qslerp(*this, q2, t);
}

void Quaternion::lerp(const Quaternion& b, f32 t)
{
	*this = Qlerp(*this, b, t);
}

void Quaternion::lerp(const Quaternion& q2, f32 t, Quaternion &q3) const
{
	q3 = Qlerp(*this, q2, t);
}

void Quaternion::setAxisAngle(const Vec3f& axis, const f32 angle)
{
	f32 s;
	s = sinf(angle * 0.5f);

	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	w = cosf(angle * 0.5f);
}

Quaternion::Quaternion(const Vec3f& axis, f32 angle)
{
	setAxisAngle(axis, angle);
}

void Quaternion::operator *= (const Quaternion &q)
{
	Quaternion quaternion = *this * q;
	*this = quaternion;
}

void Quaternion::operator*=(const Vec3f& v)
{
	*this = *this*v;
}

void Quaternion::operator += (const Quaternion &q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
}

Quaternion operator * (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion q;

	q.x = q1.y*q2.z - q1.z*q2.y + q1.w*q2.x + q1.x*q2.w;
	q.y = q1.z*q2.x - q1.x*q2.z + q1.w*q2.y + q1.y*q2.w;
	q.z = q1.x*q2.y - q1.y*q2.x + q1.w*q2.z + q1.z*q2.w;
	q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
	return q;
}

/*
//http://www.cs.yorku.ca/~arlene/aquasim/mymath.c
quaternion operator * (const quaternion& p, const quaternion& q)
{
return quaternion(
p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y,
p.w*q.y + p.y*q.w + p.z*q.x - p.x*q.z,
p.w*q.z + p.z*q.w + p.x*q.y - p.y*q.x,
p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z);
}
*/

Quaternion operator * (const Quaternion &q, const Vec3f& v)
{
	return Quaternion
	(
		q.w*v.x + q.y*v.z - q.z*v.y,
		q.w*v.y + q.z*v.x - q.x*v.z,
		q.w*v.z + q.x*v.y - q.y*v.x,
		-(q.x*v.x + q.y*v.y + q.z*v.z)
	);
}

void Quaternion::operator *= (f32 f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
}

Quaternion operator * (const Quaternion &q, f32 f)
{
	Quaternion q1;
	q1.x = q.x*f;
	q1.y = q.y*f;
	q1.z = q.z*f;
	q1.w = q.w*f;
	return q1;
}

Quaternion operator * (f32 f, const Quaternion &q)
{
	Quaternion q1;
	q1.x = q.x*f;
	q1.y = q.y*f;
	q1.z = q.z*f;
	q1.w = q.w*f;
	return q1;
}

void Quaternion::normalize()
{
	f32 len = length();
	len = 1.0f / len;

	x *= len;
	y *= len;
	z *= len;
	w *= len;
}

void Quaternion::computeMinimumRotation(const Vec3f& rotateFrom, const Vec3f& rotateTo)
{
	// Check if the vectors are valid.
	//rotateFrom.GetLength()==0.0f
	//rotateTo.GetLength()==0.0f

	Vec3f from(rotateFrom);
	from.normalize();
	Vec3f to(rotateTo);
	to.normalize();

	const f32 _dot = dot(from, to);
	Vec3f crossvec = cross(from, to);
	const f32 crosslen = (f32)crossvec.mod();

	if (crosslen == 0.0f)
	{
		// Parallel vectors
		// Check if they are pos32ing in the same direction.
		if (_dot > 0.0f)
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		// Ok, so they are parallel and pos32ing in the opposite direction
		// of each other.
		else
		{
			// Try crossing with x axis.
			Vec3f t = cross(from, Vec3f(1.0f, 0.0f, 0.0f));
			// If not ok, cross with y axis.
			if (t.mod() == 0.0f)
				cross(from, Vec3f(0.0f, 1.0f, 0.0f));

			t.normalize();
			x = t.x;
			y = t.y;
			z = t.z;
			w = 0.0f;
		}
	}
	else
	{ // Vectors are not parallel
		crossvec.normalize();
		// The fabs() wrapping is to avoid problems when `dot' "overflows"
		// a tiny wee bit, which can lead to sqrt() returning NaN.
		crossvec *= (f32)sqrt(0.5f * fabs(1.0f - _dot));
		// The fabs() wrapping is to avoid problems when `dot' "underflows"
		// a tiny wee bit, which can lead to sqrt() returning NaN.
		x = crossvec.x;
		y = crossvec.y;
		z = crossvec.z,
		w = (f32)sqrt(0.5 * fabs(1.0 + _dot));
	}
}

Quaternion SimpleRotation(const Vec3f &a, const Vec3f &b)
{
	Vec3f axis = cross(a, b);
	f32 _dot = dot(a, b);
	if (_dot < -1.0f + /*DOT_EPSILON*/0.001f) return Quaternion(0, 1, 0, 0);

	Quaternion result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (_dot + 1.0f) * 0.5f);
	result.normalize();
	//fast_normalize(&result);

	return result;
}


f32 Quaternion::length() const
{
	return sqrtf(w * w + x * x + y * y + z * z);
}

f32 Quaternion::squaredLength() const
{
	return w * w + x * x + y * y + z * z;
}

void Quaternion::toMatrix(Mat44f& matrix) const
{
	/*
	If q is guaranteed to be a unit quaternion, s will always
	be 2.  In that case, this calculation can be optimized out.
	*/
	//const f32	norm = x*x + y*y + z*z + w*w;
	//const f32 s = (norm > 0) ? 2/norm : 0;

	const f32 s = 2;

	/*
	Precalculate coordinate products
	*/
	const f32 xx = x * x * s;
	const f32 yy = y * y * s;
	const f32 zz = z * z * s;
	const f32 xy = x * y * s;
	const f32 xz = x * z * s;
	const f32 yz = y * z * s;
	const f32 wx = w * x * s;
	const f32 wy = w * y * s;
	const f32 wz = w * z * s;


	matrix.m[0][0] = 1.0f - (yy + zz);
	matrix.m[0][1] = xy + wz;
	matrix.m[0][2] = xz - wy;

	matrix.m[1][0] = xy - wz;

	matrix.m[1][1] = 1.0f - (xx + zz);

	matrix.m[1][2] = yz + wx;

	matrix.m[2][0] = xz + wy;
	matrix.m[2][1] = yz - wx;
	matrix.m[2][2] = 1.0f - (xx + yy);

	matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0;
	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0;
	matrix.m[3][3] = 1;
}

Mat44f Quaternion::toMatrix() const
{
	Mat44f matrix;
	const f32 s = 2;

	const f32 xx = x * x * s;
	const f32 yy = y * y * s;
	const f32 zz = z * z * s;
	const f32 xy = x * y * s;
	const f32 xz = x * z * s;
	const f32 yz = y * z * s;
	const f32 wx = w * x * s;
	const f32 wy = w * y * s;
	const f32 wz = w * z * s;

	matrix.m[0][0] = 1.0f - (yy + zz);
	matrix.m[1][0] = xy + wz;
	matrix.m[2][0] = xz - wy;

	matrix.m[0][1] = xy - wz;
	matrix.m[1][1] = 1.0f - (xx + zz);
	matrix.m[2][1] = yz + wx;

	matrix.m[0][2] = xz + wy;
	matrix.m[1][2] = yz - wx;
	matrix.m[2][2] = 1.0f - (xx + yy);

	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0;
	matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0;
	matrix.m[3][3] = 1;

	return matrix;
}

void Quaternion::fromMatrix(const Mat44f & mat)
{
	const f32 trace = 1.0f + mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

	if (trace > 0.00001f)
	{
		const f32 s = sqrt(trace) * 2;
		x = (mat.m[2][1] - mat.m[1][2]) / s;
		y = (mat.m[0][2] - mat.m[2][0]) / s;
		z = (mat.m[1][0] - mat.m[0][1]) / s;
		w = s / 4;
		return;
	}
	else if (mat.m[0][0] > mat.m[1][1] && mat.m[0][0] > mat.m[2][2])
	{
		const f32 s = sqrt(1.0f + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]) * 2;

		x = s / 4;
		y = (mat.m[1][0] + mat.m[0][1]) / s;
		z = (mat.m[0][2] + mat.m[2][0]) / s;
		w = (mat.m[2][1] - mat.m[1][2]) / s;
		return;
	}
	else if (mat.m[1][1] > mat.m[2][2])
	{
		const f32 s = sqrt(1.0f + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]) * 2;

		x = (mat.m[1][0] + mat.m[0][1]) / s;
		y = s / 4;
		z = (mat.m[2][1] + mat.m[1][2]) / s;
		w = (mat.m[0][2] - mat.m[2][0]) / s;
		return;
	}
	else
	{
		const f32 s = sqrt(1.0f + mat.m[2][2] - mat.m[0][0] - mat.m[1][1]) * 2;

		x = (mat.m[0][2] + mat.m[2][0]) / s;
		y = (mat.m[2][1] + mat.m[1][2]) / s;
		z = s / 4;
		w = (mat.m[1][0] - mat.m[0][1]) / s;
		return;
	}
}

f32 DotProduct(const Quaternion& q1, const Quaternion& q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

bool operator==(const Quaternion& q1, const Quaternion& q2)
{
	return ((q1.x == q2.x) && (q1.y == q2.y) &&
		(q1.z == q2.z) && (q1.w == q2.w));
}

bool operator!=(const Quaternion& q1, const Quaternion& q2)
{
	return ((q1.x != q2.x) || (q1.y != q2.y) ||
		(q1.z != q2.z) || (q1.w != q2.w));
}

/*
Logarithm of a quaternion, given as:
Qlog(q) = v*a where q = [cos(a),v*sin(a)]
*/
Quaternion Qlog(const Quaternion &q)
{
	f32 a = static_cast<f32>(acos(q.w));
	f32 sina = static_cast<f32>(sin(a));
	Quaternion ret;
	ret.w = 0;
	if (sina > 0)
	{
		ret.x = a * q.x / sina;
		ret.y = a * q.y / sina;
		ret.z = a * q.z / sina;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}
	return ret;
}

/*
e^quaternion given as:
Qexp(v*a) = [cos(a),vsin(a)]
*/
Quaternion Qexp(const Quaternion &q)
{
	f32 a = static_cast<f32>(sqrt(q.x*q.x + q.y*q.y + q.z*q.z));
	f32 sina = static_cast<f32>(sin(a));
	f32 cosa = static_cast<f32>(cos(a));
	Quaternion ret;

	ret.w = cosa;
	if (a > 0)
	{
		ret.x = sina * q.x / a;
		ret.y = sina * q.y / a;
		ret.z = sina * q.z / a;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}

	return ret;
}

Quaternion operator + (const Quaternion &q1, const Quaternion& q2)
{
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

/*
Linear s32erpolation between two quaternions
*/
Quaternion Qlerp(const Quaternion &q1, const Quaternion &q2, f32 t)
{
	Quaternion ret;
	//ret = q1 + t*(q2-q1);

	Quaternion b;
	if (DotProduct(q1, q2)< 0.0f)
		b.set(-q2.x, -q2.y, -q2.z, -q2.w);
	else b = q2;

	ret = q1 * (1 - t) + b * t;

	ret.normalize();
	return ret;
}

Quaternion Qslerp(const Quaternion &q1, const Quaternion &q2, f32 t)
{
	Quaternion q3;
	f32 dot = DotProduct(q1, q2);

	//dot = cos(theta)
	//if (dot < 0), q1 and q2 are more than 90 degrees apart,
	//so we can invert one to reduce spinning

	if (dot < 0)
	{
		dot = -dot;
		q3.set(-q2.x, -q2.y, -q2.z, -q2.w);
	}
	else
	{
		q3 = q2;
	}

	if (dot < 0.95f)
	{
		f32 angle = static_cast<f32>(acosf(dot));
		f32 sina, sinat, sinaomt;
		sina = sinf(angle);
		sinat = sinf(angle*t);
		sinaomt = sinf(angle*(1 - t));
		return (q1*sinaomt + q3 * sinat) * (1.0f / sina);
	}

	//if the angle is small, use linear s32erpolation

	else
	{
		return Qlerp(q1, q3, t);
	}



	/*
	f32 fCos = DotProduct(q1,q2);
	if (fCos > 1)
	fCos = 1;
	if (fCos < -1)
	fCos = -1;
	f32 fAngle = acosf(fCos);
	if ( fabs(fAngle) < 0.0001 )
	{
	return q1;
	}
	f32 fSin = sinf(fAngle);
	f32 fInvSin = 1.0f/fSin;
	f32 fCoeff0 = sinf((1.0f-fT)*fAngle)*fInvSin;
	f32 fCoeff1 = sinf(fT*fAngle)*fInvSin;
	return fCoeff0*q1 + fCoeff1*q2;
	*/
}


/*
Given 3 quaternions, qn-1,qn and qn+1, calculate a control pos32 to be used in spline s32erpolation
*/

Quaternion& Quaternion::operator -()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;

	return *this;
}

void Quaternion::getAxisAngle(Vec3f &v, f32 &angle) const
{
	angle = 2.0f * acosf(w);
	f32 factor = 1.0f / sqrtf(1 - w * w);
	v.x = x * factor;
	v.y = y * factor;
	v.z = z * factor;
}

Vec3f Quaternion::rotate(const Vec3f& v) const
{
	Vec3f ret;
	Quaternion temp(-x, -y, -z, w);
	temp *= v;
	temp *= (*this);

	ret.x = temp.x;
	ret.y = temp.y;
	ret.z = temp.z;

	return ret;
}

void Quaternion::toEulerAngles(Vec3f &euler) const
{
	/// Local Variables ///////////////////////////////////////////////////////////
	f32 matrix[3][3];
	f32 cx, sx;
	f32 cy, sy;
	f32 cz, sz;

	const f32 y2 = y * y;
	///////////////////////////////////////////////////////////////////////////////
	// CONVERT QUATERNION TO MATRIX - I DON'T REALLY NEED ALL OF IT
	matrix[0][0] = 1.0f - (2.0f * y2) - (2.0f * z * z);
	//	matrix[0][1] = (2.0f * quat->x * quat->y) - (2.0f * quat->w * quat->z);
	//	matrix[0][2] = (2.0f * quat->x * quat->z) + (2.0f * quat->w * quat->y);
	matrix[1][0] = (2.0f * x * y) + (2.0f * w * z);
	//	matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
	//	matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
	matrix[2][0] = (2.0f * x * z) - (2.0f * w * y);
	matrix[2][1] = (2.0f * y * z) + (2.0f * w * x);
	matrix[2][2] = 1.0f - (2.0f * x * x) - (2.0f * y2);

	sy = -matrix[2][0];
	cy = sqrtf(1 - (sy * sy));
	euler.y = atan2f(sy, cy);
	//euler->y = (yr * 180.0f) / (f32)M_PI;

	// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270 
	// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
	if (sy != 1.0f && sy != -1.0f)
	{
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		//euler->x = ((f32)atan2(sx,cx) * 180.0f) / (f32)M_PI;	// RAD TO DEG
		euler.x = atan2f(sx, cx);

		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		//euler->z = ((f32)atan2(sz,cz) * 180.0f) / (f32)M_PI;	// RAD TO DEG
		euler.z = atan2f(sz, cz);
	}
	else
	{
		// SINCE Cos(Y) IS 0, I AM SCREWED.  ADOPT THE STANDARD Z = 0
		// I THINK THERE IS A WAY TO FIX THIS BUT I AM NOT SURE.  EULERS SUCK
		// NEED SOME MORE OF THE MATRIX TERMS NOW
		matrix[1][1] = 1.0f - (2.0f * x * x) - (2.0f * z * z);
		matrix[1][2] = (2.0f * y * z) - (2.0f * w * x);
		cx = matrix[1][1];
		sx = -matrix[1][2];
		//euler->x = ((f32)atan2(sx,cx) * 180.0f) / (f32)M_PI;	// RAD TO DEG
		euler.x = atan2f(sx, cx);

		cz = 1.0f;
		sz = 0.0f;
		//euler->z = ((f32)atan2(sz,cz) * 180.0f) / (f32)M_PI;	// RAD TO DEG
		euler.z = atan2f(sz, cz);
	}
}

inline Quaternion Quaternion::Euler(const Vec3f & eulerAngles)
{
	f32 yaw = eulerAngles.x;
	f32 pitch = eulerAngles.y;
	f32 roll = eulerAngles.z;
	f32 rollOver2 = roll * 0.5f;
	f32 sinRollOver2 = (f32)sin((f32)rollOver2);
	f32 cosRollOver2 = (f32)cos((f32)rollOver2);
	f32 pitchOver2 = pitch * 0.5f;
	f32 sinPitchOver2 = (f32)sin((f32)pitchOver2);
	f32 cosPitchOver2 = (f32)cos((f32)pitchOver2);
	f32 yawOver2 = yaw * 0.5f;
	f32 sinYawOver2 = (f32)sin((f32)yawOver2);
	f32 cosYawOver2 = (f32)cos((f32)yawOver2);
	Quaternion result;
	result.x = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
	result.y = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
	result.z = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
	result.w = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
	return result;
}


void Quaternion::setAxisAngle(f32 ax, f32 ay, f32 az, f32 angle)
{
	const f32 halfAngle = angle * 0.5f;
	const f32 s = sinf(halfAngle);

	w = cosf(halfAngle);
	x = ax * s;
	y = ay * s;
	z = az * s;

}
#pragma endregion