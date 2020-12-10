#version 430

layout (location = 0) in vec4 aPosition;
layout (location = 1) in mat4 aModelMatrix;
layout (location = 5) in int aTextureIndex;
layout (location = 6) in float aTextureHeight;
layout (location = 7) in vec3 aIntensity;

out vec2 vUv;
flat out int vTextureIndex;
flat out vec3 vIntensity;

uniform mat4 viewProjection;
uniform float maxY;

void main()
{
    gl_Position = viewProjection * aModelMatrix * vec4(aPosition.xy, 0.0, 1.0);

    vUv = aPosition.zw;
    if (vUv.y == 1.0) {
        vUv.y = aTextureHeight / maxY;
    }

    vTextureIndex = aTextureIndex;
    vIntensity = aIntensity;
}
