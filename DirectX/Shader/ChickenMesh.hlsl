Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix mW : packoffset(c0); //ワールド行列
    matrix mWVP : packoffset(c4); //ワールドから射影までの変換行列
    float3 mPos : packoffset(c8);
    float3 mUpColor : packoffset(c9);
    float3 mBottomColor : packoffset(c10);
    float3 mLeftColor : packoffset(c11);
    float3 mRightColor : packoffset(c12);
    float3 mForeColor : packoffset(c13);
    float3 mBackColor : packoffset(c14);
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
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //射影変換（ワールド→ビュー→プロジェクション）
    //法線をワールド空間に
    Norm.w = 0; //移動成分を反映させない
    output.Normal = mul(Norm, mW);
    output.Pos = mul(Pos, mWVP);
    output.WorldPos = mul(Pos, mW);

    return output;
}

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
    if (input.WorldPos.x > mPos.x + 0.4)
    {
        color.xyz = mRightColor;
    }
    //左
    else if (input.WorldPos.x < mPos.x - 0.4)
    {
        color.xyz = mLeftColor;
    }
    //手前
    if (input.WorldPos.z < mPos.z - 0.5)
    {
        color.xyz = mForeColor;
    }
    //奥
    else if (input.WorldPos.z > mPos.z + 0.5)
    {
        color.xyz = mBackColor;
    }
    color = saturate(color);
    //if (g_Texture == 1)
    //{
    //    color = g_texDecal.Sample(g_samLinear, input.Tex);
    //}
    color.a = g_Diffuse.a;

    return color;
}