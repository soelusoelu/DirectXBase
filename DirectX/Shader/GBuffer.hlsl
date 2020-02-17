Texture2D g_tex : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix g_mW; //���[���h�s��
    matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
};
cbuffer global_1 : register(b1)
{
    bool g_TextureFlag : packoffset(c0) = false; //�e�N�X�`���[���\���Ă��郁�b�V�����ǂ����̃t���O
}

//�o�[�e�b�N�X�o�b�t�@�[�o��
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};
//�s�N�Z���V�F�[�_�[�o��
struct PS_OUTPUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
};

//
//�e�N�X�`���[�Ƀ����_�[ �o�[�e�b�N�X�V�F�[�_�[
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
//�e�N�X�`���[�Ƀ����_�[ �s�N�Z���V�F�[�_�[
//
PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

    //�J���[�e�N�X�`���[�֏o��
    Out.vColor = g_tex.Sample(g_samLinear, input.UV);

    //���[���h�@���e�N�X�`���[�֏o��
    float4 vNormal = input.WorldNormal;
    vNormal = vNormal * 0.5 + 0.5;
    Out.vNormal = vNormal;
    Out.vNormal.a = 1;

    //���[���h���W�e�N�X�`���[�֏o��
    Out.vPosition = input.WorldPos;
    Out.vPosition.a = 1;

    return Out;
}
