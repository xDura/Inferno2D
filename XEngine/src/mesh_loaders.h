#pragma once
#include "math.h"
#include "skinned_mesh.h"

#pragma region ASSIMP_INCLUDES
#include "..\include\assimp\postprocess.h"
#include "..\include\assimp\cimport.h"
#include "..\include\assimp\Importer.hpp"
#include "..\include\assimp\scene.h"
#include "..\include\assimp\DefaultLogger.hpp"
#include "..\include\assimp\Logger.hpp" 
#include "..\include\assimp\anim.h"
#pragma endregion

#pragma region ASSIMP_STUFF

//TODO: move this to some assimp utilities or something
Vec3f toVector3(const aiVector3D& v);
Vec3f toVector3(const aiColor4D& v);
Vec3f toVector3(const aiColor3D& v);
Quaternion toQuaternion(const aiQuaternion& q);
Vec2f toVector2(const aiVector3D& v);
Mat44f toMat44(const aiMatrix4x4& m);
aiNodeAnim* GetNodeAnimation(std::string nodeName, aiAnimation* anim);

//this computes the number of total bones in
//an assimp scene without repeats
void FillUniqueBoneNames(SkinnedMesh* mesh, const aiScene* scene);
s32 GetNumTotalVertices(const aiScene* scene);
void LoadMeshVertexData(SkinnedMesh* mesh, aiMesh* assimpMesh, std::vector<Vec3f>* verts, std::vector<Vec3f>* normals, std::vector<Vec2f>* uvs, std::vector<Vec4f>* weights, std::vector<Vec4f>* boneIds /*ADD COLORS*/);
//saves a full keyframe (for all bones) in animation 
void SaveAnimationSample(SkinnedMesh* mesh, Node* node, aiAnimation* assimpAnim, Animation* targetAnimation, float t, bool isFbx);
aiNode* GetNodeRecursively(const std::string& boneName, aiNode* from);
void loadAssimp(SkinnedMesh* mesh, const std::string& path);

//****
#pragma endregion

void loadASE(Mesh* mesh, const std::string& path);
void generateQuad(Mesh* mesh);
void generateQuad(Mesh* mesh, Vec3f color);
void SaveBinary(SkinnedMesh* mesh, const char* path);
void LoadBinary(SkinnedMesh* mesh, const char* path);