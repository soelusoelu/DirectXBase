Texture2D g_tex : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix g_mW; //ワールド行列
    matrix g_mWVP; //ワールドから射影までの変換行列
};
cbuffer global_1 : register(b1)
{
    bool g_TextureFlag : packoffset(c0) = false; //テクスチャーが貼られているメッシュかどうかのフラグ
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
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
};

//
//テクスチャーにレンダー バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(Pos, g_mWVP);
    output.WorldPos = mul(Pos, g_mW);
    //Norm.w = 0;
    output.WorldNormal = mul(Norm, g_mW);

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
    Out.vColor = g_tex.Sample(g_samLinear, input.UV);

    //ワールド法線テクスチャーへ出力
    float4 vNormal = input.WorldNormal;
    vNormal = vNormal * 0.5 + 0.5;
    Out.vNormal = vNormal;
    Out.vNormal.a = 1;

    //ワールド座標テクスチャーへ出力
    Out.vPosition = input.WorldPos;
    Out.vPosition.a = 1;

    return Out;
}
