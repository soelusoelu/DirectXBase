Texture2D g_tex : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix mWorld; //ワールド行列
    matrix mWVP; //ワールドから射影までの変換行列
};

cbuffer global_1 : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float mTextureFlag;
}

//バーテックスバッファー出力
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};
//ピクセルシェーダー出力
struct PS_OUTPUT
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Specular : SV_Target3;
};

//
//テクスチャーにレンダー バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(Pos, mWVP);
    output.WorldPos = mul(Pos, mWorld);
    Norm.w = 0;
    output.WorldNormal = mul(Norm, mWorld);

    output.UV = UV;

    return output;
}

//
//テクスチャーにレンダー ピクセルシェーダー
//
PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

    //カラーテクスチャーへ出力
    Out.Color = mDiffuse;
    if (mTextureFlag == 1)
    {
        Out.Color = g_tex.Sample(g_samLinear, input.UV);
    }

    //ワールド法線テクスチャーへ出力
    float3 normal = input.WorldNormal.xyz;
    Out.Normal = float4(normal * 0.5 + 0.5, 1.0);

    //ワールド座標テクスチャーへ出力
    Out.Position = float4(input.WorldPos.xyz, 1.0);

    //スペキュラテクスチャへ出力
    Out.Specular = float4(mSpecular.xyz, 1.0);

    return Out;
}
