#include "skinned_mesh.h"
#include "utils.h"
#include "platform.h"

//namespace XD
//{
	//TODO: change the indices that I use for the glVertexAttribPointers
	//to be constants: ex : Vertices is always 1 colors is always 4

	void SkinnedMesh::initBonesGL()
	{
		glBindVertexArray(VAO);

		//move this to the skinnedMesh
		//boneWeights attribute
		glGenBuffersARB(1, &boneWeights_VBO);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneWeights_VBO);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, weights.size() * sizeof(Vec4f), &weights[0], GL_STATIC_DRAW_ARB);
		glVertexAttribPointerARB(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4f), (void*)0);
		glEnableVertexAttribArrayARB(2);

		//boneIds attribute
		glGenBuffersARB(1, &boneIds_VBO);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, boneIds_VBO);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, boneIds.size() * sizeof(Vec4f), &boneIds[0], GL_STATIC_DRAW_ARB);
		glVertexAttribPointerARB(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4f), (void*)0);
		glEnableVertexAttribArrayARB(3);
		//**

		glBindVertexArray(0);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	void SkinnedMesh::sampleAnimation(Node * node, Animation * anim, s32 keyframeIndex)
	{
		BoneKeyFrame a = anim->keyframes[node->index][keyframeIndex];
		Vec3f desiredPos = a.pos;
		Quaternion desiredRot = a.rot;
		Vec3f desiredScale = a.scale;
		Mat44f pos;
		pos.setTranslation(desiredPos);
		Mat44f rot;
		rot = desiredRot.toMatrix();
		Mat44f scale;
		scale.setScale(desiredScale);
		Mat44f desiredTransform = scale * rot * pos;

		boneMatrices[node->index] = desiredTransform;

		for (u32 i = 0; i < node->childIndices.size(); i++)
			sampleAnimation(&nodes[node->childIndices[i]], anim, keyframeIndex);
	}

	void SkinnedMesh::sampleAnimation(Node * node, Animation * anim, f32 t)
	{
		s32 sampleA = (s32)(anim->samplesPerSecond * t);
		if (sampleA >= anim->numSamples) 
			sampleA = anim->numSamples - 1;

		s32 sampleB = sampleA + 1;
		if (sampleB >= anim->numSamples)
		{
			if (anim->loop)
			{
				//TODO: refactor this one
				sampleA = anim->numSamples - 2;
				sampleB = sampleA + 1;
			}
			else
				sampleB = sampleA;
		}

		BoneKeyFrame a = anim->keyframes[node->index][sampleA];
		BoneKeyFrame b = anim->keyframes[node->index][sampleB];

		f32 blendTime = t;
		blendTime = clamp(blendTime, (f32)a.time, (f32)b.time);

		blendTime = map(blendTime, (f32)a.time, (f32)b.time, 0.0f, 1.0f);
		blendTime = clamp(blendTime, 0.0f, 1.0f);

		Vec3f desiredPos = Vec3f::lerp(a.pos, b.pos, blendTime);
		Quaternion desiredRot = a.rot;
		desiredRot.lerp(b.rot, blendTime);
		Vec3f desiredScale = Vec3f::lerp(a.scale, b.scale, blendTime);

		Mat44f pos;
		pos.setTranslation(desiredPos);
		Mat44f rot;
		rot = desiredRot.toMatrix();
		Mat44f scale;
		scale.setScale(desiredScale);
		Mat44f desiredTransform = scale * rot * pos;

		Mat44f parentModelToBone;
		if (node->parentIndex != -1)
			parentModelToBone = currentPoseModelToBoneMatrices[node->parentIndex];

		currentPoseModelToBoneMatrices[node->index] = (desiredTransform * parentModelToBone);

		//multiply ModelToBone * invBindMatrix to get the Local&Relative matrix
		Mat44f localRelativeMat = (invBindMatrices[node->index] * currentPoseModelToBoneMatrices[node->index]);

		boneMatrices[node->index] = localRelativeMat;

		for (u32 i = 0; i < node->childIndices.size(); i++)
			sampleAnimation(&nodes[node->childIndices[i]], anim, t);
	}

	void SkinnedMesh::logBoneHierarchy(Node * node)
	{
		std::string log = boneNames[node->index];
		log += " with parent: ";
		if (node->parentIndex != -1)
			log += boneNames[node->parentIndex];

		LOG(log.c_str());
		for (u32 i = 0; i < node->childIndices.size(); i++)
			logBoneHierarchy(&nodes[node->childIndices[i]]);
	}

	//returns the index of the bone or -1 if not found

	int SkinnedMesh::getBoneIndex(const std::string & boneName)
	{
		for (u32 i = 0; i < boneNames.size(); i++)
		{
			if (boneNames[i] != boneName) continue;

			return i;
			break;
		}

		return -1;
	}

	void SkinnedMesh::setBindPose()
	{
		for (u32 i = 0; i < boneMatrices.size(); i++)
			boneMatrices[i].setIdentity();
	}

	void SkinnedMesh::computeBindMatrices()
	{
		computeBindMatrices(&nodes[0]);
	}

	void SkinnedMesh::computeBindMatrices(Node * node)
	{
		Mat44f parentBindMat;
		Mat44f currentMat;
		Mat44f bindMat;
		currentMat = boneMatrices[node->index];
		if (node->parentIndex != -1)
			parentBindMat = bindMatrices[node->parentIndex];

		Mat44f bind = currentMat * parentBindMat;
		bindMatrices[node->index] = bind;
		invBindMatrices[node->index] = bind;
		invBindMatrices[node->index].inverse();

		u32 childCount = (u32)node->childIndices.size();
		for (u32 i = 0; i < childCount; i++)
		{
			assert(node->childIndices[i] > 0 && node->childIndices[i] < nodes.size());
			Node* currentChild = &nodes[node->childIndices[i]];
			computeBindMatrices(currentChild);
		}
	}
//}
