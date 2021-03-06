Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix g_mW : packoffset(c0); //ワールド行列
    matrix g_mWVP : packoffset(c4); //ワールドから射影までの変換行列
    float3 g_vLightDir : packoffset(c8); //ライトの方向ベクトル
    float3 g_vEye : packoffset(c9); //カメラ位置
};

cbuffer global_1 : register(b1)
{
    float4 g_Diffuse : packoffset(c0) = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_Specular : packoffset(c1) = float4(1, 1, 1, 1); //鏡面反射
    float g_Texture : packoffset(c2) = 0; //テクスチャーが貼られているメッシュかどうかのフラグ
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 Color : COLOR0;
    float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
    float2 Tex : TEXCOORD3;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
    Norm.w = 0; //移動成分を反映させない
    output.Normal = mul(Norm, g_mW);
    output.Pos = mul(Pos, g_mWVP);
    output.WorldPos = mul(Pos, g_mW);
	//ライト方向
    output.Light = g_vLightDir;
	//視線ベクトル
    float3 PosWorld = mul(Pos, g_mW);
    output.EyeVector = g_vEye - PosWorld;
	
    float3 Normal = normalize(output.Normal);
    float3 LightDir = normalize(output.Light);
    float3 ViewDir = normalize(output.EyeVector);
    float NL = saturate(dot(Normal, LightDir));
	
    float3 Reflect = normalize(2 * NL * Normal - LightDir);
    float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

    output.Color = g_Diffuse * NL + specular * g_Specular;
	
	//テクスチャー座標
    output.Tex = UV;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = g_Diffuse;
    //float4 color = input.Color;
    if (g_Texture == 1)
    {
        color = g_texDecal.Sample(g_samLinear, input.Tex);
    }
    color.a = g_Diffuse.a;

    return color;
}