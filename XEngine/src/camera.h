#pragma once
#include "math.h"

//@TODO
enum PROJECTION_MODE
{
	PERSPECTIVE,
	ORTHO,
};
//
//struct PERSPECTIVE_DATA
//{
//	float fov;
//	float aspect;
//	float near;
//	float far;
//};

class Camera
{

public:
	Vec3f pos;
	Vec3f target;
	Vec3f up;

	Vec3f Forward() { return Vec3f(-viewMat.m[0][2], -viewMat.m[1][2], -viewMat.m[2][2]); }
	Vec3f Right() { return Vec3f(viewMat.m[0][0], viewMat.m[1][0], viewMat.m[2][0]); }
	Vec3f Up() { return Vec3f(viewMat.m[0][1], viewMat.m[1][1], viewMat.m[2][1]); }
	
	Mat44f viewMat;
	Mat44f projectionMat;
	Mat44f viewProjectionMat;

	PROJECTION_MODE mode;

	Camera(const Vec3f& _pos, const Vec3f& targetPoint, const Vec3f& _up, float fov, float aspect, float _near, float _far)
	{
		LookAt(_pos, targetPoint, _up);
		SetPerspective(fov, aspect, _near, _far);
	}

	Camera(const Vec3f& _pos, const Vec3f& targetPoint, const Vec3f& _up, float left, float right, float bottom, float top, float _near, float _far)
	{
		LookAt(_pos, targetPoint, _up);
		SetOrthographic(left, right, bottom, top, _near, _far);
	}

	void LookAt(const Vec3f& _pos, const Vec3f& targetPoint, const Vec3f& _up)
	{
		pos = _pos;
		target = targetPoint;
		up = _up;
		viewMat.setLookAt(pos, target, up);
		UpdateViewProjection();
	}

	void Move(const Vec3f& delta)
	{
		pos = pos - delta;
		target = target - delta;
		UpdateViewMatrix();
	}

	void MoveLocal(const Vec3f& delta)
	{
		Vec3f localDelta = getLocalVector(delta);
		Move(localDelta);
	}

	void RotateLocal(float angleX, float angleY, float angleZ)
	{
		Mat44f R;
		R.setRotation(angleX, angleY, angleZ);
		Vec3f new_front = R * (target - pos);
		target = pos + new_front;
		UpdateViewMatrix();
	}

	void RotateLocal(float angle, const Vec3f& axis)
	{
		Mat44f R;
		R.setRotation(angle, axis);
		Vec3f new_front = R * (target - pos);
		target = pos + new_front;
		UpdateViewMatrix();
	}

	Vec3f getLocalVector(const Vec3f& v)
	{
		Mat44f iV = viewMat;
		if (iV.inverse() == false)
			std::cout << "Matrix Inverse error" << std::endl;
		Vec3f result = iV.rotateVector(v);
		return result;
	}

	void SetPerspective(float fov, float aspect, float _near, float _far)
	{
		mode = PROJECTION_MODE::PERSPECTIVE;
		projectionMat.setPerspective(fov, aspect, _near, _far);
		UpdateViewProjection();
	}

	void SetOrthographic(float left, float right, float bottom, float top, float _near, float _far)
	{
		mode = PROJECTION_MODE::ORTHO;
		projectionMat.setOrthographic(left, right, bottom, top, _near, _far);
		UpdateViewProjection();
	}

	void UpdateViewMatrix()
	{
		LookAt(pos, target, up);
		viewProjectionMat = viewMat * projectionMat;
	}

	void UpdateViewProjection()
	{
		viewProjectionMat = viewMat * projectionMat;
	}
};