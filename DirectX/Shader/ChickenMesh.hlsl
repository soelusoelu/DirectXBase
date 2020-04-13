//グローバル
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

//グローバル
cbuffer global_0 : register(b0)
{
    matrix mW : packoffset(c0); //ワールド行列
    matrix mWVP : packoffset(c4); //ワールドから射影までの変換行列
    float3 mPos : packoffset(c8);
    float3 mUpColor : packoffset(c9);
    float3 mBottomColor : packoffset(c10);
};

cbuffer global_1 : register(b1)
{
    float4 g_Diffuse : packoffset(c0) = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_Specular : packoffset(c1) = float4(1, 1, 1, 1); //使わないけど
    float g_Texture : packoffset(c2) = 0; //テクスチャーが貼られているメッシュかどうかのフラグ
};

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 Normal : TEXCOORD1;
    float2 Tex : TEXCOORD3;
};
//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
    Norm.w = 0; //移動成分を反映させない
    output.Normal = mul(Norm, mW);
    output.Pos = mul(Pos, mWVP);
    output.WorldPos = mul(Pos, mW);
	
	//テクスチャー座標
    output.Tex = UV;

    return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = g_Diffuse;
    if (input.WorldPos.y > mPos.y + 0.3)
    {
        color.xyz = mUpColor;
    }
    else if (input.WorldPos.y < mPos.y - 0.3)
    {
        color.xyz = mBottomColor;
    }
    //右
    if (mPos.x + 0.4 < input.WorldPos.x)
    {
        color.xyz = float3(1.0, 0.8, 0.8);
    }
    //左
    else if (mPos.x - 0.4 > input.WorldPos.x)
    {
        color.xyz = float3(0.2, 0.2, 0.2);
    }
    //手前
    if (mPos.z - 0.5 > input.WorldPos.z)
    {
        color.xyz = float3(0.4, 0.2, 0.1);
    }
    //奥
    else if (mPos.z + 0.5 < input.WorldPos.z)
    {
        color.xyz = float3(0.9, 0.7, 0.7);
    }
    color = saturate(color);
    //if (g_Texture == 1)
    //{
    //    color = g_texDecal.Sample(g_samLinear, input.Tex);
    //}
    color.a = g_Diffuse.a;

    return color;
}