#version 430

layout (location = 0) in vec4 aPosition;
layout (location = 1) in mat4 aModelMatrix;
layout (location = 5) in int aTextureIndex;

out vec2 vUv;
flat out int vTextureIndex;

uniform mat4 projection;

void main()
{
    gl_Position = projection * aModelMatrix * vec4(aPosition.xy, 0.0, 1.0);

    vUv = aPosition.zw;
    vTextureIndex = aTextureIndex;
}
