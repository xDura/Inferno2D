#pragma once
#include <vector>
#include "math.h"

//#include <iostream>
//#include <iterator>
//#include <numeric>
//#include <algorithm>

class Mesh
{
public:

	std::vector<Vec3f> verts;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> uvs;
	std::vector<Vec3f> colors;

	u32 verts_VBO;
	u32 uvs_VBO;
	u32 colors_VBO;
	u32 VAO;

	//@TODO: remove this when we add the verts
	u32 length;

	Mesh(){}

	~Mesh()
	{
		verts.clear();
		normals.clear();
		uvs.clear();
		colors.clear();
		//TODO: clear any stuff on the GPU etc
	}

	void initGL();
};