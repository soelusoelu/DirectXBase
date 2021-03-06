//グローバル
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix g_mW : packoffset(c0); //ワールド行列
    matrix g_mWVP : packoffset(c4); //ワールドから射影までの変換行列
    float3 g_vLightPos : packoffset(c8); //ライトの位置ベクトル
    matrix g_vLightDir : packoffset(c9); //ライトの方向ベクトル
    float3 g_vEye : packoffset(c13); //カメラ位置
};

cbuffer global_1 : register(b1)
{
    float4 g_Ambient : packoffset(c0) = float4(0, 0, 0, 0); //アンビエント光
    float4 g_Diffuse : packoffset(c1) = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_Specular : packoffset(c2) = float4(1, 1, 1, 1); //鏡面反射
    float g_Texture : packoffset(c3) = 0; //テクスチャーが貼られているメッシュかどうかのフラグ
};

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
    float2 Tex : TEXCOORD3;
};
//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //射影変換（ワールド→ビュー→プロジェクション）
    //法線をワールド空間に
    output.Pos = mul(Pos, g_mWVP);
    Norm.w = 0; //移動成分を反映させない
    output.Normal = mul(Norm, g_mW);
    //ライト方向
    output.WorldPos = mul(Pos, g_mW);
    output.Light = normalize(g_vLightPos - output.WorldPos);
    //視線ベクトル
    output.EyeVector = normalize(g_vEye - output.WorldPos);
    
    //テクスチャー座標
    if (g_Texture == 1)
    {
        output.Tex = Tex;
    }

    return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 lightBaseVector = float4(0, 1, 0, 1); //ライトの基準ベクトル

    //ライトの基準ベクトルに現在のライトの回転を反映
    lightBaseVector = mul(lightBaseVector, g_vLightDir);

    //環境光
    float4 ambient = g_Ambient;
    //拡散反射光
    float NL = saturate(dot(input.Normal, input.Light));
    float4 tex = float4(0, 0, 0, 1);
    if (g_Texture == 1)
    {
        tex = g_texDecal.Sample(g_samLinear, input.Tex);
    }
    float4 diffuse = (g_Diffuse / 2 + tex / 2) * NL;
    //鏡面反射光
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular = pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;
    //フォンモデル最終色 3つの光の合計
    float4 color = ambient + diffuse + specular;

    //スポットの範囲外のとき
    float cos = saturate(dot(lightBaseVector.xyz, input.Light));
    if (cos < 0.9)
    {
        color.rgb *= pow(cos / 3, 12 * (0.9 - cos));
    }

    //距離減衰
    float distance = length(g_vLightPos - input.WorldPos);
    color.rgb *= 1.0 / (0 + 0 * distance + 0.2 * distance * distance); // att = 1.0 / (a+b*d+c*d^2) d:距離 a,b,c：定数

    //ライトの強さ
    color.rgb *= 5.f;

    return color;
}