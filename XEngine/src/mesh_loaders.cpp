#include "mesh_loaders.h"
#include "platform.h"
#include "External/textparser.h"

//TODO: move this to some assimp utilities or something
Vector3 toVector3(const aiVector3D & v)
{
	Vector3 vector3;
	vector3.x = v.x;
	vector3.y = v.y;
	vector3.z = v.z;
	return vector3;
}

Vector3 toVector3(const aiColor4D & v)
{
	Vector3 vector3;
	vector3.x = v.r;
	vector3.y = v.g;
	vector3.z = v.b;
	return vector3;
}

Vector3 toVector3(const aiColor3D & v)
{
	Vector3 vector3;
	vector3.x = v.r;
	vector3.y = v.g;
	vector3.z = v.b;
	return vector3;
}

Quaternion toQuaternion(const aiQuaternion & q)
{
	Quaternion quaternion;
	quaternion.x = q.x;
	quaternion.y = q.y;
	quaternion.z = q.z;
	quaternion.w = q.w;
	return quaternion;
}

Vector2 toVector2(const aiVector3D & v)
{
	Vector2 vector2;
	vector2.x = v.x;
	vector2.y = v.y;
	return vector2;
}

Mat44 toMat44(const aiMatrix4x4 & m)
{
	Mat44 newMat;
	newMat.values[0] = m.a1;
	newMat.values[1] = m.a2;
	newMat.values[2] = m.a3;
	newMat.values[3] = m.a4;

	newMat.values[4] = m.b1;
	newMat.values[5] = m.b2;
	newMat.values[6] = m.b3;
	newMat.values[7] = m.b4;

	newMat.values[8] = m.c1;
	newMat.values[9] = m.c2;
	newMat.values[10] = m.c3;
	newMat.values[11] = m.c4;

	newMat.values[12] = m.d1;
	newMat.values[13] = m.d2;
	newMat.values[14] = m.d3;
	newMat.values[15] = m.d4;

	//transpose it (assimp is row major)
	newMat.transpose();
	return newMat;
}

aiNodeAnim * GetNodeAnimation(std::string nodeName, aiAnimation * anim)
{
	unsigned int numChannels = anim->mNumChannels;
	std::string currentName = "";
	for (unsigned int i = 0; i < numChannels; i++)
	{
		currentName = anim->mChannels[i]->mNodeName.C_Str();
		if (nodeName != currentName) continue;

		return anim->mChannels[i];
	}

	return NULL;
}

//this computes the number of total bones in
//an assimp scene without repeats
void FillUniqueBoneNames(SkinnedMesh * mesh, const aiScene * scene)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
		{
			std::string boneName = scene->mMeshes[i]->mBones[j]->mName.C_Str();
			if ((mesh->GetBoneIndex(boneName)) != -1) continue;

			mesh->boneNames.push_back(boneName);
		}
	}
}

int GetNumTotalVertices(const aiScene * scene)
{
	int totalVerts = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		totalVerts += (scene->mMeshes[i]->mNumFaces * 3);

	return totalVerts;
}

