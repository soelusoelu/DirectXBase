Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    float3 mLightDir : packoffset(c0); //ライトの方向ベクトル
    float3 mCameraPos : packoffset(c1); //カメラ位置
    float3 mAmbientLight : packoffset(c2); //環境光
};

//バーテックスバッファー出力
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

//
//テクスチャーを参照してレンダー バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = Pos;
    output.UV = UV;

    return output;
}

//
//テクスチャーを参照してレンダー ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    //テクスチャーから情報を取り出す
    float3 gbufferDiffuse = g_texColor.Sample(g_samLinear, input.UV).xyz;
    float3 gbufferWorldNormal = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 worldNormal = gbufferWorldNormal * 2.0 - 1.0; //デコード
    float3 gbufferWorldPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;

    //取り出した情報をもとにフォンシェーディングを計算
    float3 N = normalize(worldNormal);
    float3 L = normalize(mLightDir);
    float3 V = normalize(mCameraPos - gbufferWorldPos);
    float3 R = normalize(reflect(-L, N));

    float3 Phong = mAmbientLight;
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        float3 Diffuse = NdotL;
        float3 Specular = pow(max(0.0, dot(R, V)), 1.0);
        Phong += Diffuse + Specular;
    }

    Phong = saturate(Phong);

    return float4(gbufferDiffuse * Phong, 1.0);
}