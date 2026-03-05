#version 330

uniform sampler2D uTexture;

in vec2 flagTexCoord;

out vec4 outColor;

void main()
{
	outColor = texture(uTexture, flagTexCoord);
}