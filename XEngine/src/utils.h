#ifndef UTILS_H
#define UTILS_H

#include "opengl_defines.h"
#include "math.h"
#include "vector"
#include <stdio.h>

#define BIT(bit) 1 << bit;

void SetGreatestWeights(Vector4& weights, Vector4& boneIds, float newWeight, int newBoneId);

template<typename T>
bool SerializeVector(const std::vector<T>& vector, FILE* file) 
{
	if (file == NULL) return false;

	int size = (int)vector.size();
	fwrite(&size, sizeof(size_t), 1, file);
	if (size != 0) 
	{
		int totalWrites = (int)fwrite(&vector[0], sizeof(T), size, file);
		return totalWrites == size;
	}

	return true;
}

template<typename T>
bool DeserializeVector(std::vector<T>& vector, FILE* file)
{
	if (file == NULL) return false;

	int size = 0;
	fread(&size, sizeof(size_t), 1, file);
	vector.resize(size);
	if (size != 0)
	{
		int totalReads = (int)fread(&vector[0], sizeof(T), size, file);
		return totalReads == size;
	}
	return true;
}

bool SerializeString(std::string& s, FILE* file);
bool DeserializeString(std::string& s, FILE* file);
bool SerializeCharArray(const char* string, FILE* file);
bool DeserializeCharArray(char* string, FILE* file);
void ReadAllFile(std::string& content, FILE* file);
char* Copy(const char* source);

//this memory stats stuff is taken from javi agenjo's code
//https://www.dtic.upf.edu/~jagenjo/?page_id=11
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
std::string getGPUStats();
bool DebugDrawText(float x, float y, std::string text, Vector3 c, float scale, int window_width, int window_height);

#endif // !UTILS_H