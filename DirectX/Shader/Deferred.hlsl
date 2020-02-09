Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    float3 g_vLightDir : packoffset(c0); //���C�g�̕����x�N�g��
    float3 g_vEye : packoffset(c1); //�J�����ʒu
};

cbuffer global_1 : register(b1)
{
    float4 g_Ambient = float4(0, 0, 0, 0); //�A���r�G���g��
    float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 g_Specular = float4(1, 1, 1, 1); //���ʔ���
};

//�o�[�e�b�N�X�o�b�t�@�[�o��
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

//
//�e�N�X�`���[���Q�Ƃ��ă����_�[ �o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = Pos;
    output.UV = UV;

    return output;
}

//
//�e�N�X�`���[���Q�Ƃ��ă����_�[ �s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    //�e�N�X�`���[����������o��
    float4 vDiffuse = g_texColor.Sample(g_samLinear, input.UV);
    float3 vWorldNormal = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 vWorldPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    //���o�����������ƂɃt�H���V�F�[�f�B���O���v�Z
    float3 vLightDir = normalize(g_vLightDir - vWorldPos);
    float3 vEyeVec = g_vEye.xyz;
    float3 vDiffuseIntensity = dot(vLightDir, vWorldNormal);
    float3 vSpecularIntensity = pow(max(0, dot(vEyeVec, reflect(-vLightDir, vWorldNormal))), 4);

    float4 FinalColor;
    FinalColor.rgb = vDiffuseIntensity * vDiffuse.rgb + vSpecularIntensity * 1.0;

    return FinalColor;
}