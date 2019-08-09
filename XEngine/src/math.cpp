#include "math.h"

float randomFloat(float range)
{
	return (rand() / (float)RAND_MAX) * 2.0f * range - range;
}

#pragma region VECTOR2

const Vector2 Vector2::zero = Vector2();
const Vector2 Vector2::one = Vector2(1.0f, 1.0f);

Vector2::Vector2(float values[2])
{
	x = values[0];
	y = values[1];
}

float Vector2::dot(const Vector2& other)
{
	return (x * other.x + y * other.y);
}

float Vector2::mod() const
{
	return sqrt(x*x + y*y);
}

std::string Vector2::toString() const
{
	std::stringstream ss;
	ss << x << ", " << y;
	return ss.str();
}

Vector2 Vector2::random(float range)
{
	return Vector2(randomFloat(range), randomFloat(range));
}

float Vector2::distance(const Vector2& other) const
{
	Vector2 to = *this - other;
	return to.mod();
}

void Vector2::normalize() 
{
	float a_mod = mod();
	x /= a_mod;
	y /= a_mod;
}

bool Vector2::equals(const Vector2& other)
{
	return (x == other.x && y == other.y);
}

bool Vector2::equals_offset(const Vector2& other, float offset)
{
	float a_x = abs(x - other.x);
	float a_y = abs(y - other.y);

	return (a_x < offset && a_y < offset);
}

Vector2 Vector2::normalized() const
{
	float a_mod = mod();
	return Vector2(x / a_mod, y / a_mod);
}

Vector2 Vector2::operator + (const Vector2 & op) const
{
	return Vector2(x+op.x, y+op.y);
}

Vector2 Vector2::operator - (const Vector2 & op) const
{
	return Vector2(x-op.x, y-op.y);
}

Vector2 Vector2::operator * (float op) const
{
	return Vector2(x*op, y*op);
}

Vector2 Vector2::operator / (float op) const
{
	return Vector2(x/op, y/op);
}

void Vector2::operator *= (float op)
{
	x = x*op;
	y = y*op;
}

void Vector2::operator /= (float op)
{
	x = x / op;
	y = y / op;
}

Vector2 operator * (float f, const Vector2 & op)
{
	return op * f;
}


Vector2 operator / (float f, const Vector2 & op)
{
	return op / f;
}
#pragma endregion

#pragma region MAT33
const Mat33 identity = Mat33();

void Mat33::setIdentity()
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

void Mat33::setTranslation(const Vector2& translation)
{
	setIdentity();
	m[2][0] = translation.x;
	m[2][1] = translation.y;
}

void Mat33::setRotation(float rotX, float rotY, float rotZ)
{

}

void Mat33::setScale(const Vector2& scale)
{
	setIdentity();
	m[0][0] = scale.x;
	m[1][1] = scale.y;
}

void Mat33::transpose()
{
	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[2][1], m[1][2]);
}

void Mat33::inverse()
{

}

void Mat33::orthonormalize()
{

}

void Mat33::translate(const Vector2& translation)
{
	m[2][0] += translation.x;
	m[2][0] += translation.y;
}

void Mat33::scale(const Vector2& scale)
{
	m[0][0] += scale.x;
	m[1][1] += scale.y;
}

Vector2 Mat33::up() const
{
	return Vector2();
}

Vector2 Mat33::right() const
{
	return Vector2();
}

void Mat33::lookAt(Vector2& v)
{

}

Mat33 Mat33::operator * (float val)
{
	for (int i = 0; i < 8; i++)
		M[i] = M[i] * val;

	return *this;
}

