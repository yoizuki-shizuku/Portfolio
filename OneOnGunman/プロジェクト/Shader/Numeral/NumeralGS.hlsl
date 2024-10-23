#include "Numeral.hlsli"

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
void main(
	point PS_INPUT input[1], 
	inout TriangleStream< PS_INPUT > output
)
{
	for (unsigned int i = 0; i < vnum; i++)
	{
		//	�W�I���g���o��
        PS_INPUT element;
        
        //  �󂯎��������ϐ��֑��
        float2 pos = input[0].pos.xy;
        float radian = input[0].pos.z;
        float2 size = input[0].tex;
        
        //  �s�N�Z���P�ʂɉ��������_�̈ʒu���v�Z
        element.pos.xy = size * offset_vertex[i];
        
        //  �摜�̒��S�����_�ɂȂ�悤�ɂ���
        element.pos.x -= size.x / 2.f;
        element.pos.y -= size.y / 2.f;
        
                //  ��]������
        float2 original = element.pos.xy;
        element.pos.x = (cos(radian) * original.x) + (-sin(radian) * original.y);
        element.pos.y = (sin(radian) * original.x) + (cos(radian) * original.y);

                //  �傫�����s�N�Z���P�ʂɂȂ�悤�v�Z
        element.pos.x *= 2.f / windowWidth;
        element.pos.y *= 2.f / windowHeight;
        
        //  ��ʂ̍����ɂ���
        element.pos.xy -= float2(1.f, 1.f);
        
        //  position�̈ʒu�ֈړ�������
        pos /= float2(windowWidth, windowHeight);
        pos *= 2.f;
        element.pos.xy += pos;
        
        //  y���𔽓]
        element.pos.y *= -1;
                
        //XY���ʂȂ̂� z��0�Aw��1
        element.pos.z = 0.0f;
        element.pos.w = 1.0f;

        //  �����ɍ��킹�ĉ摜��؂��邽�߂�uv���W���v�Z
        element.tex.x = (0.1f * numeral) + (0.1f * offset_vertex[i].x);
        element.tex.y = offset_vertex[i].y;
        
        output.Append(element);
    }
    output.RestartStrip();
}