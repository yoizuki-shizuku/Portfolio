Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer ConstBuffer : register(b0)
{
    float4 Time;
}

float2 barrel(float2 uv)
{
    float s1 = 1.f, s2 = 0.125f;
    
    float2 centre = 2.f * uv - 1.f;
    float barrel = min(1.f - length(centre) * s1, 1.f) * s2;
    return uv - centre * barrel;
}

float2 CRT(float2 uv)
{
    float2 nu = 2.f * uv - 1.f;
    
    float2 offset = abs(nu.yx) / float2(6.f, 4.f);
    
    nu = nu + nu * offset * offset;
        
    return nu;
}

float scanline(float2 uv)
{
    float scanline = clamp(0.95f + 0.05f * cos(3.14f * (uv.y + 0.008f * floor(Time.x * 15.f) / 15.f) * 240.f * 1.f), 0.f, 1.f);
    float grille = 0.85f + 0.15f * clamp(1.5f * cos(3.14 * uv.x * 640.f * 1.f), 0.f, 1.f);
    return scanline * grille * 1.2f;
}

float4 main(float4 color : COLOR0,
    float2 texCoord : TEXCOORD0) : SV_Target0
{
    float2 uv = barrel(texCoord);
    
    float4 output = Texture.Sample(TextureSampler, uv);
    
    output.rgb *= scanline(texCoord);
    
    float2 crt = CRT(texCoord);
    crt = pow(crt, 16.f);
    output.rgb = lerp(output.rgb, 0.f.xxx, crt.x + crt.y);
    
	return output;
}