void LoadMeshVertexData(SkinnedMesh * mesh, aiMesh * assimpMesh, std::vector<Vector3>* verts, std::vector<Vector3>* normals, std::vector<Vector2>* uvs, std::vector<Vector4>* weights, std::vector<Vector4>* boneIds)
{
	LOG("Loading all vertex data ");
	int numVerts = (assimpMesh->mNumFaces * 3);
	LOG("num vertices ", numVerts);

	verts->resize(numVerts);
	normals->resize(numVerts);
	uvs->resize(numVerts);
	boneIds->resize(numVerts);
	weights->resize(numVerts);
	//colors->resize(numVerts);

	//aux weight arrays
	std::vector<int> numWeightsOnVertex;
	numWeightsOnVertex.resize(numVerts);
	std::vector<Vector4> auxAllBones;
	auxAllBones.resize(numVerts);
	std::vector<Vector4> auxAllWeights;
	auxAllWeights.resize(numVerts);
	//***

	LOG("setting auxiliar weight arrays");
	for (unsigned int i = 0; i < assimpMesh->mNumBones; i++)
	{
		for (unsigned int j = 0; j < assimpMesh->mBones[i]->mNumWeights; j++)
		{
			float weight = assimpMesh->mBones[i]->mWeights[j].mWeight;
			float vertId = (float)assimpMesh->mBones[i]->mWeights[j].mVertexId;
			int currentNumWeights = numWeightsOnVertex[(int)vertId];

			if (currentNumWeights >= mesh->maxBones)
				SetGreatestWeights(auxAllWeights[(int)vertId], auxAllBones[(int)vertId], weight, i);
			else
			{
				int boneIndex = mesh->GetBoneIndex(assimpMesh->mBones[i]->mName.C_Str());
				if (boneIndex != -1)
				{
					auxAllWeights[(int)vertId].values[currentNumWeights] = weight;
					auxAllBones[(int)vertId].values[currentNumWeights] = (float)boneIndex;
					numWeightsOnVertex[(int)vertId] += 1;
				}
				else
					LOGERROR("Index not found in hierarchy");
			}
		}
	}

	LOG("Unindexing vertices");
	for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			int index = assimpMesh->mFaces[i].mIndices[j];

			aiVector3D vert = (assimpMesh)->mVertices[index];
			aiVector3D normal = (assimpMesh)->mNormals[index];
			aiVector3D* uv = &(assimpMesh->mTextureCoords[0][index]);

			int fullIndex = (3 * i) + j;

			(*verts)[fullIndex] = toVector3(vert);
			(*normals)[fullIndex] = toVector3(normal);
			(*uvs)[fullIndex] = toVector2(*uv);
			(*weights)[fullIndex] = auxAllWeights[index];
			(*boneIds)[fullIndex] = auxAllBones[index];

			/*if ((assimpMesh)->HasVertexColors(0))
			{
			aiColor4D* color = &(assimpMesh)->mColors[0][index];
			colors[fullIndex] = toVector3(*color);
			}*/
		}
	}

	//free all aux stuff
	numWeightsOnVertex.clear();
	auxAllWeights.clear();
	auxAllBones.clear();
}

