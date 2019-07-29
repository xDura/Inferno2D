#include "shader.h"
#include "utils.h"

void Shader::Load(const std::string & vsPath, const std::string & psPath)
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
	ReadAllFile(fullvsContent, vsFile);
	ReadAllFile(fullpsContent, psFile);
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
	assert(v_success);

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
}
