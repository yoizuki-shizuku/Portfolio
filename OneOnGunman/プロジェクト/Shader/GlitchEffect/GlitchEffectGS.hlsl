#include "GlitchEffect.hlsli"

static const unsigned int vnum = 4;

//  �e���_�̈ʒu
static const float2 offset_vertex[vnum] =
{
    float2(0.f, 1.f), //  ����
    float2(1.f, 1.f), //  �E��
    float2(0.f, 0.f), //  ����
    float2(1.f, 0.f), //  �E��
};

[maxvertexcount(vnum)]
void main(point PS_INPUT input[1],
    inout TriangleStream<PS_INPUT> output)
{
    for (unsigned int i = 0; i < vnum; i++)
    {
        //  �W�I���g���o��
        PS_INPUT element;
        
        //  �󂯎��������ϐ��֑��
        float2 pos = input[0].pos.xy;
        float offsetPos = input[0].pos.z;
        float2 scale = input[0].color.xy;
        uint anchor = input[0].color.z;
        float offsetWidth = input[0].color.a;
        float2 size = input[0].tex.xy;
                
        //  �s�N�Z�������ʔ䗦�ɉ��������_�̈ʒu���v�Z
        element.pos.xy = scale * size * offset_vertex[i];
        
        //  �A���J�[�|�C���g�̈ʒu�֌��_�𓮂���
        element.pos.x -= (size.x * scale.x / 2.f) * (anchor % 3u);
        element.pos.y -= (size.y * scale.y / 2.f) * (anchor / 3u);
        
        //  ��]������
        //float2 original = element.pos.xy;
        //element.pos.x = (cos(radian) * original.x) + (-sin(radian) * original.y);
        //element.pos.y = (sin(radian) * original.x) + (cos(radian) * original.y);

        //  �傫�����s�N�Z���P�ʂɂȂ�悤�v�Z
        element.pos.x *= 2.f / windowSize.x;
        element.pos.y *= 2.f / windowSize.y;
        
        //  ��ʂ̍����ɂ���
        element.pos.xy -= float2(1.f, 1.f);
        
        //  position�̈ʒu�ֈړ�������
        pos /= windowSize.xy;
        pos *= 2.f;
        element.pos.xy += pos;
        
        //  y���𔽓]
        element.pos.y *= -1;
                
        //XY���ʂȂ̂� z��0�Aw��1
        element.pos.z = 0.0f;
        element.pos.w = 1.0f;
        
        //  �����𒸓_�ɓ���Ă���
        element.color = float4(offsetPos, offsetWidth, 1.f, 1.f);
        
        //  �e�N�X�`����UV���W
        element.tex = offset_vertex[i].xy;
        
        output.Append(element);
    }
    output.RestartStrip();
}