//saves a full keyframe (for all bones) in animation 
void SaveAnimationSample(SkinnedMesh * mesh, Node * node, aiAnimation * assimpAnim, Animation * targetAnimation, float t, bool isFbx)
{
	//find the two desired keys based on time for position
	//get the lerp of both of them
	aiNodeAnim* nodeAnim = GetNodeAnimation(mesh->boneNames[node->index], assimpAnim);
	if (nodeAnim == NULL)
	{
		LOGERROR("SaveAnimationSample bone not found id: %d", node->index);
		return;
	}

	//TODO: investigate the assimp fix timing issue (mTime is just the frame number, not the time in seconds but in collada it is)
	//the problem is also that m_totalTicks is the total time of the animation on dae files instead of the full number of samples
	double animationDurationSecs = assimpAnim->mDuration / assimpAnim->mTicksPerSecond;
	float samplesToTime = (float)animationDurationSecs / (float)assimpAnim->mDuration;

	Vector3 position = toVector3(nodeAnim->mPositionKeys[0].mValue);
	for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys; i++)
	{
		float timeToCompare = (float)nodeAnim->mPositionKeys[i].mTime * samplesToTime;
		if (timeToCompare < t) continue;

		//desired keyframe found!
		if (i > 0)
		{
			aiVectorKey a = nodeAnim->mPositionKeys[i - 1];
			aiVectorKey b = nodeAnim->mPositionKeys[i];
			float aTime = (float)a.mTime * samplesToTime;
			float bTime = (float)b.mTime * samplesToTime;

			float blendTime = clamp(t, (float)a.mTime, (float)b.mTime);

			blendTime = map(blendTime, aTime, bTime, 0, 1);
			blendTime = clamp(blendTime, 0.0f, 1.0f);

			Vector3 posA = toVector3(a.mValue);
			Vector3 posB = toVector3(b.mValue);

			position = Vector3::lerp(posA, posB, blendTime);
		}
		else
			position = toVector3(nodeAnim->mPositionKeys[i].mValue);

		break;
	}

	//find the two desired keys based on time for rotation
	//get the lerp of both of them

	Quaternion rotation = toQuaternion(nodeAnim->mRotationKeys[0].mValue);
	for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys; i++)
	{
		float timeToCompare = (float)nodeAnim->mRotationKeys[i].mTime * samplesToTime;
		if (timeToCompare < t) continue;

		//desired keyframe found!
		if (i > 0)
		{
			aiQuatKey a = nodeAnim->mRotationKeys[i - 1];
			aiQuatKey b = nodeAnim->mRotationKeys[i];
			float aTime = (float)a.mTime * samplesToTime;
			float bTime = (float)b.mTime * samplesToTime;

			float blendTime = clamp(t, (float)a.mTime, (float)b.mTime);

			blendTime = map(blendTime, aTime, bTime, 0, 1);
			blendTime = clamp(blendTime, 0.0f, 1.0f);

			Quaternion rotA = toQuaternion(a.mValue);
			Quaternion rotB = toQuaternion(b.mValue);

			rotation = rotA;
			rotation.lerp(rotB, blendTime);
		}
		else
			rotation = toQuaternion(nodeAnim->mRotationKeys[i].mValue);

		break;
	}

	//find the two desired keys based on time for scale
	//get the lerp of both of them
	Vector3 scale = toVector3(nodeAnim->mScalingKeys[0].mValue);
	for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys; i++)
	{
		float timeToCompare = (float)nodeAnim->mScalingKeys[i].mTime * samplesToTime;
		if (timeToCompare < t) continue;

		//desired keyframe found!
		if (i > 0)
		{
			aiVectorKey a = nodeAnim->mScalingKeys[i - 1];
			aiVectorKey b = nodeAnim->mScalingKeys[i];
			float aTime = (float)a.mTime * samplesToTime;
			float bTime = (float)b.mTime * samplesToTime;

			float blendTime = clamp(t, (float)a.mTime, (float)b.mTime);

			blendTime = map(blendTime, aTime, bTime, 0, 1);
			blendTime = clamp(blendTime, 0.0f, 1.0f);

			Vector3 scaleA = toVector3(a.mValue);
			Vector3 scaleB = toVector3(b.mValue);

			scale = Vector3::lerp(scaleA, scaleB, blendTime);
		}
		else
			scale = toVector3(nodeAnim->mScalingKeys[i].mValue);

		break;
	}

	//construct a matrix from position, rotation and scale
	//Mat44 posMat;
	//posMat.setTranslation(position);
	//Mat44 rotMat;
	//rotation.toMatrix(rotMat);
	//Mat44 scaleMat;
	//scaleMat.setScale(scale);
	//Mat44 desiredTransform = scaleMat * rotMat * posMat;

	BoneKeyFrame keyframe;
	keyframe.pos = position;
	keyframe.rot = rotation.invert();
	keyframe.scale = scale;
	keyframe.time = t;
	targetAnimation->keyframes[node->index].push_back(keyframe);

	//this matrix is Local but Absolute (we need Local&Relative to bindPose matrix)
	//use this matrix to calculate this bone&frame ModelToBone matrix using

	////THE BUG MUST BE HERE
	//Mat44 parentModelToBone;
	//if (node->parentIndex != -1)
	//	parentModelToBone = currentPoseModelToBoneMatrices[node->parentIndex];

	//currentPoseModelToBoneMatrices[node->index] = (desiredTransform * parentModelToBone);

	////multiply ModelToBone * invBindMatrix to get the Local&Relative matrix
	//Mat44 invBindMatrix = bindMatrices[node->index];
	//invBindMatrix.inverse();
	//Mat44 localRelativeMat = (invBindMatrix * currentPoseModelToBoneMatrices[node->index]);

	////decompose Local&Relative matrix in position, rotation and scale and save it on the keyframe
	////TODO
	//Vector3 localRelativePos = localRelativeMat.getPosition();
	//Mat44 localRelativeRotMat = localRelativeMat.getRotationOnly();
	//Quaternion localRelativeRotQuat;
	//localRelativeRotQuat.fromMatrix(localRelativeRotMat);
	//Vector3 localRelativeScale = localRelativeMat.getScale();
	//BoneKeyFrame keyframe;
	//keyframe.pos = localRelativePos;
	//keyframe.rot = localRelativeRotQuat;
	//keyframe.scale = localRelativeScale;
	//keyframe.time = t;
	////TODO: make the calculation to put it in place, not at the end
	//targetAnimation->keyframes[node->index].push_back(keyframe);
	////*************

	//save sample for its child bones
	for (unsigned int i = 0; i < node->childIndices.size(); i++)
		SaveAnimationSample(mesh, &(mesh->nodes[node->childIndices[i]]), assimpAnim, targetAnimation, t, isFbx);
}

