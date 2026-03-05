#version 330 core

in vec3 vTexCoord;
out vec4 outColor;

// サンプラー
uniform samplerCube uSkybox;

void main()
{
    // サンプルテクスチャはキューブマップ
    outColor = vec4(vTexCoord, 1.0);
}
