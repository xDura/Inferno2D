#ifndef UTILS_H
#define UTILS_H

#include "opengl_defines.h"
#include "math.h"

void SetGreatestWeights(Vector4& weights, Vector4& boneIds, float newWeight, int newBoneId);

bool drawText(float x, float y, std::string text, Vector3 c, float scale, int window_width, int window_height);

//this memory stats stuff is taken from javi agenjo's code
//https://www.dtic.upf.edu/~jagenjo/?page_id=11
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
std::string getGPUStats();
//
//#pragma region DEBUG_LINE_RENDERER
//void AddLine(Mesh* mesh, const Vector3& start, const Vector3& end, const Vector3& color)
//{
//	mesh->verts.push_back(start);
//	mesh->verts.push_back(end);
//
//	//one color for each vert
//	mesh->colors.push_back(color);
//	mesh->colors.push_back(color);
//}
//
//void AddWhireSphereLines(Mesh* mesh, const Vector3& position, const Vector3& color, float radius = 1.0f)
//{
//	float angle = 10.0f;
//
//	Vector3 x = Vector3(position.x, position.y + radius * (float)sin(0), position.z + radius * (float)cos(0));
//	Vector3 y = Vector3(position.x + radius * (float)cos(0), position.y, position.z + radius * (float)sin(0));
//	Vector3 z = Vector3(position.x + radius * (float)cos(0), position.y + radius * (float)sin(0), position.z);
//
//	Vector3 new_x;
//	Vector3 new_y;
//	Vector3 new_z;
//
//	for (int i = 1; i < 37; i++)
//	{
//		new_x = Vector3(position.x, position.y + radius * (float)sin(RADTODEG(angle*i)), position.z + radius * (float)cos(RADTODEG(angle*i)));
//		new_y = Vector3(position.x + radius * (float)cos(RADTODEG(angle*i)), position.y, position.z + radius * (float)sin(RADTODEG(angle*i)));
//		new_z = Vector3(position.x + radius * (float)cos(RADTODEG(angle*i)), position.y + radius * (float)sin(RADTODEG(angle*i)), position.z);
//
//		AddLine(mesh, x, new_x, color);
//		AddLine(mesh, y, new_y, color);
//		AddLine(mesh, z, new_z, color);
//
//		x = new_x;
//		y = new_y;
//		z = new_z;
//	}
//}
//
//void AddCircleLines(Mesh* mesh, const Vector3& position, const Vector3& up, const Vector3& color, float radius = 1.0f)
//{
//	Vector3 _up = up.normalized() * radius;
//	Vector3 _forward = Vector3::slerp(_up, (up * -1.0f), 0.5f);
//	Vector3 _right = _up.cross(_forward).normalized() * radius;
//
//	Mat44 matrix = Mat44();
//
//	matrix.values[0] = _right.x;
//	matrix.values[1] = _right.y;
//	matrix.values[2] = _right.z;
//
//	matrix.values[4] = _up.x;
//	matrix.values[5] = _up.y;
//	matrix.values[6] = _up.z;
//
//	matrix.values[8] = _forward.x;
//	matrix.values[9] = _forward.y;
//	matrix.values[10] = _forward.z;
//
//	Vector3 p = Vector3((float)cos(0), 0, (float)cos(0));
//	Vector3 _lastPoint = position + (matrix * p);
//	Vector3 _nextPoint = Vector3::zero;
//
//	for (int i = 0; i < 91; i++)
//	{
//		_nextPoint.x = (float)RADTODEG((float)cos((i * 4)));
//		_nextPoint.z = (float)RADTODEG((float)sin((i * 4)));
//		_nextPoint.y = 0;
//
//		_nextPoint = position + (matrix * _nextPoint);
//
//		AddLine(mesh, _lastPoint, _nextPoint, color);
//		_lastPoint = _nextPoint;
//	}
//}
//
//void AddConeLines(Mesh* mesh, const Vector3& position, const Vector3& direction, const Vector3& color, float angle = 45)
//{
//	float length = direction.mod();
//
//	Vector3 _forward = direction;
//	Vector3 forwardInv = _forward * -1.0f;
//	Vector3 _up = Vector3::slerp(_forward, forwardInv, 0.5f);
//	Vector3 _right = _forward.cross(_up).normalized() * length;
//
//	Vector3 normalizedDirection = direction.normalized();
//
//	Vector3 slerpedVector = Vector3::slerp(_forward, _up, angle / 90.0f);
//
//	float dist;
//	Vector3 invDir = normalizedDirection * -1;
//	float denom = invDir.dot(slerpedVector);
//	if (denom > 0)
//	{
//		Vector3 p0l0 = (position + _forward) - position;
//		dist = p0l0.dot(invDir) * (1 / denom);
//	}
//	else
//		dist = 0;
//
//	AddLine(mesh, position, slerpedVector.normalized() * dist, color);
//	AddLine(mesh, position, Vector3::slerp(_forward, _up * -1, angle / 90.0f).normalized()*dist, color);
//	AddLine(mesh, position, Vector3::slerp(_forward, _right * -1, angle / 90.0f).normalized()*dist, color);
//	AddLine(mesh, position, Vector3::slerp(_forward, _right * -1, angle / 90.0f).normalized()*dist, color);
//
//	AddCircleLines(mesh, position + _forward, normalizedDirection, color, (_forward - (slerpedVector.normalized() *dist)).mod());
//	AddCircleLines(mesh, position + (_forward*0.5f), normalizedDirection, color, ((_forward*0.5f) - (slerpedVector.normalized()*(dist*0.5f))).mod());
//}
//
//void AddArrowLines(Mesh* mesh, const Vector3& position, const Vector3& direction, const Vector3& color)
//{
//	AddLine(mesh, position, direction, color);
//	AddConeLines(mesh, position + direction, direction * -0.333f, color, 15);
//}
//
//void ClearLines(Mesh* mesh)
//{
//	mesh->verts.clear();
//	mesh->colors.clear();
//}
//
//void ReserveLines(Mesh* mesh)
//{
//	mesh->verts.reserve(20000);
//	mesh->colors.reserve(20000);
//}
//
//void InitLines(Mesh* mesh)
//{
//	glGenVertexArrays(1, &mesh->VAO);
//	glBindVertexArray(mesh->VAO);
//
//	glGenBuffersARB(1, &(mesh->verts_VBO));
//	glGenBuffersARB(1, &(mesh->colors_VBO));
//
//	glBindVertexArray(0);
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
//}
//
//void RenderAsLines(Mesh* mesh)
//{
//	glBindVertexArray(mesh->VAO);
//
//	// position attribute
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mesh->verts_VBO);
//	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mesh->verts.size() * sizeof(Vector3), &(mesh->verts[0]), GL_DYNAMIC_DRAW_ARB);
//	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArrayARB(0);
//
//	// color attribute
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mesh->colors_VBO);
//	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mesh->colors.size() * sizeof(Vector3), &(mesh->colors[0]), GL_DYNAMIC_DRAW_ARB);
//	glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArrayARB(1);
//
//	glDrawArrays(GL_LINES, 0, (GLsizei)(mesh->verts.size()));
//
//	glBindVertexArray(0);
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
//}
//
//void AddSkeletonLines(Mesh* lineMesh, SkinnedMesh* skeletonMesh, int currentIndex)
//{
//	Node* node = &(skeletonMesh)->nodes[currentIndex];
//	int parentIndex = (skeletonMesh)->nodes[currentIndex].parentIndex;
//	Node* parentNode = &(skeletonMesh)->nodes[parentIndex];
//
//	Vector3 pos, pos2;
//	pos = skeletonMesh->bindMatrices[node->index].getPosition();
//	pos2 = skeletonMesh->bindMatrices[parentNode->index].getPosition();
//
//	Vector3 poscurrent = skeletonMesh->currentPoseModelToBoneMatrices[node->index].getPosition();
//	Vector3 poscurrentParent = skeletonMesh->currentPoseModelToBoneMatrices[parentNode->index].getPosition();
//
//	AddLine(lineMesh, poscurrent, poscurrentParent, Vector3(0.0f, 1.0f, 0.0f));
//	AddWhireSphereLines(lineMesh, poscurrent, Vector3(0.0f, 1.0f, 0.0f), 1.0f);
//
//	for (int i = 0; i < node->childIndices.size(); i++)
//	{
//		int childIndex = node->childIndices[i];
//		AddSkeletonLines(skeletonMesh, lineMesh, childIndex);
//	}
//}
//
//void DebugSkeleton(Mesh* lineMesh, SkinnedMesh* skeletonMesh)
//{
//	int size = (int)(skeletonMesh)->nodes[0].childIndices.size();
//	for (int i = 0; i < size; i++)
//	{
//		int childIndex = (skeletonMesh)->nodes[0].childIndices[i];
//		AddSkeletonLines(lineMesh, skeletonMesh, (int)childIndex);
//	}
//}
//#pragma endregion

#endif // !UTILS_H