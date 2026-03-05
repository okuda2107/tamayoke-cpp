#version 330 core
out vec4 FragColor;

in vec3 vPos;

uniform vec3 glowColor = vec3(1.0, 0.3, 0);  // 発光色
uniform float radius = 2;     // 発光の広がり

void main()
{
    // 中心からの距離を計算
    float dist = length(vPos);

    // 外側でフェードアウト（0～1）
    float intensity = 1.0 - smoothstep(0.0, radius, dist);

    // 加算ブレンド用の発光色
    FragColor = vec4(glowColor * intensity, intensity);
}
