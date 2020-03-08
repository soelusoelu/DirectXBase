//グローバル
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
    matrix mWorldProj;
    float4 gColor;
    float4 gUV;
};

//構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, mWorldProj);
    output.Tex = Tex * float2(gUV.z - gUV.x, gUV.w - gUV.y) + float2(gUV.x, gUV.y);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = g_texDecal.Sample(g_samLinear, input.Tex);
    color *= gColor;
    return color;
}