#version 330
in vec3 vDir;
out vec4 outColor;

uniform float uTime;   // 経過秒
uniform vec3  uHorizonColor = vec3(0.01, 0.02, 0.1);
uniform vec3  uZenithColor  = vec3(0.00, 0.00, 0.02);

// 2Dハッシュ（セル→[0,1)）
// 決定的な乱数じゃないと，計算する度に変わってしまい画面全体がノイズっぽくなる
float hash21(vec2 p){
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 78.233);
    return fract(p.x * p.y);
}

// 2Dハッシュでセル内オフセット
vec2 hash22(vec2 p){
    // 2Dベクトルを受け取り、[0,1)範囲の乱数ベクトルを返す
    return fract(
        sin(vec2(
            dot(p, vec2(127.1, 311.7)),
            dot(p, vec2(269.5, 183.3))
        )) * 43758.5453
    );
}

// ラジアン/π の定数
const float PI = 3.14159265359;

// dir→擬似UV（正距円筒）。内部でしか使わないのでシームは目立ちにくい。
vec2 dirToUV(vec3 d){
    d = normalize(d);
    float lon = atan(d.y, d.x);     // -PI..PI
    float lat = asin(clamp(d.z, -1.0, 1.0)); // -PI/2..PI/2
    vec2 uv = vec2(lon/(2.0*PI) + 0.5, lat/PI + 0.5);
    return fract(uv);
}

void main(){
    // 天球の座標
    vec3 d = normalize(vDir);

    // 背景グラデーション（地平線→天頂）
    float h = clamp(d.z * 0.5 + 0.5, 0.0, 1.0);
    vec3 sky = mix(uHorizonColor, uZenithColor, h);

    vec2 uv = dirToUV(d);

    // 星レイヤ（スケール違いで3層）
    float starLum = 0.0;
    vec3 starCol  = vec3(0.0);

    // レイヤーパラメータ
    for(int i=0;i<3;i++){
        float scale     = 300.0 * pow(1.9, i); // セル密度
        float threshold = mix(0.985, 0.99, float(i)/2.0); // 星が出る確率（上層ほど疎）
        float size      = mix(0.55, 0.35, float(i)/2.0);   // 星サイズ（セル内半径）

        vec2  suv   = uv * scale;
        vec2  cell  = floor(suv);
        vec2  f     = fract(suv);

        float rnd   = hash21(cell);

        // 星があるセルか
        if(rnd > threshold){
            // セル内の中心位置と色/明るさ
            vec2  c     = hash22(cell);
            float dist  = length(f - c);
            // 柔らかいコア（ガウシアン風）
            // float core  = exp(-pow(dist / size, 2.0) * 12.0);
            float core = exp(-pow(dist / (size * 0.45), 2.0) * 20.0);

            // 星色：僅かに色温度差
            float warm  = hash21(cell + 17.0);
            vec3  color = mix(vec3(0.75,0.80,1.0), vec3(1.0,0.94,0.85), smoothstep(0.0,1.0,warm));

            // 基本輝度
            float baseB = mix(0.6, 1.6, hash21(cell + 3.7));

            // 瞬き：セルごとに位相/速さをずらす
            float phase = hash21(cell + 9.1) * 6.28318;
            float rate  = mix(0.7, 2.3, hash21(cell + 2.3)); // Hz 風
            float twk   = 0.5 + 0.5 * sin(uTime * rate + phase);
            // 微小ランダムで瞬きの揺らぎ
            float twk2  = 0.85 + 0.3 * sin(uTime * (rate*0.37) + phase*1.7);

            float starB = baseB * mix(0.2, 1.5, twk) * twk2;

            // コロナ（十字スパイクのような微妙な広がり）
            // float spike = pow(max(0.0, 1.0 - dist/ (size*2.0)), 3.0);
            float spike = exp(-pow(dist / (size * 1.4), 2.0) * 6.0);

            float lum   = core + 0.12 * spike;

            starLum += lum * starB * 0.7;
            starCol += color * lum * starB;
        }
    }

    // 大気減光：地平線付近の星を少し弱める
    float extinction = smoothstep(0.05, 0.25, h); // h=0（地平）→弱、h=1（天頂）→強
    starCol *= extinction;

    // 合成（加算+軽いスクリーン調）
    vec3 col = sky + starCol + (1.0 - (1.0 - sky) * exp(-starCol));

    // クリップ抑制のための軽いトーンマップ
    col = col / (1.0 + max(0.0, dot(col, vec3(0.2126,0.7152,0.0722))));

    // ガンマ
    col = pow(col, vec3(1.0/2.2));

    outColor = vec4(col, 1.0);
}
