//�O���[�o��
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer global_0 : register(b0)
{
    matrix g_mW : packoffset(c0); //���[���h�s��
    matrix g_mWVP : packoffset(c4); //���[���h����ˉe�܂ł̕ϊ��s��
    float3 mUpColor : packoffset(c8);
    float3 mBottomColor : packoffset(c9);
};

cbuffer global_1 : register(b1)
{
    float4 g_Diffuse : packoffset(c0) = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 g_Specular : packoffset(c1) = float4(1, 1, 1, 1); //�g��Ȃ�����
    float g_Texture : packoffset(c2) = 0; //�e�N�X�`���[���\���Ă��郁�b�V�����ǂ����̃t���O
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 Normal : TEXCOORD1;
    float2 Tex : TEXCOORD3;
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
    Norm.w = 0; //�ړ������𔽉f�����Ȃ�
    output.Normal = mul(Norm, g_mW);
    output.Pos = mul(Pos, g_mWVP);
    output.WorldPos = mul(Pos, g_mW);
	
	//�e�N�X�`���[���W
    output.Tex = UV;

    return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = g_Diffuse;
    if (input.WorldPos.y > 0.0)
    {
        color.xyz = mUpColor;
    }
    else
    {
        color.xyz = mBottomColor;
    }
    //if (g_Texture == 1)
    //{
    //    color = g_texDecal.Sample(g_samLinear, input.Tex);
    //}
    color.a = g_Diffuse.a;

    return color;
}