aiNode * GetNodeRecursively(const std::string & boneName, aiNode * from)
{
	std::string name(from->mName.C_Str());
	if (name.compare(boneName) == 0)
		return from;
	else
	{
		for (unsigned int i = 0; i < from->mNumChildren; i++)
		{
			aiNode* result = GetNodeRecursively(boneName, from->mChildren[i]);
			if (result != NULL)
				return result;
		}

		return NULL;
	}
}

void loadAssimp(SkinnedMesh * mesh, const std::string & path)
{
	bool fbx = false;
	//check the extension
	std::string extension = path.substr(path.find_last_of("."));
	if (extension == ".dae")
	{
		LOG("file is collada");
	}
	else if (extension == ".fbx")
	{
		LOG("file is fbx");
		fbx = true;
	}

	Assimp::Importer importer;

	std::string fullpath = Path::GetPath(path);

	const aiScene* scene = importer.ReadFile(fullpath, aiProcessPreset_TargetRealtime_MaxQuality);
	std::string err(aiGetErrorString());

	if (err != "" || scene == NULL)
	{
		LOGERROR("error importing mesh at: %s", fullpath);
		return;
	}

	FillUniqueBoneNames(mesh, scene);
	int numBones = (int)mesh->boneNames.size();

	//check for all our restrictions
	if (numBones >= mesh->maxBones)
	{
		LOGERROR("Mesh at: %s has to much bones: %d", fullpath, numBones);
		importer.FreeScene();
		mesh->boneNames.clear();
		return;
	}

	//BONES & NODES
	LOG("Loading Bones data %d", numBones);
	mesh->boneMatrices.resize(numBones);
	mesh->bindMatrices.resize(numBones);
	mesh->invBindMatrices.resize(numBones);
	mesh->currentPoseModelToBoneMatrices.resize(numBones);
	mesh->nodes.resize(numBones);

	//get all bones for meshes
	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* currentMesh = scene->mMeshes[m];

		for (unsigned int i = 0; i < currentMesh->mNumBones; i++)
		{
			std::string boneName = (currentMesh)->mBones[i]->mName.C_Str();
			int index = mesh->GetBoneIndex(boneName);

			LOG(boneName.c_str());
			Mat44 boneMat = toMat44((currentMesh)->mBones[i]->mOffsetMatrix);
			boneMat.inverse();
			mesh->bindMatrices[index] = boneMat;
			mesh->invBindMatrices[index] = boneMat;
			mesh->invBindMatrices[index].inverse();
		}
	}

	LOG("Loading bone hierarchy data");
	std::vector<aiNode*> orderedNodes;
	orderedNodes.resize(numBones);

	//set the aux nodes in order
	for (unsigned int i = 0; i < mesh->boneNames.size(); i++)
	{
		aiNode* node = NULL;
		node = GetNodeRecursively(mesh->boneNames[i], scene->mRootNode);
		if (node == NULL)
		{
			LOGERROR("bone: %s not found in hierarchy", mesh->boneNames[i]);
			continue;
		}

		LOG(mesh->boneNames[i].c_str());
		LOG(node->mName.C_Str());
		LOG("Adding node %d", i);
		Mat44 boneMat = toMat44(node->mTransformation);
		mesh->boneMatrices[i] = boneMat;
		orderedNodes[i] = node;
	}

	//set all nodes data
	for (unsigned int i = 0; i < orderedNodes.size(); i++)
	{
		aiNode* assimpNode = orderedNodes[i];
		Node* node = &(mesh->nodes[i]);
		node->index = i;
		if (assimpNode->mParent != NULL)
		{
			int parentIndex = mesh->GetBoneIndex(assimpNode->mParent->mName.C_Str());
			node->parentIndex = parentIndex;
		}

		for (unsigned int j = 0; j < assimpNode->mNumChildren; j++)
		{
			int childIndex = mesh->GetBoneIndex(assimpNode->mChildren[j]->mName.C_Str());
			//TODO: investigate the hierarchies having bones that are not animated bones
			if (childIndex != -1)
				node->childIndices.push_back(childIndex);
			else
			{
				LOGERROR("index not found for: %s: will have index -1", assimpNode->mChildren[j]->mName.C_Str());
			}
		}
	}
	//****

	//vertices data
	int totalVerts = GetNumTotalVertices(scene);
	mesh->verts.reserve(totalVerts);
	mesh->normals.reserve(totalVerts);
	mesh->uvs.reserve(totalVerts);
	mesh->weights.reserve(totalVerts);
	mesh->boneIds.reserve(totalVerts);

	//vertex color aux
	//TODO: upgrade this
	if (scene->HasMaterials()) mesh->colors.reserve(totalVerts);

	//load vertices data
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* currentMesh = scene->mMeshes[i];
		std::vector<Vector3> current_verts;
		std::vector<Vector3> current_normals;
		std::vector<Vector2> current_uvs;
		std::vector<Vector4> current_weights;
		std::vector<Vector4> current_boneIds;

		LoadMeshVertexData(mesh, currentMesh, &current_verts, &current_normals, &current_uvs, &current_weights, &current_boneIds);

		//add all data at the end
		mesh->verts.insert(mesh->verts.end(), current_verts.begin(), current_verts.end());
		mesh->normals.insert(mesh->normals.end(), current_normals.begin(), current_normals.end());
		mesh->uvs.insert(mesh->uvs.end(), current_uvs.begin(), current_uvs.end());
		mesh->weights.insert(mesh->weights.end(), current_weights.begin(), current_weights.end());
		mesh->boneIds.insert(mesh->boneIds.end(), current_boneIds.begin(), current_boneIds.end());

		//vertex color aux
		//TODO: upgrade this
		if (scene->HasMaterials())
		{
			aiMaterial* currentMaterial = scene->mMaterials[currentMesh->mMaterialIndex];
			std::vector<Vector3> aux_colors;
			aux_colors.resize(mesh->verts.size());
			for (unsigned int i = 0; i < aux_colors.size(); i++)
			{
				aiColor3D ai_color;
				currentMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
				Vector3 col = toVector3(ai_color);
				aux_colors[i] = (col * 1.5f);
			}
			mesh->colors.insert(mesh->colors.end(), aux_colors.begin(), aux_colors.end());
		}
	}
	//**

	mesh->LogBoneHierarchy(&(mesh->nodes[0]));

	LOG("Sampling Animations");
	unsigned int numAnims = scene->mNumAnimations;
	mesh->animations.resize(numAnims);
	for (unsigned int i = 0; i < numAnims; i++)
	{
		aiAnimation* currentAssimpAnim = scene->mAnimations[i];
		Animation* currentAnim = &mesh->animations[i];
		if (currentAssimpAnim == NULL)
		{
			LOG("Anim is null: %d", i);
			continue;
		}
		LOG(currentAssimpAnim->mName.C_Str());
		LOG("assimp mTicksPerSecond: %f", currentAssimpAnim->mTicksPerSecond);
		LOG("assimp mDuration: %f", currentAssimpAnim->mDuration);

		currentAnim->name = currentAssimpAnim->mName.C_Str();
		currentAnim->keyframes.resize(numBones);

		//TODO: check this as soon as I fix the problem with the fbx/dae assimp differences
		float totalTimeSecs = (1.0f / (float)currentAssimpAnim->mTicksPerSecond) * (float)currentAssimpAnim->mDuration;
		int numSamples = (int)std::floor(((float)mesh->samplesPerSecond * totalTimeSecs));
		currentAnim->numSamples = numSamples;
		currentAnim->samplesPerSecond = mesh->samplesPerSecond;
		LOG("currentAnim->ticksPerSecond: %f", currentAnim->samplesPerSecond);
		LOG("currentAnim->numSamples: %f", currentAnim->numSamples);
		LOG("totalTimeSecs: %f", totalTimeSecs);

		//reserve space for all samples
		for (int j = 0; j < numBones; j++) currentAnim->keyframes[i].reserve(numSamples);

		//make all the samples
		for (int j = 0; j < numSamples; j++)
		{
			LOG("--- START SAMPLE --- %d", j);
			float currentTime = ((1.0f / mesh->samplesPerSecond) * j);
			if (j == numSamples - 1)
			{
				LOG("LastSample: %f", 1.0f);
				SaveAnimationSample(mesh, &(mesh->nodes[0]), currentAssimpAnim, currentAnim, totalTimeSecs, fbx);
			}
			else
			{
				LOG("CurrentTime: %f", currentTime);
				SaveAnimationSample(mesh, &(mesh->nodes[0]), currentAssimpAnim, currentAnim, currentTime, fbx);
			}

			LOG("---- END SAMPLE ---- %d", j);
		}
	}

	mesh->SetBindPose();
	importer.FreeScene();
	orderedNodes.clear();

	mesh->InitGL();
	if (scene->HasAnimations())
		mesh->InitBonesGL();
}

