#version 420

layout(binding = 0) uniform sampler2D uTex0;

in vec2 flagTexCoord;

out vec4 outColor;

void main()
{
    outColor = texture(uTex0, flagTexCoord);
}
