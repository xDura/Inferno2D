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
		std::vector<int> childIndices;
	};

	class SkinnedMesh : public Mesh
	{
	public:

		//skinning
		std::vector<Vector4> boneIds;
		std::vector<Vector4> weights;
		unsigned int boneIds_VBO;
		unsigned int boneWeights_VBO;
		//******

		//skeleton
		std::vector<std::string> boneNames;
		std::vector<Node> nodes;
		std::vector<Mat44> bindMatrices;
		std::vector<Mat44> invBindMatrices;
		//******

		//skeleton runtime
		std::vector<Mat44> boneMatrices;
		std::vector<Mat44> currentPoseModelToBoneMatrices;
		//******

		//skeleton (move to constatns)
		const static int maxBones = 80;

		//animation (move to somewhere)
		std::vector<Animation> animations;
		const static int samplesPerSecond = 15;
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
		void InitBonesGL();
		void SampleAnimation(Node* node, Animation* anim, int keyframeIndex);
		void SampleAnimation(Node* node, Animation* anim, float t);
		void LogBoneHierarchy(Node* node);
		//returns the index of the bone or -1 if not found
		int GetBoneIndex(const std::string& boneName);
		void SetBindPose();
		void ComputeBindMatrices();
		void ComputeBindMatrices(Node* node);
	};
//}