void loadASE(Mesh * mesh, const std::string & path)
{
	std::string fullpath = Path::GetPath(path);
	LOG("loading ASE Mesh: %s", fullpath);

	TextParser parser;
	if (!parser.create(fullpath.c_str()))
	{
		LOGERROR("load failed: %s", fullpath);
		return;
	}

	parser.seek("*MESH_NUMVERTEX");
	int numverts = parser.getint();
	parser.seek("*MESH_NUMFACES");
	int numfaces = parser.getint();

	//parse verts
	LOG("parsing vertices ");
	std::vector<Vector3> allverts;
	allverts.reserve(numverts);
	parser.seek("*MESH_VERTEX_LIST");
	for (int i = 0; i < numverts; i++)
	{
		parser.seek("*MESH_VERTEX");
		parser.getint();
		Vector3 vert;
		vert.x = (float)parser.getfloat();
		vert.z = -(float)(parser.getfloat());
		vert.y = (float)parser.getfloat();
		allverts.push_back(vert);
	}

	//parse faces
	LOG("parsing faces");
	parser.seek("*MESH_FACE_LIST");
	mesh->verts.reserve(numfaces);
	for (int i = 0; i < numfaces; i++)
	{
		parser.seek("*MESH_FACE");
		parser.getint();
		parser.seek("A:");
		int a = parser.getint();
		parser.seek("B:");
		int b = parser.getint();
		parser.seek("C:");
		int c = parser.getint();
		mesh->verts.push_back(allverts[a]);
		mesh->verts.push_back(allverts[b]);
		mesh->verts.push_back(allverts[c]);
	}

	allverts.clear();

	//parse uvs
	LOG("parsing uvs");
	parser.seek("*MESH_NUMTVERTEX");
	if (parser.eof()) return;

	int num_uvs = parser.getint();
	mesh->uvs.reserve(num_uvs);
	for (int i = 0; i < num_uvs; i++)
	{
		parser.seek("*MESH_TVERT");
		parser.getint();
		float u = (float)parser.getfloat();
		float v = (float)parser.getfloat();

		mesh->uvs.push_back(Vector2(u, v));
	}


	//parse normals
	LOG("parsing normals");
	parser.seek("*MESH_NORMALS");
	if (parser.eof()) return;

	mesh->normals.resize(numverts);
	for (int i = 0; i < (numfaces * 3); i++)
	{
		parser.seek("*MESH_VERTEXNORMAL");
		int index = parser.getint();
		Vector3 n;
		n.x = (float)parser.getfloat();
		n.y = (float)parser.getfloat();
		n.z = (float)parser.getfloat();
		mesh->normals[index] = n;
	}
	LOG("numTotalVerts: %d", numfaces * 3);
	LOG("done loading: %s", fullpath);
	//materials etc
	mesh->InitGL();
}

