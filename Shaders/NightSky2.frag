#version 330 core
in vec3 vDir;  // 頂点シェーダから渡された方向ベクトル
out vec4 fragColor;

uniform vec3 uSkyColor = vec3(0, 0, 0.3);

void main() {
    vec3 dir = normalize(vDir);

    // 方向ベクトルを使って乱数を生成（座標→一意の乱数へ）
    vec3 p = fract(dir * vec3(123.9898, 478.233, 645.164));
    p += dot(p, p+78.233);
    float noise = fract(p.x * p.y * p.z);

    // 星の出現確率を制御（0.999 以上なら星にする）
    float star = step(0.99, noise);

    // 星があれば白、なければ背景色
    vec3 color = mix(uSkyColor, vec3(1.0), star);

    fragColor = vec4(color, 1.0);
}
