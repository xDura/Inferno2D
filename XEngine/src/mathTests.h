#ifndef MATHTESTS
#define MATHTESTS
#include "math.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include "mathTests.h"
#include "platform.h"

class MathTests
{
public:

	static void LogResult(const std::string & logText, bool success)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		int color = 4;
		if (success)
			color = 2;

		SetConsoleTextAttribute(hConsole, color);
		std::cout << logText << std::endl;

		//set console color to white
		SetConsoleTextAttribute(hConsole, 7);
	}

	static void testAngles() 
	{
		float angleInDegrees = 180.0;
		float angleInRad = (float)DEGTORAD(180.0);
	}

	static void testVector2()
	{
		std::cout << "Vector2Tests" << std::endl;
		//vector2 zero
		Vector2 v = Vector2::zero;
		bool result = (v.x == 0.0 && v.y == 0.0);
		LogResult("Vector2::zero is 0,0", result);

		//constructors
		//Vector2() { x = y = 0; };
		v = Vector2();
		result = (v.x == 0.0 && v.y == 0.0);
		LogResult("empty constructor set to zero", result);

		//Vector2(float a_x, float a_y) { x = a_x; y = a_y; };
		float a = 1.0;
		float b = 2.0;
		v = Vector2(a, b);
		result = (v.x == a && v.y == b);
		LogResult("double float constructor", result);

		//Vector2(float value) { x = y = value; };
		//v = Vector2(a);
		//result = (v.x == a && v.y == a);
		//LogResult("one float constructor", result);

		/*Vector2(float values[2])
		{
		x = values[0];
		y = values[1];
		}*/
		float f[2];
		f[0] = 1.0;
		f[1] = 2.0;

		v = Vector2(f);
		result = (v.x == 1.0 && v.y == 2.0);
		LogResult("float array constructor", result);


		//float mod() const;
		v.x = 2.0;
		v.y = 0.0;
		result = (v.mod() == 2.0);
		LogResult("mod", result);

		v.x = 1.0;
		v.y = 1.0;
		result = (abs((v.mod() - 1.4142)) < 0.001);
		LogResult("mod", result);


		//void normalize();
		v.y = 2;
		v.x = 20;
		v.normalize();
		result = (v.mod() == 1.0);
		LogResult("normalize", result);

		//Vector2 normalized() const;
		v.y = 2;
		v.x = 20;
		result = (v.normalized().mod() == 1.0);
		LogResult("normalized return", result);

		//float dot(const Vector2& other);
		Vector2 v2 = Vector2(2.0, 1.5);
		v.x = 1.0;
		v.y = 2.0;
		float dot = v.dot(v2);
		LogResult("dot product", dot == 5.0);

		//void set(float a_x, float a_y) { x = a_x; y = a_y; };
		v.set(4, 1.5);
		result = (v.x == 4 && v.y == 1.5);
		LogResult("set vector values method", result);

		//float distance(const Vector2 & other) const;
		v.x = 1.0;
		v.y = 2.0;
		v2.x = 2.0;
		v2.y = 2.0;
		result = (v.distance(v2) == 1.0);
		LogResult("distance", result);

		//static Vector2 random(float range);
		std::vector<Vector2> vects = std::vector<Vector2>();
		vects.push_back(Vector2::random(2.0));
		vects.push_back(Vector2::random(2.0));
		vects.push_back(Vector2::random(2.0));
		vects.push_back(Vector2::random(2.0));
		vects.push_back(Vector2::random(2.0));
		result = true;
		for (unsigned int i = 0; i < vects.size(); i++)
		{
			if (vects[i].x < -2.0 || vects[i].x > 2.0 || vects[i].y < -2.0 || vects[i].y > 2.0)
				result = false;
		}

		LogResult("randoms in range", result);
		vects.clear();

		//Vector2 operator + (const Vector2 & op) const;
		v.x = 1.0;
		v.y = 2.0;
		v = v + v;
		result = (v.x == 2.0 && v.y == 4.0);
		LogResult("operator vector + vector", result);

		//Vector2 operator - (const Vector2 & op) const;
		v.x = 1.0;
		v.y = 2.0;
		v = v - v;
		result = (v.x == 0.0 && v.y == 0.0);
		LogResult("operator vector - vector", result);

		//void operator *= (float op);
		v.x = 1.0;
		v.y = 2.0;
		v *= 3.0;
		result = (v.x == 3.0 && v.y == 6.0);
		LogResult("operator vector *= float ", result);

		//void operator /= (float op);
		v.x = 3.0;
		v.y = 3.0;
		v /= 3.0;
		result = (v.x == 1.0 && v.y == 1.0);
		LogResult("operator vector /= float ", result);

		//Vector2 operator * (float op) const;
		v.x = 3.0;
		v.y = 3.0;
		v = v * 3;
		result = (v.x == 9.0 && v.y == 9.0);
		LogResult("operator vector * float ", result);

		//Vector2 operator / (float op) const;
		v.x = 3.0;
		v.y = 3.0;
		v = v / 3;
		result = (v.x == 1.0 && v.y == 1.0);
		LogResult("operator vector / float ", result);

		//equals
		bool result2;
		v.x = 1.0;
		v.y = 2.0;
		v2.x = 1.0;
		v2.y = 2.0;
		result = (v.equals(v2));
		v2.x = 2.0;
		result2 = (v.equals(v2));
		LogResult("equals", result && !result2);

		//equals with offset
		float f_test = 0.01f;
		v.x = 1.0;
		v.y = 2.0;
		v2.x = 1.001f;
		v2.y = 1.999f;
		result = (v.equals_offset(v2, f_test));
		result2 = v.equals_offset(v2, 0.0001f);
		LogResult("equals with offset", result && !result2);
	}

	static void testMat44()
	{
		Vector3 desiredScale(1.0f, 2.0f, 2.0f);
		Vector3 targetScale;
		Mat44 mat;
		mat.setScale(desiredScale);
		Mat44 translationMat;
		translationMat.setTranslation(Vector3(1.0f, 1.0f, 2.0f));
		Mat44 rotationMat;
		rotationMat.setRotation(10.0f, 20.0f, 2.0f);

		mat = mat * rotationMat * translationMat;
		targetScale = mat.getScale();

		LogResult("mat44 getScale", desiredScale.equals(targetScale));

		Mat44 resultRotation;
		resultRotation = mat.getRotationOnly();
	}

	static void testQuaternion()
	{
		Mat44 rotationMat;
		rotationMat.setRotation(10.0f, 20.0f, 2.0f);

		Quaternion rot;
		rot.fromMatrix(rotationMat);
		Mat44 resultRotMat = rot.toMatrix();

		Logger::Log(rotationMat.toString().c_str());
		Logger::Log("*****");
		Logger::Log(resultRotMat.toString().c_str());
	}
};

#endif
