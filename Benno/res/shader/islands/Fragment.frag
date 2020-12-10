#version 430

out vec4 fragColor;

in vec2 vUv;
flat in int vTextureIndex;
flat in vec3 vIntensity;

uniform sampler2DArray sampler;

void main()
{
    vec3 uv = vec3(vUv, vTextureIndex);
    fragColor = texture(sampler, uv) * vec4(vIntensity, 1.0);
}
