Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

SamplerState g_samLinear : register(s0);

cbuffer PointLight : register(b0)
{
    matrix mWVP : packoffset(c0);
    float3 mWorldPos : packoffset(c4);
    float3 mDiffuseColor : packoffset(c5);
    float mInnerRadius : packoffset(c6);
    float mOuterRadius : packoffset(c7);
    float2 mWindowSize : packoffset(c8);
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(Pos, mWVP);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float2 gbufferCoord = input.Pos.xy / mWindowSize;

    //テクスチャーから情報を取り出す
    float3 gbufferDiffuse = g_texColor.Sample(g_samLinear, gbufferCoord).xyz;
    float3 gbufferWorldNormal = g_texNormal.Sample(g_samLinear, gbufferCoord).xyz;
    float3 gbufferWorldPos = g_texPosition.Sample(g_samLinear, gbufferCoord).xyz;

    float3 N = normalize(gbufferWorldNormal);
    float3 L = normalize(mWorldPos - gbufferWorldPos);

    float3 Phong = float3(0.0, 0.0, 0.0);
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        float dist = distance(mWorldPos, gbufferWorldPos);
        float intensity = smoothstep(mInnerRadius, mOuterRadius, dist);
        float3 DiffuseColor = lerp(mDiffuseColor, float3(0.0, 0.0, 0.0), intensity);
        Phong = DiffuseColor * NdotL;
    }

    return float4(gbufferDiffuse * Phong, 1.0);
}
