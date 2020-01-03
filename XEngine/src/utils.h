#ifndef UTILS_H
#define UTILS_H

#include "opengl_defines.h"
#include "math.h"
#include "vector"
#include <stdio.h>

#define BIT(bit) 1 << bit;

//80bit string
typedef struct
{
	char str[10];
} String10;

//160bit string
typedef struct
{
	char str[20];
} String20;

//240bit string
typedef struct
{
	char str[30];
} String30;

void setGreatestWeights(Vec4f& weights, Vec4f& boneIds, float newWeight, s32 newBoneId);

template<typename T>
bool SerializeVector(const std::vector<T>& vector, FILE* file) 
{
	if (file == NULL) return false;

	s32 size = (s32)vector.size();
	fwrite(&size, sizeof(size_t), 1, file);
	if (size != 0) 
	{
		s32 totalWrites = (s32)fwrite(&vector[0], sizeof(T), size, file);
		return totalWrites == size;
	}

	return true;
}

template<typename T>
bool DeserializeVector(std::vector<T>& vector, FILE* file)
{
	if (file == NULL) return false;

	s32 size = 0;
	fread(&size, sizeof(size_t), 1, file);
	vector.resize(size);
	if (size != 0)
	{
		s32 totalReads = (s32)fread(&vector[0], sizeof(T), size, file);
		return totalReads == size;
	}
	return true;
}

bool serializeString(std::string& s, FILE* file);
bool deserializeString(std::string& s, FILE* file);
bool serializeCharArray(const char* string, FILE* file);
bool deserializeCharArray(char* string, FILE* file);
void readAllFile(std::string& content, FILE* file);
char* copySTR(const char* source);

//this memory stats stuff is taken from javi agenjo's code
//https://www.dtic.upf.edu/~jagenjo/?page_id=11
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
std::string getGPUStats();
bool debugDrawText(float x, float y, std::string text, Vec3f c, float scale, s32 window_width, s32 window_height);

#endif // !UTILS_H