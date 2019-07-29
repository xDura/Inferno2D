#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aWeights;
layout (location = 3) in vec4 aBoneIds;
out vec2 TexCoord;
uniform mat4 M;
uniform mat4 boneMats[80];

void main()
{

	mat4 boneTransform = boneMats[int(aBoneIds[0])] * aWeights[0];
	boneTransform += boneMats[int(aBoneIds[1])] * aWeights[1];
	boneTransform += boneMats[int(aBoneIds[2])] * aWeights[2];
	boneTransform += boneMats[int(aBoneIds[3])] * aWeights[3];

	TexCoord = aTexCoord;
	gl_Position = M * boneTransform * vec4(aPos, 1.0);
}