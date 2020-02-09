Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    float3 g_vLightDir : packoffset(c0); //ライトの方向ベクトル
    float3 g_vEye : packoffset(c1); //カメラ位置
};

cbuffer global_1 : register(b1)
{
    float4 g_Ambient = float4(0, 0, 0, 0); //アンビエント光
    float4 g_Diffuse = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_Specular = float4(1, 1, 1, 1); //鏡面反射
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
    float4 vDiffuse = g_texColor.Sample(g_samLinear, input.UV);
    float3 vWorldNormal = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 vWorldPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    //取り出した情報をもとにフォンシェーディングを計算
    float3 vLightDir = normalize(g_vLightDir - vWorldPos);
    float3 vEyeVec = g_vEye.xyz;
    float3 vDiffuseIntensity = dot(vLightDir, vWorldNormal);
    float3 vSpecularIntensity = pow(max(0, dot(vEyeVec, reflect(-vLightDir, vWorldNormal))), 4);

    float4 FinalColor;
    FinalColor.rgb = vDiffuseIntensity * vDiffuse.rgb + vSpecularIntensity * 1.0;

    return FinalColor;
}