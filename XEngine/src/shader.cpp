#include "shader.h"
#include "utils.h"
#include <string.h>

void Shader::Load(const char* vsPath, const char* psPath)
{
	FILE* vsFile;
	FILE* psFile;
	std::string fullpathVS = Path::GetPath(vsPath);
	std::string fullpathPS = Path::GetPath(psPath);
	fopen_s(&vsFile, fullpathVS.c_str(), "r");
	fopen_s(&psFile, fullpathPS.c_str(), "r");
	if (vsFile == NULL || psFile == NULL)
	{
		if(vsFile) fclose(vsFile);
		if(psFile) fclose(psFile);

		ASSERT(false);
		LOGERROR("Error loading shader files at: %s, %s", fullpathVS.c_str(), fullpathPS.c_str());
		return;
	}
	vsFilePath = Copy(vsPath);
	psFilePath = Copy(psPath);

	std::string fullvsContent;
	std::string fullpsContent;
	ReadAllFile(fullvsContent, vsFile);
	ReadAllFile(fullpsContent, psFile);
	fclose(vsFile);
	fclose(psFile);
	Init(fullvsContent.c_str(), fullpsContent.c_str());
}

void Shader::Init(const char * vertex, const char * pixel)
{
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &vertex, NULL);
	glCompileShader(vertex_id);

	int  v_success;
	char infoLog[512];
	glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &v_success);

	if (!v_success)
	{
		glGetShaderInfoLog(vertex_id, 512, NULL, infoLog);
		LOGERROR("Error compiling vertex shader: %s \ninfoLog: %s", vertex, infoLog);
	}
	ASSERT(v_success);

	unsigned int fragment_id;
	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &pixel, NULL);
	glCompileShader(fragment_id);

	int f_success;
	glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &f_success);
	if (!f_success)
	{
		glGetShaderInfoLog(fragment_id, 512, NULL, infoLog);
		LOGERROR("Error compiling fragment shader: %s \ninfoLog: %s", pixel, infoLog);
	}
	ASSERT(f_success);

	
	id = glCreateProgram();
	glAttachShader(id, vertex_id);
	glAttachShader(id, fragment_id);
	glLinkProgram(id);

	int final_sucess;
	glGetProgramiv(id, GL_LINK_STATUS, &final_sucess);
	if (!final_sucess)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		LOGERROR("Error linking shader: %s \ninfoLog: %s", vertex, infoLog);
	}
	ASSERT(final_sucess);
}

void Shader::Destroy()
{
	free(vsFilePath);
	free(psFilePath);
	glDetachShader(id, vertex_id);
	glDetachShader(id, fragment_id);
	glDeleteProgram(id);
}

void Shader::SetInt(const std::string & parameterName, const int & value) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniform1i(uniformLoc, value);
}

void Shader::SetBool(const std::string & parameterName, bool b) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniform1i(uniformLoc, b);
}

void Shader::SetFloat(const std::string & parameterName, const float & value) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniform1f(uniformLoc, value);
}

void Shader::SetVector2(const std::string & parameterName, const Vector2 & value) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniform2f(uniformLoc, value.x, value.y);
}

void Shader::SetVector3(const std::string & parameterName, const Vector3 & value) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniform3f(uniformLoc, value.x, value.y, value.z);
}

void Shader::SetMat33(const std::string & parameterName, const Mat33 & value) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, value.M);
}

void Shader::SetMat44(const std::string & parameterName, const Mat44 & value) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value.values);
}

void Shader::SetMat44Array(const std::string & parameterName, const Mat44 & value, int nummats) const
{
	int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
	ASSERT(uniformLoc != -1);
	glUniformMatrix4fv(uniformLoc, nummats, GL_FALSE, value.values);
}

void Shader::SetTexture(unsigned int id) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Shader::enable() const
{
	glUseProgram(id);
}

void Shader::disable() const
{
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
}
