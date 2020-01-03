#pragma once
#include "mesh.h"

//TODO: remove this from here as 
//soon as skinnedMeshes are extracted from here
#include "3d_animation.h"

//
#pragma region assimp
#include "..\include\assimp\postprocess.h"
#include "..\include\assimp\cimport.h"
#include "..\include\assimp\Importer.hpp"
#include "..\include\assimp\scene.h"
#include "..\include\assimp\DefaultLogger.hpp"
#include "..\include\assimp\Logger.hpp" 
#include "..\include\assimp\anim.h" 
#pragma endregion
//namespace XD
//{
	class Node
	{
	public:
		int index = 0;
		int parentIndex = 0;
		std::vector<u32> childIndices;
	};

	class SkinnedMesh : public Mesh
	{
	public:

		//skinning
		std::vector<Vec4f> boneIds;
		std::vector<Vec4f> weights;
		u32 boneIds_VBO;
		u32 boneWeights_VBO;
		//******

		//skeleton
		std::vector<std::string> boneNames;
		std::vector<Node> nodes;
		std::vector<Mat44f> bindMatrices;
		std::vector<Mat44f> invBindMatrices;
		//******

		//skeleton runtime
		std::vector<Mat44f> boneMatrices;
		std::vector<Mat44f> currentPoseModelToBoneMatrices;
		//******

		//skeleton (move to constatns)
		const static u32 maxBones = 80;

		//animation (move to somewhere)
		std::vector<Animation> animations;
		const static u32 samplesPerSecond = 15;
		//*****

		SkinnedMesh()
		{

		}

		~SkinnedMesh()
		{
			boneIds.clear();
			weights.clear();
			nodes.clear();
			boneNames.clear();
			boneMatrices.clear();
			bindMatrices.clear();
			invBindMatrices.clear();
			currentPoseModelToBoneMatrices.clear();
			animations.clear();
		}

		//TODO: change the indices that I use for the glVertexAttribPointers
		//to be constants: ex : Vertices is always 1 colors is always 4
		void initBonesGL();
		void sampleAnimation(Node* node, Animation* anim, s32 keyframeIndex);
		void sampleAnimation(Node* node, Animation* anim, f32 t);
		void logBoneHierarchy(Node* node);
		//returns the index of the bone or -1 if not found
		int getBoneIndex(const std::string& boneName);
		void setBindPose();
		void computeBindMatrices();
		void computeBindMatrices(Node* node);
	};
//}