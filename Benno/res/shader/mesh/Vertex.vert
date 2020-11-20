#version 330

layout (location = 0) in vec4 aPosition;

out vec2 vUv;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(aPosition.xy, 0.0, 1.0);
    vUv = aPosition.zw;
}
