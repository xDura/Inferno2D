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

	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<Vector3> colors;

	unsigned int verts_VBO;
	unsigned int uvs_VBO;
	unsigned int colors_VBO;
	unsigned int VAO;

	//@TODO: remove this when we add the verts
	unsigned int length;

	Mesh(){}

	~Mesh()
	{
		verts.clear();
		normals.clear();
		uvs.clear();
		colors.clear();
		//TODO: clear any stuff on the GPU etc
	}

	void InitGL();
};