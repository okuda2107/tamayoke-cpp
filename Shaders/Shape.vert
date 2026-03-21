#version 330 core

layout(location=0) in vec3 position;

uniform mat4 uViewProj;
uniform vec3 fragColor;

out vec3 vColor;

void main()
{
    vColor = fragColor;
    gl_Position = uViewProj * vec4(position,1.0);
}
