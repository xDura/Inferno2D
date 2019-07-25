#include "skinned_mesh.h"
#include "utils.h"
#include "platform.h"

//TODO: change the indices that I use for the glVertexAttribPointers
//to be constants: ex : Vertices is always 1 colors is always 4

void SkinnedMesh::InitBonesGL()
{
	glBindVertexArray(VAO);

	//move this to the skinnedMesh
	//boneWeights attribute
	glGenBuffersARB(1, &boneWeights_VBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneWeights_VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, weights.size() * sizeof(Vector4), &weights[0], GL_STATIC_DRAW_ARB);
	glVertexAttribPointerARB(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);
	glEnableVertexAttribArrayARB(2);

	//boneIds attribute
	glGenBuffersARB(1, &boneIds_VBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneIds_VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, boneIds.size() * sizeof(Vector4), &boneIds[0], GL_STATIC_DRAW_ARB);
	glVertexAttribPointerARB(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);
	glEnableVertexAttribArrayARB(3);
	//**

	glBindVertexArray(0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void SkinnedMesh::SampleAnimation(Node * node, Animation * anim, int keyframeIndex)
{
	BoneKeyFrame a = anim->keyframes[node->index][keyframeIndex];
	Vector3 desiredPos = a.pos;
	Quaternion desiredRot = a.rot;
	Vector3 desiredScale = a.scale;
	Mat44 pos;
	pos.setTranslation(desiredPos);
	Mat44 rot;
	rot = desiredRot.toMatrix();
	Mat44 scale;
	scale.setScale(desiredScale);
	Mat44 desiredTransform = scale * rot * pos;

	boneMatrices[node->index] = desiredTransform;

	for (unsigned int i = 0; i < node->childIndices.size(); i++)
		SampleAnimation(&nodes[node->childIndices[i]], anim, keyframeIndex);
}

void SkinnedMesh::SampleAnimation(Node * node, Animation * anim, float t)
{
	int sampleA = (int)(anim->samplesPerSecond * t);
	if (sampleA >= anim->numSamples)
	{
		//TODO: Investigate why we exceed samples
		//Logger::Log("Exceeding samples ", ERR);
		sampleA = anim->numSamples - 1;
	}

	//Logger::Log("Sample: ", sampleA);
	//Logger::Log("t: ", t);

	//TODO: if sampleA is the last just dont interpolate and put the frame
	int sampleB = sampleA + 1;
	if (sampleB >= anim->numSamples)
		sampleB = sampleA;

	BoneKeyFrame a = anim->keyframes[node->index][sampleA];
	BoneKeyFrame b = anim->keyframes[node->index][sampleB];

	float blendTime = t;
	blendTime = clamp(blendTime, (float)a.time, (float)b.time);

	blendTime = map(blendTime, (float)a.time, (float)b.time, 0.0f, 1.0f);
	blendTime = clamp(blendTime, 0.0f, 1.0f);

	Vector3 desiredPos = Vector3::lerp(a.pos, b.pos, blendTime);
	Quaternion desiredRot = a.rot;
	desiredRot.lerp(b.rot, blendTime);
	Vector3 desiredScale = Vector3::lerp(a.scale, b.scale, blendTime);

	Mat44 pos;
	pos.setTranslation(desiredPos);
	Mat44 rot;
	rot = desiredRot.toMatrix();
	Mat44 scale;
	scale.setScale(desiredScale);
	Mat44 desiredTransform = scale * rot * pos;

	Mat44 parentModelToBone;
	if (node->parentIndex != -1)
		parentModelToBone = currentPoseModelToBoneMatrices[node->parentIndex];

	currentPoseModelToBoneMatrices[node->index] = (desiredTransform * parentModelToBone);

	//multiply ModelToBone * invBindMatrix to get the Local&Relative matrix
	Mat44 localRelativeMat = (invBindMatrices[node->index] * currentPoseModelToBoneMatrices[node->index]);

	boneMatrices[node->index] = localRelativeMat;

	for (unsigned int i = 0; i < node->childIndices.size(); i++)
		SampleAnimation(&nodes[node->childIndices[i]], anim, t);
}

void SkinnedMesh::LogBoneHierarchy(Node * node)
{
	std::string log = boneNames[node->index];
	log += " with parent: ";
	if (node->parentIndex != -1)
		log += boneNames[node->parentIndex];

	LOG(log.c_str());
	for (unsigned int i = 0; i < node->childIndices.size(); i++)
		LogBoneHierarchy(&nodes[node->childIndices[i]]);
}

//returns the index of the bone or -1 if not found

int SkinnedMesh::GetBoneIndex(const std::string & boneName)
{
	for (unsigned int i = 0; i < boneNames.size(); i++)
	{
		if (boneNames[i] != boneName) continue;

		return i;
		break;
	}

	return -1;
}

void SkinnedMesh::SetBindPose()
{
	for (unsigned int i = 0; i < boneMatrices.size(); i++)
		boneMatrices[i].setIdentity();
}

void SkinnedMesh::ComputeBindMatrices()
{
	ComputeBindMatrices(&nodes[0]);
}

void SkinnedMesh::ComputeBindMatrices(Node * node)
{
	Mat44 parentBindMat;
	Mat44 currentMat;
	Mat44 bindMat;
	currentMat = boneMatrices[node->index];
	if (node->parentIndex != -1)
		parentBindMat = bindMatrices[node->parentIndex];

	Mat44 bind = currentMat * parentBindMat;
	bindMatrices[node->index] = bind;
	invBindMatrices[node->index] = bind;
	invBindMatrices[node->index].inverse();

	unsigned int childCount = (unsigned int)node->childIndices.size();
	for (unsigned int i = 0; i < childCount; i++)
	{
		assert(node->childIndices[i] > 0 && node->childIndices[i] < nodes.size());
		Node* currentChild = &nodes[node->childIndices[i]];
		ComputeBindMatrices(currentChild);
	}
}
