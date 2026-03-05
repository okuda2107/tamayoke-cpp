#version 330 core

layout(location = 0) in vec3 inPosition;

uniform mat4 uViewProj;
uniform mat4 uWorldTransform;

out vec3 vTexCoord;

void main()
{
    // 頂点位置をクリップ空間に変換
    gl_Position = vec4(inPosition, 1.0) * uWorldTransform * uViewProj;

    // テクスチャ用に球座標をそのまま出力（簡易）
    vTexCoord = normalize(inPosition);
}