void SaveBinary(SkinnedMesh * mesh, const char* path)
{
	FILE *fp;
	std::string fullpath = Path::GetPath(path);
	remove(fullpath.c_str());
	fopen_s(&fp, fullpath.c_str(), "wb+");

	SerializeVector(mesh->verts, fp);
	SerializeVector(mesh->normals, fp);
	SerializeVector(mesh->uvs, fp);
	SerializeVector(mesh->colors, fp);
	SerializeVector(mesh->boneIds, fp);
	SerializeVector(mesh->weights, fp);

	int numBones = mesh->boneNames.size();
	fwrite(&numBones, sizeof(int), 1, fp);
	for (int i = 0; i < numBones; i++)
		SerializeString(mesh->boneNames[i], fp);

	for (int i = 0; i < numBones; i++)
	{
		fwrite(&(mesh->nodes[i].parentIndex), sizeof(int), 1, fp);
		fwrite(&(mesh->nodes[i].index), sizeof(int), 1, fp);
		SerializeVector(mesh->nodes[i].childIndices, fp);
	}

	SerializeVector(mesh->boneMatrices, fp);
	SerializeVector(mesh->bindMatrices, fp);
	SerializeVector(mesh->invBindMatrices, fp);

	int numAnims = mesh->animations.size();
	fwrite(&numAnims, sizeof(int), 1, fp);

	for (int i = 0; i < numAnims; i++)
	{
		Animation animation = mesh->animations[i];
		SerializeCharArray(animation.name, fp);
		fwrite(&(animation.samplesPerSecond), sizeof(double), 1, fp);
		fwrite(&(animation.numSamples), sizeof(int), 1, fp);

		for (size_t i = 0; i < animation.keyframes.size(); i++)
			SerializeVector(animation.keyframes[i], fp);
	}
	fclose(fp);
}

