Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer ConstBuffer : register(b0)
{
    float Ratio;
    float Timer;
    float2 None;
}

float4 GrayScale(float4 color)
{
    float4 gray = float4(1.f, 1.f, 1.f, 1.f);
    gray.rgb = (color.r + color.g + color.b) * 0.333f;
    
    return gray;
}

float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float noice(float2 uv)
{
    float2 p = floor(uv);
    return random(p + Timer);
}

float4 main(float4 color : COLOR0,
    float2 texCoord : TEXCOORD0) : SV_Target0
{
    float4 colors = Texture.Sample(TextureSampler, texCoord);
    float4 monochro = GrayScale(colors);
    
    float4 output = lerp(colors, monochro, Ratio);
    
    output.rgb *= lerp(1.f, noice(texCoord * 350), Ratio / 2);
    
	return output;
}