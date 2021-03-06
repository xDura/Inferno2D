#pragma once
#include "utils.h"
#include "External/STB/stb_easy_font.h"
#include "platform.h"
#include "iostream"
#include "string"

void setGreatestWeights(Vec4f & weights, Vec4f & boneIds, f32 newWeight, s32 newBoneId)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (newWeight <= weights.values[i]) continue;

		weights.values[i] = newWeight;
		boneIds.values[i] = (float)newBoneId;
	}
}

bool debugDrawText(f32 x, f32 y, std::string text, Vec3f c, f32 scale, s32 window_width, s32 window_height)
{
	static char buffer[99999]; // ~500 chars
	s32 num_quads;

	if (scale == 0)
		return true;

	x /= scale;
	y /= scale;

	num_quads = stb_easy_font_print(x, y, (char*)(text.c_str()), NULL, buffer, sizeof(buffer));

	Mat44f projection_matrix;
	projection_matrix.setOrthographic(0, window_width / scale, window_height / scale, 0, -1, 1);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(Mat44f().values);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(projection_matrix.values);

	glColor3f(c.x, c.y, c.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 16, buffer);
	glDrawArrays(GL_QUADS, 0, num_quads * 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

bool serializeString(std::string & s, FILE * file)
{
	if (file == NULL) return false;

	s32 size = (int)s.size();
	fwrite(&size, sizeof(int), 1, file);
	if (size != 0)
	{

		s32 totalWrites = (int)fwrite(&s[0], sizeof(char), size, file);
		return totalWrites == size;
	}
	return true;
}

bool deserializeString(std::string & s, FILE * file)
{
	if (file == NULL) return false;

	s32 size = 0;
	fread(&size, sizeof(int), 1, file);
	s.resize(size, 'c');
	if (size != 0)
	{
		s32 totalReads = (int)fread(&s[0], sizeof(char), size, file);
		return totalReads == size;
	}
	return true;
}

bool serializeCharArray(const char * string, FILE * file)
{
	if (file == NULL) return false;

	s32 size = (int)strlen(string);
	fwrite(&size, sizeof(int), 1, file);
	if (size != 0)
	{

		s32 totalWrites = (int)fwrite(&string, sizeof(char), size, file);
		return totalWrites == size;
	}
	return true;
}

bool deserializeCharArray(char * string, FILE * file)
{
	if (file == NULL) return false;

	s32 size = 0;
	fread(&size, sizeof(int), 1, file);
	string = (char*)malloc(size * sizeof(char));
	if (size != 0)
	{
		s32 totalReads = (int)fread(string, sizeof(char), size, file);
		return totalReads == size;
	}
	return true;
}

void readAllFile(std::string& content, FILE * file)
{
	s32 count;
	fseek(file, 0, SEEK_END);
	count = ftell(file);
	rewind(file);

	content.resize(count);
	if (count > 0)
	{
		count = (int)fread(&content[0], sizeof(char), count, file);
	}
}

char * copySTR(const char * source)
{
	char* new_string = new char[strlen(source) + 1];
	memcpy(new_string, source, strlen(source) + 1);
	return new_string;
}

std::string getGPUStats()
{
	s32 nTotalMemoryInKB = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &nTotalMemoryInKB);
	s32 nCurAvailMemoryInKB = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &nCurAvailMemoryInKB);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) //unsupported feature by driver
	{
		LOGERROR("Driver error, trying to getting VRAM info");
		nTotalMemoryInKB = 0;
		nCurAvailMemoryInKB = 0;
	}

	/*std::string str = "FPS: " + std::to_string(Game::instance->fps) + " DCS: " + std::to_string(Mesh::num_meshes_rendered) + " Tris: " + std::to_string(long(Mesh::num_triangles_rendered * 0.001)) + */
	std::string str = "Ks  VRAM: " + std::to_string(int(nCurAvailMemoryInKB * 0.001)) + "MBs / " + std::to_string(int(nTotalMemoryInKB * 0.001)) + "MBs";
	//Mesh::num_meshes_rendered = 0;
	//Mesh::num_triangles_rendered = 0;
	return str;
}
