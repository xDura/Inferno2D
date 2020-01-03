#pragma once
#include "opengl_defines.h"
#include <cassert>
#include "math.h"
#include <iostream>
#include "platform.h"

class Shader 
{
public:
	unsigned int id;
	unsigned int vertex_id;
	unsigned int fragment_id;

	char* vsFilePath;
	char* psFilePath;

	void Load(const char* vsPath, const char* psPath);
	void Init(const char* vertex, const char* pixel);
	void Destroy();

	void SetInt(const std::string& parameterName, const int& value) const;
	void SetBool(const std::string& parameterName, bool b) const;
	void SetFloat(const std::string& parameterName, const float& value) const;
	void SetVector2(const std::string& parameterName, const Vec2f& value) const;
	void SetVector3(const std::string& parameterName, const Vec3f& value) const;
	void SetMat33(const std::string& parameterName, const Mat33f& value) const;
	void SetMat44(const std::string& parameterName, const Mat44f& value) const;
	void SetMat44Array(const std::string& parameterName, const Mat44f& value, int nummats) const;
	void SetTexture(unsigned int id) const;
	void enable() const;
	void disable() const;
};
