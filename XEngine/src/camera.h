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
	Vector3 pos;
	Vector3 target;
	Vector3 up;

	Vector3 Forward() { return Vector3(-viewMat.m[0][2], -viewMat.m[1][2], -viewMat.m[2][2]); }
	Vector3 Right() { return Vector3(viewMat.m[0][0], viewMat.m[1][0], viewMat.m[2][0]); }
	Vector3 Up() { return Vector3(viewMat.m[0][1], viewMat.m[1][1], viewMat.m[2][1]); }
	
	Mat44 viewMat;
	Mat44 projectionMat;
	Mat44 viewProjectionMat;

	PROJECTION_MODE mode;

	Camera(const Vector3& _pos, const Vector3& targetPoint, const Vector3& _up, float fov, float aspect, float _near, float _far)
	{
		LookAt(_pos, targetPoint, _up);
		SetPerspective(fov, aspect, _near, _far);
	}

	Camera(const Vector3& _pos, const Vector3& targetPoint, const Vector3& _up, float left, float right, float bottom, float top, float _near, float _far)
	{
		LookAt(_pos, targetPoint, _up);
		SetOrthographic(left, right, bottom, top, _near, _far);
	}

	void LookAt(const Vector3& _pos, const Vector3& targetPoint, const Vector3& _up)
	{
		pos = _pos;
		target = targetPoint;
		up = _up;
		viewMat.setLookAt(pos, target, up);
		UpdateViewProjection();
	}

	void Move(const Vector3& delta)
	{
		pos = pos - delta;
		target = target - delta;
		UpdateViewMatrix();
	}

	void MoveLocal(const Vector3& delta)
	{
		Vector3 localDelta = getLocalVector(delta);
		Move(localDelta);
	}

	void RotateLocal(float angleX, float angleY, float angleZ)
	{
		Mat44 R;
		R.setRotation(angleX, angleY, angleZ);
		Vector3 new_front = R * (target - pos);
		target = pos + new_front;
		UpdateViewMatrix();
	}

	void RotateLocal(float angle, const Vector3& axis)
	{
		Mat44 R;
		R.setRotation(angle, axis);
		Vector3 new_front = R * (target - pos);
		target = pos + new_front;
		UpdateViewMatrix();
	}

	Vector3 getLocalVector(const Vector3& v)
	{
		Mat44 iV = viewMat;
		if (iV.inverse() == false)
			std::cout << "Matrix Inverse error" << std::endl;
		Vector3 result = iV.rotateVector(v);
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