float3 blendOverlay(float3 base, float3 blend, float opacity)
{
    float3 result1 = 1.f - 2.f * (1.f - base) * (1.f - blend);
    float3 result2 = 2.f * base * blend;
    float3 zeroOrOne = step(base, 0.5f);
    float3 output = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
    return lerp(base, output, opacity);
}

float Remap(float inValue, float2 inMinMax, float2 outMinMax)
{
    return outMinMax.x + (inValue - inMinMax.x) * (outMinMax.y - outMinMax.x) / (inMinMax.y - inMinMax.x);
}

float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float noise(float2 uv, float scale)
{
    float2 p = floor(uv * scale);
    return random(p);
}

float2 random2(float2 st)
{
    st = float2(dot(st, float2(127.1f, 311.7f)),
                dot(st, float2(269.5f, 183.3f)));
    
    return -1.f + 2.f * frac(sin(st) * 43758.5453f);
}

float perlinNoise(float2 uv, float scale)
{
    float2 p = floor(uv * scale);
    float2 f = frac(uv * scale);
    float2 u = f * f * (3.f - 2.f * f);
    
    float2 v00 = random2(p + float2(0, 0));
    float2 v10 = random2(p + float2(1, 0));
    float2 v01 = random2(p + float2(0, 1));
    float2 v11 = random2(p + float2(1, 1));
    
    return lerp(lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
                lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
                u.y) + 0.5f;
}

float valueNoise(float2 uv, float scale)
{
    float t = 0.f;
    
    for (int i = 0; i < 3; i++)
    {
        float freq = pow(2.f, float(i));
        float amp = pow(0.5f, float(3 - i));
        t += perlinNoise(uv, scale / freq) * amp;
    }

    return t;
}
