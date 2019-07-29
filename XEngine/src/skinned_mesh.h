#pragma once
#include "mesh.h"
#include "3d_animation.h"

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
	SkinnedMesh()
	{

	}

	~SkinnedMesh() 
	{
		boneIds.clear();
		weights.clear();
		nodes.clear();
		boneNames.clear();
		bones.clear();
		boneMatrices.clear();
		bindMatrices.clear();
		invBindMatrices.clear();
		currentPoseModelToBoneMatrices.clear();
		animations.clear();
	}

	//TODO: Remove all of this from Mesh.h

	//skinning
	std::vector<Vector4> boneIds;
	std::vector<Vector4> weights;

	//the rig
	std::vector<int> bones;
	std::vector<std::string> boneNames;
	std::vector<Node> nodes;

	std::vector<Mat44> boneMatrices;
	std::vector<Mat44> bindMatrices;
	std::vector<Mat44> invBindMatrices;
	std::vector<Mat44> currentPoseModelToBoneMatrices;
	const static int maxBones = 80;
	const static int samplesPerSecond = 15;

	std::vector<Animation> animations;

	unsigned int boneIds_VBO;
	unsigned int boneWeights_VBO;
	//******

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