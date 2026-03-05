#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 頂点座標をそのまま出す
out vec3 vPos;

void main()
{
    vPos = aPos;
    gl_Position = vec4(aPos, 1.0) * uWorldTransform * uViewProj;
}
