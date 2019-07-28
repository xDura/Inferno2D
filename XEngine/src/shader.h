#pragma once
#include "opengl_defines.h"
#include <cassert>
#include "math.h"
#include <iostream>
#include "platform.h"

const char* basic_line_shader_vertex =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;"
"layout(location = 1) in vec3 aColor;"
""
"out vec3 v_color;"
"uniform mat4 MVP;"
""
"void main()"
"{"
"	gl_Position = MVP * vec4(aPos, 1.0);"
"	v_color = aColor;"
"}";

const char* basic_line_shader_fragment =
"#version 330 core\n"
"out vec4 FragColor;"
""
"in vec3 v_color;"
""
"void main()"
"{"
"	FragColor = vec4(v_color, 1.0);"
"}";

//const char* basic_vertex_shader_texture =
//"#version 330 core\n"
//"layout (location = 0) in vec3 aPos;"
//"layout (location = 1) in vec2 aTexCoord;"
//"out vec2 TexCoord;"
//"uniform mat4 M;"
//""
//"void main()"
//"{"
//"	gl_Position = M * vec4(aPos, 1.0);"
//"	TexCoord = aTexCoord;"
//"}";
//
//const char* basic_fragment_shader_texture =
//"#version 330 core\n"
//"out vec4 FragColor;"
//""
//"in vec2 TexCoord;"
//""
//"uniform sampler2D texture1;"
//""
//"void main()"
//"{"
//"	FragColor = texture2D(texture1, TexCoord);"
//"}";

const char* skinned_mesh_vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 aTexCoord;"
"layout (location = 2) in vec4 aWeights;"
"layout (location = 3) in vec4 aBoneIds;"
"out vec2 TexCoord;"
"uniform mat4 M;"
"uniform mat4 boneMats[80];"
""
"void main()"
"{"
""
"	mat4 boneTransform = boneMats[int(aBoneIds[0])] * aWeights[0];"
"	boneTransform += boneMats[int(aBoneIds[1])] * aWeights[1];"
"	boneTransform += boneMats[int(aBoneIds[2])] * aWeights[2];"
"	boneTransform += boneMats[int(aBoneIds[3])] * aWeights[3];"
""
"	TexCoord = aTexCoord;"
"	gl_Position = M * boneTransform * vec4(aPos, 1.0);"
"}";


//Shader to debug weights
//"uniform float desiredBoneID;"
//"   if(aBoneIds.x == desiredBoneID)"
//"		gl_Position = M * vec4(aPos, 1.0);"
//"	if(aBoneIds.y == desiredBoneID)"
//"		gl_Position = M * vec4(aPos, 1.0);"
//"	if(aBoneIds.z == desiredBoneID)"
//"		gl_Position = M * vec4(aPos, 1.0);"
//"	if(aBoneIds.w == desiredBoneID)"
//"		gl_Position = M * vec4(aPos, 1.0);"

const char* skinned_mesh_fragment_shader =
"#version 330 core\n"
"out vec4 FragColor;"
""
"in vec2 TexCoord;"
""
"uniform sampler2D texture1;"
""
"void main()"
"{"
"	FragColor = texture2D(texture1, TexCoord);"
"}";

const char* skinned_mesh_vertex_shader_color =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 aTexCoord;"
"layout (location = 2) in vec4 aWeights;"
"layout (location = 3) in vec4 aBoneIds;"
"layout (location = 4) in vec3 aColor;"
"out vec3 vertColor;"
"out vec2 TexCoord;"
"uniform mat4 M;"
"uniform mat4 boneMats[80];"
""
"void main()"
"{"
""
"	mat4 boneTransform = boneMats[int(aBoneIds[0])] * aWeights[0];"
"	boneTransform += boneMats[int(aBoneIds[1])] * aWeights[1];"
"	boneTransform += boneMats[int(aBoneIds[2])] * aWeights[2];"
"	boneTransform += boneMats[int(aBoneIds[3])] * aWeights[3];"
"	vertColor = aColor;"
"	TexCoord = aTexCoord;"
"	gl_Position = M * boneTransform * vec4(aPos, 1.0);"
"}";