void LoadBinary(SkinnedMesh* mesh, const char* path)
{
	std::string fullpath = Path::GetPath(path);

	FILE *fp;
	fopen_s(&fp, fullpath.c_str(), "rb");

	size_t lastRead = 0;
	//TODO: set a flag that tells what kind of asset it is
	DeserializeVector(mesh->verts, fp);
	DeserializeVector(mesh->normals, fp);
	DeserializeVector(mesh->uvs, fp);
	DeserializeVector(mesh->colors, fp);
	DeserializeVector(mesh->boneIds, fp);
	DeserializeVector(mesh->weights, fp);

	int numBones = 0;
	lastRead = fread(&numBones, sizeof(int), 1, fp);
	mesh->boneNames.resize(numBones);
	for (int i = 0; i < numBones; i++)
		DeserializeString(mesh->boneNames[i], fp);


	mesh->nodes.resize(numBones);
	for (int i = 0; i < numBones; i++)
	{
		fread(&(mesh->nodes[i].parentIndex), sizeof(int), 1, fp);
		fread(&(mesh->nodes[i].index), sizeof(int), 1, fp);
		DeserializeVector(mesh->nodes[i].childIndices, fp);
	}

	DeserializeVector(mesh->boneMatrices, fp);
	DeserializeVector(mesh->bindMatrices, fp);
	DeserializeVector(mesh->invBindMatrices, fp);

	int numAnims = 0;
	fread(&numAnims, sizeof(int), 1, fp);
	mesh->animations.resize(numAnims);
	for (int i = 0; i < numAnims; i++)
	{
		//std::string name = "";
		char* name = NULL;
		DeserializeCharArray(name, fp);
		mesh->animations[i].name = name;
		double samplesPerSec = 0;
		fread(&samplesPerSec, sizeof(double), 1, fp);
		mesh->animations[i].samplesPerSecond = samplesPerSec;
		int numSamples = 0;
		fread(&numSamples, sizeof(int), 1, fp);
		mesh->animations[i].numSamples = numSamples;

		mesh->animations[i].keyframes.resize(numBones);
		for (int j = 0; j < numBones; j++)
			DeserializeVector(mesh->animations[i].keyframes[j], fp);
	}
	fclose(fp);

	mesh->currentPoseModelToBoneMatrices.resize(numBones);
	mesh->SetBindPose();
	mesh->InitGL();
	mesh->InitBonesGL();
}

