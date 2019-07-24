#pragma once
#include "opengl_defines.h"
#include <cassert>
#include "math.h"

//const char *basic_vertex_shader =
//"layout (location = 0) in vec3 aPos; "
//"void main()"
//"{"
//"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
//"}";
//
//const char* basic_fragment_shader =
//"out vec4 FragColor;"
//"void main()"
//"{"
//"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
//"}";

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

const char* basic_vertex_shader_texture =
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

const char* basic_fragment_shader_texture =
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

class Shader 
{
public:
	unsigned int id;
	unsigned int vertex_id;
	unsigned int fragment_id;

	//@TODO: make constructor for shader files
	Shader(const char* vertex, const char* pixel)
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
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
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
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
