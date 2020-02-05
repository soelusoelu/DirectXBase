//�O���[�o��
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer global_0 : register(b0)
{
    matrix g_mW : packoffset(c0); //���[���h�s��
    matrix g_mWVP : packoffset(c4); //���[���h����ˉe�܂ł̕ϊ��s��
    float3 g_vLightDir : packoffset(c8); //���C�g�̕����x�N�g��
    float3 g_vEye : packoffset(c9); //�J�����ʒu
};

cbuffer global_1 : register(b1)
{
    float4 g_Ambient : packoffset(c0) = float4(0, 0, 0, 0); //�A���r�G���g��
    float4 g_Diffuse : packoffset(c1) = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 g_Specular : packoffset(c2) = float4(1, 1, 1, 1); //���ʔ���
    float g_Texture : packoffset(c3) = 0; //�e�N�X�`���[���\���Ă��郁�b�V�����ǂ����̃t���O
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
    float2 Tex : TEXCOORD3;
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
    //�@�������[���h��Ԃ�
    output.Pos = mul(Pos, g_mWVP);
    Norm.w = 0; //�ړ������𔽉f�����Ȃ�
    output.Normal = mul(Norm, g_mW);
    //���C�g����
    output.Light = normalize(g_vLightDir);
    //�����x�N�g��
    float3 PosWorld = normalize(mul(Pos, g_mW));
    output.EyeVector = normalize(g_vEye - PosWorld);
    
    //�e�N�X�`���[���W
    if (g_Texture == 1)
    {
        output.Tex = Tex;
    }

    return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    //����
    float4 ambient = g_Ambient;
    //�g�U���ˌ�
    float NL = saturate(dot(input.Normal, input.Light));
    float4 tex = float4(0, 0, 0, 1);
    if (g_Texture == 1)
    {
        tex = g_texDecal.Sample(g_samLinear, input.Tex);
    }
    float4 diffuse = (g_Diffuse / 2 + tex / 2) * NL;
    //���ʔ��ˌ�
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular = pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;
    //�t�H�����f���ŏI�F 3�̌��̍��v
    float4 color = ambient + diffuse + specular;

    return color;
}