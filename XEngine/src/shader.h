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

	std::string vsFilePath;
	std::string psFilePath;

	void Load(const std::string& vsPath, const std::string& psPath);
	void Init(const char* vertex, const char* pixel);

	void SetInt(const std::string& parameterName, const int& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniform1i(uniformLoc, value);
	}

	void SetBool(const std::string& parameterName, bool b) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniform1i(uniformLoc, b);
	}

	void SetFloat(const std::string& parameterName, const float& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniform1f(uniformLoc, value);
	}

	void SetVector2(const std::string& parameterName, const Vector2& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniform2f(uniformLoc, value.x, value.y);
	}

	void SetVector3(const std::string& parameterName, const Vector3& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniform3f(uniformLoc, value.x, value.y, value.z);
	}

	void SetMat33(const std::string& parameterName, const Mat33& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, value.M);
	}

	void SetMat44(const std::string& parameterName, const Mat44& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value.values);
	}

	void SetMat44Array(const std::string& parameterName, const Mat44& value, int nummats) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		ASSERT(uniformLoc != -1);
		glUniformMatrix4fv(uniformLoc, nummats, GL_FALSE, value.values);
	}
	
	void SetTexture(unsigned int id) const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void enable() const
	{
		glUseProgram(id);
	}

	void disable() const 
	{
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(0);
	}
};