void generateQuad(Mesh * mesh, Vector3 color)
{
	mesh->verts.resize(6);
	mesh->verts[0] = Vector3(1.0f, -1.0f, 0.0f);
	mesh->verts[1] = Vector3(-1.0f, 1.0f, 0.0f);
	mesh->verts[2] = Vector3(-1.0f, -1.0f, 0.0f);

	mesh->verts[3] = Vector3(1.0f, 1.0f, 0.0f);
	mesh->verts[4] = Vector3(-1.0f, 1.0f, 0.0f);
	mesh->verts[5] = Vector3(1.0f, -1.0f, 0.0f);

	mesh->uvs.resize(6);
	mesh->uvs[0] = Vector2(1.0f, 0.0f);
	mesh->uvs[1] = Vector2(0.0f, 1.0f);
	mesh->uvs[2] = Vector2(0.0f, 0.0f);

	mesh->uvs[3] = Vector2(1.0f, 1.0f);
	mesh->uvs[4] = Vector2(0.0f, 1.0f);
	mesh->uvs[5] = Vector2(1.0f, 0.0f);

	mesh->colors.resize(6);
	mesh->colors[0] = color;
	mesh->colors[1] = color;
	mesh->colors[2] = color;

	mesh->colors[3] = color;
	mesh->colors[4] = color;
	mesh->colors[5] = color;

	mesh->InitGL();
}

void generateQuad(Mesh * mesh)
{
	mesh->verts.resize(6);
	mesh->verts[0] = Vector3(1.0f, -1.0f, 0.0f);
	mesh->verts[1] = Vector3(-1.0f, 1.0f, 0.0f);
	mesh->verts[2] = Vector3(-1.0f, -1.0f, 0.0f);

	mesh->verts[3] = Vector3(1.0f, 1.0f, 0.0f);
	mesh->verts[4] = Vector3(-1.0f, 1.0f, 0.0f);
	mesh->verts[5] = Vector3(1.0f, -1.0f, 0.0f);

	mesh->uvs.resize(6);
	mesh->uvs[0] = Vector2(1.0f, 0.0f);
	mesh->uvs[1] = Vector2(0.0f, 1.0f);
	mesh->uvs[2] = Vector2(0.0f, 0.0f);

	mesh->uvs[3] = Vector2(1.0f, 1.0f);
	mesh->uvs[4] = Vector2(0.0f, 1.0f);
	mesh->uvs[5] = Vector2(1.0f, 0.0f);

	mesh->InitGL();
}