const char* skinned_mesh_fragment_shader_color =
"#version 330 core\n"
"out vec4 FragColor;"
"in vec3 vertColor;"
"in vec2 TexCoord;"
""
"uniform sampler2D texture1;"
""
"void main()"
"{"
"	FragColor = vec4(vertColor, 1.0f);"
"}";

const char* basic_vertex_shader_texture_tiled =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 aTexCoord;"
"out vec2 TexCoord;"
"uniform mat4 M;"
""
"void main()"
"{"
"	gl_Position = M * vec4(aPos, 1.0);"
"	TexCoord = aTexCoord;"
"}";
//
//const char* basic_fragment_shader_texture_tiled =
//"#version 330 core\n"
//"out vec4 FragColor;"
//""
//"in vec2 TexCoord;"
//"uniform vec2 tileSize;"
//"uniform sampler2D texture1;"
//"uniform int tileIndex;"
//"uniform bool invertX;"
//""
//"void main()"
//"{"
//"	float texCoordX = TexCoord.x;"
//"	if(invertX)"
//"		texCoordX = 1.0f - TexCoord.x;"
//"	vec2 newTexCoord = vec2(0.0, 0.0);"
//"	vec2 normalizedTileSize = vec2(1.0 / max(1.0, tileSize.x), 1.0 / max(1.0, tileSize.y));"
//"	int tileRow = tileIndex / int(tileSize.x);"
//"	int tileCol = tileIndex - ((tileRow - 1) * int(tileSize.x));"
//"	newTexCoord.x = (texCoordX * normalizedTileSize.x) + (tileCol * normalizedTileSize.x);"
//"	newTexCoord.y = (TexCoord.y * normalizedTileSize.y) + (((tileSize.y - 1.0f) - float(tileRow))* normalizedTileSize.y);"
//""
//"	FragColor = texture2D(texture1, newTexCoord);"
//"}";

class Shader 
{
public:
	unsigned int id;
	unsigned int vertex_id;
	unsigned int fragment_id;

	std::string vsFilePath;
	std::string psFilePath;

	void Load(const std::string& vsPath, const std::string& psPath)
	{
		FILE* vsFile;
		FILE* psFile;
		std::string fullpathVS = Path::GetPath(vsPath);
		std::string fullpathPS = Path::GetPath(psPath);
		fopen_s(&vsFile, fullpathVS.c_str(), "r");
		fopen_s(&psFile, fullpathPS.c_str(), "r");
		if (vsFile == NULL || psFile == NULL)
		{
			ASSERT(false);
			LOGERROR("Error loading shader files at: %s, %s", fullpathVS.c_str(), fullpathPS.c_str());
			return;
		}
		vsFilePath = vsPath;
		psFilePath = psPath;

		std::string fullvsContent;
		std::string fullpsContent;
		readAllFile(fullvsContent, vsFile);
		readAllFile(fullpsContent, psFile);
		Init(fullvsContent.c_str(), fullpsContent.c_str());
	}

	void Init(const char* vertex, const char* pixel)
	{
		//compile vert shader
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
		assert(v_success);
		//***

		//compile fragment
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
		assert(f_success);
		//****

		//create the shader
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
		assert(final_sucess);
		//*****
	}

	void SetInt(const std::string& parameterName, const int& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniform1i(uniformLoc, value);
	}

	void SetBool(const std::string& parameterName, bool b) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniform1i(uniformLoc, b);
	}

	void SetFloat(const std::string& parameterName, const float& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniform1f(uniformLoc, value);
	}

	void SetVector2(const std::string& parameterName, const Vector2& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniform2f(uniformLoc, value.x, value.y);
	}

	void SetVector3(const std::string& parameterName, const Vector3& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniform3f(uniformLoc, value.x, value.y, value.z);
	}

	void SetMat33(const std::string& parameterName, const Mat33& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, value.M);
	}

	void SetMat44(const std::string& parameterName, const Mat44& value) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value.values);
	}

	void SetMat44Array(const std::string& parameterName, const Mat44& value, int nummats) const
	{
		int uniformLoc = glGetUniformLocation(id, parameterName.c_str());
		assert(uniformLoc != -1);
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