Mat33 Mat33::operator * (const Mat33& other)
{
	Mat33 result;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
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

const Vector3 Vector3::zero = Vector3();
const Vector3 Vector3::one = Vector3(1.0, 1.0, 1.0);

Vector3::Vector3(float values[3])
{
	x = values[0];
	y = values[1];
	y = values[2];
}

bool Vector3::equals(const Vector3& other) 
{
	return ((other.x == x) && (other.y == y) && (other.z == z));
}

bool Vector3::equals_offset(const Vector3& other, float offset)
{
	return (this->distance(other) < offset);
}

float Vector3::mod() const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

void Vector3::normalize() 
{
	float mod = this->mod();

	if (mod == 0)
		return;

	x = x / mod; y = y / mod; z = z / mod;
}

Vector3 Vector3::normalized() const 
{
	float mod = this->mod();
	return Vector3(x/mod, y/mod, z/mod);
}

float Vector3::dot(const Vector3& other) const 
{
	return (other.x*x) + (other.y*y) + (other.z*z);
}

float dot(const Vector3& one, const Vector3& other)
{
	return (other.x * one.x) + (other.y * one.y) + (other.z * one.z);
}

Vector3 Vector3::cross(const Vector3& other) const
{
	return Vector3((y*other.z) - (z * other.y), (z*other.x) - (x * other.z), (x*other.y - y * other.x));
}

Vector3 cross(const Vector3& one, const Vector3& other)
{
	return Vector3(one.y*other.z - one.z * other.y, one.z*other.x - one.x * other.z, one.x*other.y - one.y * other.x);
}

Vector3 Vector3::operator + (const Vector3 & op) const
{
	return Vector3(x + op.x, y + op.y, z + op.z);
}

Vector3 Vector3::operator - (const Vector3 & op) const
{
	return Vector3(x - op.x, y - op.y, z - op.z);
}

float Vector3::distance(const Vector3 & op) const
{
	float x1 = x - op.x;
	float y1 = y - op.y;
	float z1 = z - op.z;

	return sqrt((x1 * x1) + (y1 * y1) + (z1 * z1));
}

std::string Vector3::toString() const
{
	std::stringstream ss;
	ss << x << ", " << y << ", " << z;
	return ss.str();
}

Vector3 Vector3::random(float range)
{
	return Vector3(randomFloat(range), randomFloat(range), randomFloat(range));
}

void Vector3::operator *= (float op) 
{
	x = x * op;
	y = y * op;
	z = z * op;
}

void Vector3::operator /= (float op)
{
	x = x / op;
	y = y / op;
	z = z / op;
}

Vector3 Vector3::operator * (float op) const
{
	return Vector3(x * op, y * op, z * op);
}

Vector3 Vector3::operator / (float op) const
{
	return Vector3(x / op, y / op, z / op);
}

Vector3 Vector3::slerp(const Vector3& start, const Vector3& end, float percent)
{
	float dot = start.dot(end);
	dot = fminf(1.0f, fmaxf(dot, -1.0f));
	float theta = acos(dot) * percent;
	Vector3 RelativeVec = end - (start * dot);
	RelativeVec.normalize();
	return ((start* cos(theta)) + (RelativeVec * sin(theta)));
}

bool operator == (const Vector3& a, Vector3& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

Vector3 operator * (float a, Vector3 b)
{
	return Vector3(a*b.x, a*b.y, a*b.z);
}

#pragma endregion

#pragma region MAT44
const Mat44 Mat44::identity = Mat44();

Mat44::Mat44(const Mat44& other)
{
	memcpy((void*)&values, (void*)&other.values, sizeof(float) * 16);
};

inline void Mat44::setIdentity()
{
	m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
	m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
	m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
	m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
}

void Mat44::setTranslation(const Vector3& translation)
{
	setTranslation(translation.x, translation.y, translation.z);
}

void Mat44::setTranslation(float tx, float ty, float tz)
{
	setIdentity();
	m[3][0] = tx;
	m[3][1] = ty;
	m[3][2] = tz;
}

Vector3 Mat44::getPosition() const
{
	return Vector3(m[3][0], m[3][1], m[3][2]);
}

Vector3 Mat44::getScale() const
{
	Vector3 aux;
	aux.x = m[0][0];
	aux.y = m[0][1];
	aux.z = m[0][2];
	float scaleX = aux.mod();

	aux.x = m[1][0];
	aux.y = m[1][1];
	aux.z = m[1][2];
	float scaleY = aux.mod();

	aux.x = m[2][0];
	aux.y = m[2][1];
	aux.z = m[2][2];
	float scaleZ = aux.mod();

	return Vector3(scaleX, scaleY, scaleZ);
}

std::string Mat44::toString() const
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

void Mat44::setRotation(float angleX, float angleY, float angleZ)
{
	setIdentity();

	Mat44 rx, ry, rz;
	//TODO: remove this 3 matrix concats?
	rx.setRotation(angleX, Vector3(1.0, 0.0, 0.0));
	ry.setRotation(angleY, Vector3(0.0, 1.0, 0.0));
	rz.setRotation(angleZ, Vector3(0.0, 0.0, 1.0));

	(*this) = (*this) * rx * ry * rz;
}

void Mat44::setRotation(float angle, const Vector3 & axis)
{
	setIdentity();
	Vector3 axis_n = axis;
	axis_n.normalize();

	float c = (float)cos((float)DEGTORAD(angle));
	float s = (float)sin((float)DEGTORAD(angle));
	float t = (float)1 - c;

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

void Mat44::setScale(const Vector3 & scale)
{
	setIdentity();
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void Mat44::transpose()
{
	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[0][3], m[3][0]);

	std::swap(m[1][2], m[2][1]);
	std::swap(m[1][3], m[3][1]);

	std::swap(m[2][3], m[3][2]);
}

bool Mat44::inverse()
{
	unsigned int i, j, k, swap;
	float t;
	Mat44 temp, final;
	final.setIdentity();

	temp = (*this);

	unsigned int m, n;
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

void Mat44::translate(const Vector3 & translation)
{
	Mat44 T;
	T.setTranslation(translation);
	*this = (*this) * T;
}

void Mat44::translate(float tx, float ty, float tz)
{
	Mat44 T;
	T.setTranslation(tx, ty, tz);
	*this = (*this) * T;
}

void Mat44::translateLocal(const Vector3 & translation)
{
	Mat44 T;
	T.setTranslation(translation);
	*this = T * (*this);
}

void Mat44::translateLocal(float tx, float ty, float tz)
{
	Mat44 T;
	T.setTranslation(tx, ty, tz);
	*this = T * (*this);
}

void Mat44::rotateLocal(float rotX, float rotY, float rotZ)
{
	Mat44 rotation;
	rotation.setRotation(rotX, rotY, rotZ);
	*this = rotation * *this;
}

void Mat44::rotate(float rotX, float rotY, float rotZ)
{
	Mat44 rotation;
	rotation.setRotation(rotX, rotY, rotZ);

	(*this) = (*this) * rotation;
}

void Mat44::scale(const Vector3 & scale)
{
	m[0][0] *= scale.x;
	m[1][1] *= scale.y;
	m[2][2] *= scale.z;
}

void Mat44::replaceScale(const Vector3 & scale)
{
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void Mat44::setLookAt(const Vector3 & position, const Vector3 & targetPoint, const Vector3 & up)
{
	Vector3 front = (targetPoint - position);
	front.normalize();
	Vector3 right = front.cross(up);
	right.normalize();
	Vector3 top = right.cross(front);
	top.normalize();

	setIdentity();

	m[0][0] = right.x; m[0][1] = top.x; m[0][2] = -front.x;
	m[1][0] = right.y; m[1][1] = top.y; m[1][2] = -front.y;
	m[2][0] = right.z; m[2][1] = top.z; m[2][2] = -front.z;

	translateLocal(-position.x, -position.y, -position.z);
}

void Mat44::setPerspective(float fov, float aspect, float near, float far)
{
	setIdentity();

	float f = 1.0f / tan((float)DEGTORAD(fov) * 0.5f);

	m[0][0] = f / aspect;
	m[1][1] = f;
	m[2][2] = (far + near) / (near - far);
	m[2][3] = -1.0f;
	m[3][2] = 2.0f * (far * near) / (near - far);
	m[3][3] = 0.0f;
}

void Mat44::setOrthographic(float left, float right, float bottom, float top, float near, float far)
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

Mat44 Mat44::getRotationOnly()
{
	Vector3 scale = getScale();
	Mat44 rot;
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

Vector3 Mat44::rotateVector(const Vector3 & v)
{
	return (*this) * v;
}

Mat44 Mat44::operator * (float val)
{
	Mat44 result;
	for (size_t i = 0; i < 16; i++)
		result.values[i] = values[i] * val;

	return result;
}

Mat44 Mat44::operator * (const Mat44& other)
{
	Mat44 result;
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

Vector3 Mat44::operator * (const Vector3& other)
{
	Vector3 result;
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

Quaternion::Quaternion(const float X, const float Y, const float Z, const float W) : x(X), y(Y), z(Z), w(W)
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

void Quaternion::set(const float X, const float Y, const float Z, const float W)
{
	x = X;  y = Y; z = Z; w = W;
}

Quaternion::Quaternion(const float* q)
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

void Quaternion::slerp(const Quaternion& b, float t)
{
	//quaternion a(*this);
	*this = Qslerp(*this, b, t);
}

void Quaternion::slerp(const Quaternion& q2, float t, Quaternion &q3) const
{
	q3 = Qslerp(*this, q2, t);
}

void Quaternion::lerp(const Quaternion& b, float t)
{
	*this = Qlerp(*this, b, t);
}

void Quaternion::lerp(const Quaternion& q2, float t, Quaternion &q3) const
{
	q3 = Qlerp(*this, q2, t);
}

void Quaternion::setAxisAngle(const Vector3& axis, const float angle)
{
	float s;
	s = sinf(angle * 0.5f);

	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	w = cosf(angle * 0.5f);
}

Quaternion::Quaternion(const Vector3& axis, float angle)
{
	setAxisAngle(axis, angle);
}

void Quaternion::operator *= (const Quaternion &q)
{
	Quaternion quaternion = *this * q;
	*this = quaternion;
}

void Quaternion::operator*=(const Vector3& v)
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

Quaternion operator * (const Quaternion &q, const Vector3& v)
{
	return Quaternion
	(
		q.w*v.x + q.y*v.z - q.z*v.y,
		q.w*v.y + q.z*v.x - q.x*v.z,
		q.w*v.z + q.x*v.y - q.y*v.x,
		-(q.x*v.x + q.y*v.y + q.z*v.z)
	);
}

void Quaternion::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
}

Quaternion operator * (const Quaternion &q, float f)
{
	Quaternion q1;
	q1.x = q.x*f;
	q1.y = q.y*f;
	q1.z = q.z*f;
	q1.w = q.w*f;
	return q1;
}

Quaternion operator * (float f, const Quaternion &q)
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
	float len = length();
	len = 1.0f / len;

	x *= len;
	y *= len;
	z *= len;
	w *= len;
}

void Quaternion::computeMinimumRotation(const Vector3& rotateFrom, const Vector3& rotateTo)
{
	// Check if the vectors are valid.
	//rotateFrom.GetLength()==0.0f
	//rotateTo.GetLength()==0.0f

	Vector3 from(rotateFrom);
	from.normalize();
	Vector3 to(rotateTo);
	to.normalize();

	const float _dot = dot(from, to);
	Vector3 crossvec = cross(from, to);
	const float crosslen = (float)crossvec.mod();

	if (crosslen == 0.0f)
	{
		// Parallel vectors
		// Check if they are pointing in the same direction.
		if (_dot > 0.0f)
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		// Ok, so they are parallel and pointing in the opposite direction
		// of each other.
		else
		{
			// Try crossing with x axis.
			Vector3 t = cross(from, Vector3(1.0f, 0.0f, 0.0f));
			// If not ok, cross with y axis.
			if (t.mod() == 0.0f)
				cross(from, Vector3(0.0f, 1.0f, 0.0f));

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
		crossvec *= (float)sqrt(0.5f * fabs(1.0f - _dot));
		// The fabs() wrapping is to avoid problems when `dot' "underflows"
		// a tiny wee bit, which can lead to sqrt() returning NaN.
		x = crossvec.x;
		y = crossvec.y;
		z = crossvec.z,
		w = (float)sqrt(0.5 * fabs(1.0 + _dot));
	}
}

Quaternion SimpleRotation(const Vector3 &a, const Vector3 &b)
{
	Vector3 axis = cross(a, b);
	float _dot = dot(a, b);
	if (_dot < -1.0f + /*DOT_EPSILON*/0.001f) return Quaternion(0, 1, 0, 0);

	Quaternion result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (_dot + 1.0f) * 0.5f);
	result.normalize();
	//fast_normalize(&result);

	return result;
}


float Quaternion::length() const
{
	return sqrtf(w * w + x * x + y * y + z * z);
}

float Quaternion::squaredLength() const
{
	return w * w + x * x + y * y + z * z;
}

void Quaternion::toMatrix(Mat44& matrix) const
{
	/*
	If q is guaranteed to be a unit quaternion, s will always
	be 2.  In that case, this calculation can be optimized out.
	*/
	//const float	norm = x*x + y*y + z*z + w*w;
	//const float s = (norm > 0) ? 2/norm : 0;

	const float s = 2;

	/*
	Precalculate coordinate products
	*/
	const float xx = x * x * s;
	const float yy = y * y * s;
	const float zz = z * z * s;
	const float xy = x * y * s;
	const float xz = x * z * s;
	const float yz = y * z * s;
	const float wx = w * x * s;
	const float wy = w * y * s;
	const float wz = w * z * s;


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

Mat44 Quaternion::toMatrix() const
{
	Mat44 matrix;
	const float s = 2;

	const float xx = x * x * s;
	const float yy = y * y * s;
	const float zz = z * z * s;
	const float xy = x * y * s;
	const float xz = x * z * s;
	const float yz = y * z * s;
	const float wx = w * x * s;
	const float wy = w * y * s;
	const float wz = w * z * s;

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

void Quaternion::fromMatrix(const Mat44 & mat)
{
	const float trace = 1.0f + mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

	if (trace > 0.00001f)
	{
		const float s = sqrt(trace) * 2;
		x = (mat.m[2][1] - mat.m[1][2]) / s;
		y = (mat.m[0][2] - mat.m[2][0]) / s;
		z = (mat.m[1][0] - mat.m[0][1]) / s;
		w = s / 4;
		return;
	}
	else if (mat.m[0][0] > mat.m[1][1] && mat.m[0][0] > mat.m[2][2])
	{
		const float s = sqrt(1.0f + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]) * 2;

		x = s / 4;
		y = (mat.m[1][0] + mat.m[0][1]) / s;
		z = (mat.m[0][2] + mat.m[2][0]) / s;
		w = (mat.m[2][1] - mat.m[1][2]) / s;
		return;
	}
	else if (mat.m[1][1] > mat.m[2][2])
	{
		const float s = sqrt(1.0f + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]) * 2;

		x = (mat.m[1][0] + mat.m[0][1]) / s;
		y = s / 4;
		z = (mat.m[2][1] + mat.m[1][2]) / s;
		w = (mat.m[0][2] - mat.m[2][0]) / s;
		return;
	}
	else
	{
		const float s = sqrt(1.0f + mat.m[2][2] - mat.m[0][0] - mat.m[1][1]) * 2;

		x = (mat.m[0][2] + mat.m[2][0]) / s;
		y = (mat.m[2][1] + mat.m[1][2]) / s;
		z = s / 4;
		w = (mat.m[1][0] - mat.m[0][1]) / s;
		return;
	}
}

float DotProduct(const Quaternion& q1, const Quaternion& q2)
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
	float a = static_cast<float>(acos(q.w));
	float sina = static_cast<float>(sin(a));
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
	float a = static_cast<float>(sqrt(q.x*q.x + q.y*q.y + q.z*q.z));
	float sina = static_cast<float>(sin(a));
	float cosa = static_cast<float>(cos(a));
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
Linear interpolation between two quaternions
*/
Quaternion Qlerp(const Quaternion &q1, const Quaternion &q2, float t)
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

Quaternion Qslerp(const Quaternion &q1, const Quaternion &q2, float t)
{
	Quaternion q3;
	float dot = DotProduct(q1, q2);

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
		float angle = static_cast<float>(acosf(dot));
		float sina, sinat, sinaomt;
		sina = sinf(angle);
		sinat = sinf(angle*t);
		sinaomt = sinf(angle*(1 - t));
		return (q1*sinaomt + q3 * sinat) * (1.0f / sina);
	}

	//if the angle is small, use linear interpolation

	else
	{
		return Qlerp(q1, q3, t);
	}



	/*
	float fCos = DotProduct(q1,q2);
	if (fCos > 1)
	fCos = 1;
	if (fCos < -1)
	fCos = -1;
	float fAngle = acosf(fCos);
	if ( fabs(fAngle) < 0.0001 )
	{
	return q1;
	}
	float fSin = sinf(fAngle);
	float fInvSin = 1.0f/fSin;
	float fCoeff0 = sinf((1.0f-fT)*fAngle)*fInvSin;
	float fCoeff1 = sinf(fT*fAngle)*fInvSin;
	return fCoeff0*q1 + fCoeff1*q2;
	*/
}


/*
Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
*/

Quaternion& Quaternion::operator -()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;

	return *this;
}

void Quaternion::getAxisAngle(Vector3 &v, float &angle) const
{
	angle = 2.0f * acosf(w);
	float factor = 1.0f / sqrtf(1 - w * w);
	v.x = x * factor;
	v.y = y * factor;
	v.z = z * factor;
}

Vector3 Quaternion::rotate(const Vector3& v) const
{
	Vector3 ret;
	Quaternion temp(-x, -y, -z, w);
	temp *= v;
	temp *= (*this);

	ret.x = temp.x;
	ret.y = temp.y;
	ret.z = temp.z;

	return ret;
}

void Quaternion::toEulerAngles(Vector3 &euler) const
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float matrix[3][3];
	float cx, sx;
	float cy, sy;
	float cz, sz;

	const float y2 = y * y;
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
	//euler->y = (yr * 180.0f) / (float)M_PI;

	// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270 
	// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
	if (sy != 1.0f && sy != -1.0f)
	{
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		//euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)M_PI;	// RAD TO DEG
		euler.x = atan2f(sx, cx);

		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		//euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)M_PI;	// RAD TO DEG
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
		//euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)M_PI;	// RAD TO DEG
		euler.x = atan2f(sx, cx);

		cz = 1.0f;
		sz = 0.0f;
		//euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)M_PI;	// RAD TO DEG
		euler.z = atan2f(sz, cz);
	}
}

inline Quaternion Quaternion::Euler(const Vector3 & eulerAngles)
{
	float yaw = eulerAngles.x;
	float pitch = eulerAngles.y;
	float roll = eulerAngles.z;
	float rollOver2 = roll * 0.5f;
	float sinRollOver2 = (float)sin((float)rollOver2);
	float cosRollOver2 = (float)cos((float)rollOver2);
	float pitchOver2 = pitch * 0.5f;
	float sinPitchOver2 = (float)sin((float)pitchOver2);
	float cosPitchOver2 = (float)cos((float)pitchOver2);
	float yawOver2 = yaw * 0.5f;
	float sinYawOver2 = (float)sin((float)yawOver2);
	float cosYawOver2 = (float)cos((float)yawOver2);
	Quaternion result;
	result.x = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
	result.y = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
	result.z = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
	result.w = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
	return result;
}


void Quaternion::setAxisAngle(float ax, float ay, float az, float angle)
{
	const float halfAngle = angle * 0.5f;
	const float s = sinf(halfAngle);

	w = cosf(halfAngle);
	x = ax * s;
	y = ay * s;
	z = az * s;

}
#pragma endregion