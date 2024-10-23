/*/
 *  �t�@�C����		�F	Collider.cpp
 *  �T�v			�F	�����蔻����_�u���f�B�X�p�b�`�Ŏ���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/01
/*/
#include "pch.h"
#include "Collider.h"

using namespace DirectX;

#include "Useful.h"
#include "UserResources.h"
#include "GeometricPrimitive.h"

namespace ColliderJudge
{
    /// <summary>
    /// �_�Ɛ��̍ŒZ����
    /// </summary>
    /// <param name="p">�_</param>
    /// <param name="s">�����̎n�_</param>
    /// <param name="v">�����̕����x�N�g��</param>
    /// <param name="h">�_����~�낵�������̑�(�߂�l)</param>
    /// <param name="t">�x�N�g���W��(�߂�l)</param>
    /// <returns>�ŒZ����</returns>
    float ClacPointLineDist(const DirectX::SimpleMath::Vector3& p,
        const DirectX::SimpleMath::Vector3& s,
        const DirectX::SimpleMath::Vector3& v,
        DirectX::SimpleMath::Vector3* h,
        float* t)
    {
        float lenSqV = v.LengthSquared();
        *t = 0.f;
        if (lenSqV > 0.f)
        {
            *t = v.Dot(p - s) / lenSqV;
        }

        *h = s + *t * v;
        return (*h - p).Length();
    }

    //	��̃x�N�g�����s�p�֌W���ǂ���
    static bool IsSharpAngle(const DirectX::SimpleMath::Vector3& p1,
        const DirectX::SimpleMath::Vector3& p2)
    {
        return p1.Dot(p2) >= 0.f;
    }

    //	�O�̈ʒu�֌W�������̓����ɂ��邩�ǂ���
    static bool SharpAngleThree(const DirectX::SimpleMath::Vector3& p1,
        const DirectX::SimpleMath::Vector3& p2,
        const DirectX::SimpleMath::Vector3& p3)
    {
        return IsSharpAngle((p1 - p2), (p3 - p2));
    }

    /// <summary>
    /// �_�Ɛ����̍ŒZ����
    /// </summary>
    /// <param name="p">�_</param>
    /// <param name="s">�����̎n�_</param>
    /// <param name="e">�����̏I�_</param>
    /// <param name="h">�ŒZ�����ƂȂ�[�_(�߂�l)</param>
    /// <param name="t">�[�_�ʒu(0���� : �n�_�̊O, 1���� : �I�_�̊O</param>
    /// <returns>�ŒZ����</returns>
    static float ClacPointSegmentDist(const DirectX::SimpleMath::Vector3& p,
        const DirectX::SimpleMath::Vector3& s,
        const DirectX::SimpleMath::Vector3& e,
        DirectX::SimpleMath::Vector3* h,
        float* t)
    {
        //	�����̒����A�����̑��̍��W�y��t���Z�o
        float len = ClacPointLineDist(p, s, (e - s), h, t);

        if (!SharpAngleThree(p, s, e))	//	�n�_���̊O��
        {
            *h = s;
            return (s - p).Length();
        }
        else if (!SharpAngleThree(p, e, s))	//	�I�_���̊O��
        {
            *h = e;
            return (e - p).Length();
        }

        return len;
    }

    //	���s���ǂ���
    static bool IsParallel(const DirectX::SimpleMath::Vector3& v1,
        const DirectX::SimpleMath::Vector3& v2)
    {
        float d = (v1.Cross(v2)).LengthSquared();
        return (abs(d) < FLT_EPSILON);		//	�덷�͈͓��Ȃ畽�s�Ɣ���
    }

    /// <summary>
    /// 2�����̍ŒZ����
    /// </summary>
    /// <param name="s1">����1�̎n�_</param>
    /// <param name="v1">����1�̕����x�N�g��</param>
    /// <param name="s2">����2�̎n�_</param>
    /// <param name="v2">����2�̕����x�N�g��</param>
    /// <param name="p1">��ڂ̐����ւ̐����̑�(�߂�l)</param>
    /// <param name="p2">��ڂ̐����ւ̐����̑�(�߂�l)</param>
    /// <param name="t1">��ڂ̐����̃x�N�g���W��(�߂�l)</param>
    /// <param name="t2">��ڂ̐����̃x�N�g���W��(�߂�l)</param>
    /// <returns>�ŒZ����</returns>
    static float ClacLineLineDist(const DirectX::SimpleMath::Vector3& s1,
        const DirectX::SimpleMath::Vector3& v1,
        const DirectX::SimpleMath::Vector3& s2,
        const DirectX::SimpleMath::Vector3& v2,
        DirectX::SimpleMath::Vector3* p1,
        DirectX::SimpleMath::Vector3* p2,
        float* t1,
        float* t2)
    {
        //	2���������s���ǂ���
        if (IsParallel(v1, v2))
        {
            //	�P�ڂ̒����̎n�_�Ɠ�ڂ̒����̍ŒZ�������ɋA��
            float len = ClacPointLineDist(s1, s2, v2, p2, t2);
            *p1 = s1;
            *t1 = 0.f;

            return len;
        }

        //	2�����͂˂���֌W
        float dv1v2 = v1.Dot(v2);
        float dv1v1 = v1.LengthSquared();
        float dv2v2 = v2.LengthSquared();
        SimpleMath::Vector3 s1s2 = s1 - s2;
        *t1 = (dv1v2 * v2.Dot(s1s2) - dv2v2 * v1.Dot(s1s2)) / (dv1v1 * dv2v2 - dv1v2 * dv1v2);
        *p1 = s1 + ((*t1) * v1);
        *t2 = v2.Dot((*p1) - s2) / dv2v2;
        *p2 = s2 + ((*t2) * v2);

        return ((*p2) - (*p1)).Length();
    }

    /// <summary>
    /// 2�����̍ŒZ����
    /// </summary>
    /// <param name="s1">�����̎n�_</param>
    /// <param name="s2">�����̎n�_</param>
    /// <param name="e1">�����̏I�_</param>
    /// <param name="e2">�����̏I�_</param>
    /// <param name="v1">�����̃x�N�g��</param>
    /// <param name="v2">�����̃x�N�g��</param>
    /// <returns></returns>
    static float ClacSegmentSegmentDist(const DirectX::SimpleMath::Vector3& s1,
        const DirectX::SimpleMath::Vector3& s2,
        const DirectX::SimpleMath::Vector3& e1,
        const DirectX::SimpleMath::Vector3& e2,
        const DirectX::SimpleMath::Vector3& v1,
        const DirectX::SimpleMath::Vector3& v2)
    {
        SimpleMath::Vector3 p1, p2;	//	�����̑�
        float t1, t2;	//	�x�N�g���W��

        //	v1���k�ނ��Ă���H
        if (v1.LengthSquared() < FLT_EPSILON)
        {
            //	v2���k�ނ��Ă���H
            if (v2.LengthSquared() < FLT_EPSILON)
            {
                //	�_�Ɠ_�̋����̖��ɋA��
                return (s2 - s1).Length();
            }
            else
            {
                //	s1��2�߂̐����̍ŒZ���ɋA��
                return ClacPointSegmentDist(s1, s2, e2, &p2, &t2);
            }
        }
        //	v2���k�ނ��Ă���H
        else if (v2.LengthSquared() < FLT_EPSILON)
        {
            //	s2��1�߂̐����̖��ɋA��
            return ClacPointSegmentDist(s2, s1, e1, &p1, &t1);
        }

        //	�������m

        //	2���������s�������琂���̒[�_��1��p1�ɉ�����
        if (IsParallel(v1, v2))
        {
            t1 = 0.f;
            p1 = s1;
            float len = ClacPointSegmentDist(s1, s2, e2, &p2, &t2);

            //	t���͈͓��������炻�̂܂ܕԂ�
            if (t2 >= 0.f && t2 <= 1.0f) return len;
        }
        else
        {
            //	�����͂˂���̊֌W
            //	2�����Ԃ̍ŒZ���������߂ĉ���t1�At2�����߂�
            float len = ClacLineLineDist(s1, v1, s2, v2, &p1, &p2, &t1, &t2);

            //	t���͈͓��������炻�̂܂ܕԂ�
            if (t1 >= 0.f && t1 <= 1.f &&
                t2 >= 0.f && t2 <= 1.f)
            {
                return len;
            }
        }

        //	�����̑����O�ɂ��邱�Ƃ�����
        //	t1��0�`1�̊ԂɃN�����v���Đ������~�낷
        Useful::Clamp(t1, 0.f, 1.f);
        p1 = s1 + (t1 * v1);
        float len = ClacPointSegmentDist(p1, s2, e2, &p2, &t2);
        if (t2 >= 0.f && t2 <= 1.f) return len;		//	�͈͓��Ȃ�Ԃ�

        //	t2�����O�������̂�t2�����N�����v�A1�߂̐����ɐ������~�낷
        Useful::Clamp(t2, 0.f, 1.f);
        p2 = s2 + (t2 * v2);
        len = ClacPointSegmentDist(p2, s1, e1, &p1, &t1);
        if (t1 >= 0.f && t1 <= 1.f) return len;		//	�͈͓��Ȃ�Ԃ�

        //	�o���̒[�_���ŒZ�Ɣ���
        Useful::Clamp(t1, 0.f, 1.f);
        p1 = s1 + (t1 * v1);
        return (p2 - p1).Length();
    }
}

#pragma region Sphere

Sphere::Sphere(float r, SimpleMath::Vector3 position) :
    Collider(),
    m_position(position),
    m_radius(r)
{
}

bool Sphere::Intersects(Sphere* other)
{
    //  �L�����`�F�b�N
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  ����
    float dis = SimpleMath::Vector3::DistanceSquared(m_position, other->GetPosition());
    //  ���a�̍��v
    float r = powf(m_radius + other->GetRadius(), 2.f);

    //  �����蔻������
    bool hit = dis <= r;

    //  �����n��
    SetIsHit(hit);
    other->SetIsHit(hit);

    return hit;
}

bool Sphere::Intersects(Capsule* other)
{
    //  �L�����`�F�b�N
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  ���₷�����邽�ߕϐ��֑��
    SimpleMath::Vector3 s = other->GetStartPosition();  //  �n�_�̈ʒu
    SimpleMath::Vector3 e = other->GetEndPosition();	//	�I�_�̈ʒu
    SimpleMath::Vector3 h;                              //  �ŒZ�����̒[�_
    float t;                                            //  �[�_�ʒu�̌W��

    //  �ŒZ�������v�Z
    float d = ColliderJudge::ClacPointSegmentDist(m_position, s, e, &h, &t);

    //  �����蔻������
    bool hit = d <= m_radius + other->GetRadius();

    //  �����n��
    SetIsHit(hit);
    other->SetIsHit(hit);

    return hit;
}

#pragma endregion

#pragma region Capsule

Capsule::Capsule(float length,
    float r,
    SimpleMath::Vector3 position,
    SimpleMath::Quaternion rotate) :
    Collider(),
    m_length(length),
    m_radius(r)
{
    //  �n�_�ƏI�_�̃x�N�g�����v�Z
    m_up = SimpleMath::Vector3(0.f, length / 2.f, 0.f);
    m_down = SimpleMath::Vector3(0.f, -length / 2.f, 0.f);

    //	�n�_�ƏI�_�̈ʒu���v�Z
    m_upPosition = SimpleMath::Vector3::Transform(m_up, rotate) + position;
    m_downPositon = SimpleMath::Vector3::Transform(m_down, rotate) + position;

    //	�n�_����I�_�ւ̃x�N�g�����v�Z
    m_lineVec = m_downPositon - m_upPosition;
}

void Capsule::SetPosition(SimpleMath::Vector3 position)
{
    m_position = position;

    //	�n�_�ƏI�_�̈ʒu���v�Z
    m_upPosition = SimpleMath::Vector3::Transform(m_up, m_rotate) + position;
    m_downPositon = SimpleMath::Vector3::Transform(m_down, m_rotate) + position;

    //	�n�_����I�_�ւ̃x�N�g�����v�Z
    m_lineVec = m_downPositon - m_upPosition;
}

bool Capsule::Intersects(Sphere* other)
{
    //  �L�����`�F�b�N
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  ���₷�����邽�ߕϐ��֑��
    SimpleMath::Vector3 p = other->GetPosition();   //  ���̈ʒu
    SimpleMath::Vector3 h;                          //  �ŒZ�����̒[�_
    float t;                                        //  �[�_�ʒu�̌W��

    //  �ŒZ�������v�Z
    float d = ColliderJudge::ClacPointSegmentDist(p, m_upPosition, m_downPositon, &h, &t);

    //  �����蔻������
    bool hit =  d <= m_radius + other->GetRadius();

    //  �����n��
    SetIsHit(hit);
    other->SetIsHit(hit);

    return hit;
}

bool Capsule::Intersects(Capsule* other)
{
    //  �L�����`�F�b�N
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  ���₷�����邽�ߕϐ��ɑ��
    SimpleMath::Vector3 s1 = m_upPosition;	                //	�n�_
    SimpleMath::Vector3 e1 = m_downPositon;		            //	�I�_
    SimpleMath::Vector3 v1 = m_lineVec;			            //	�����x�N�g��

    SimpleMath::Vector3 s2 = other->GetStartPosition();	    //	�n�_
    SimpleMath::Vector3 e2 = other->GetEndPosition();		//	�I�_
    SimpleMath::Vector3 v2 = other->GetVector();			//	�����x�N�g��

    //	�ŒZ�������擾
    float d = ColliderJudge::ClacSegmentSegmentDist(s1, s2, e1, e2, v1, v2);

    //  �����蔻������
    bool hit = d <= m_radius + other->GetRadius();

    //  �����n��
    SetIsHit(hit);
    other->SetIsHit(hit);

    return hit;
}

#pragma endregion

#pragma region ColliderDebugRender

ColliderDebugRender::ColliderDebugRender(SimpleMath::Matrix view, SimpleMath::Matrix proj) :
    m_view(view),
    m_proj(proj)
{
}

void ColliderDebugRender::DebugRender(Sphere* col)
{
    //	�R���e�L�X�g�擾
    auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

    //	�W�I���g���b�N�v���~�e�B�u�쐬
    std::unique_ptr<GeometricPrimitive> geometry =
        GeometricPrimitive::CreateSphere(context, col->GetRadius() * 2.f);

    //	���[���h�s����쐬
    SimpleMath::Vector3 pos = col->GetPosition();
    SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
    world *= SimpleMath::Matrix::CreateTranslation(pos);

    //	�F���쐬
    SimpleMath::Vector4 color = SimpleMath::Vector4(1.f, 1.f, 1.f, 0.3f);
    //	���������Ă��Ȃ��Ȃ�F��ɂ���
    if (!col->GetIsValid()) color = SimpleMath::Vector4(0.f, 0.f, 1.f, 0.3f);
    //	�������Ă���Ȃ�F��Ԃɂ���
    if (col->GetIsHit()) color = SimpleMath::Vector4(1.f, 0.f, 0.f, 0.3f);

    //	�`��
    geometry->Draw(world, m_view, m_proj, color);
}

void ColliderDebugRender::DebugRender(Capsule* col)
{
    //	�R���e�L�X�g�擾
    auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

    //	�W�I���g���b�N�v���~�e�B�u�쐬
    std::unique_ptr<GeometricPrimitive> cylinder =
        GeometricPrimitive::CreateCylinder(context, col->GetLength(), col->GetRadius() * 2.f);
    std::unique_ptr<GeometricPrimitive> sphere =
        GeometricPrimitive::CreateSphere(context, col->GetRadius() * 2.f);

    //	�F���쐬
    SimpleMath::Vector4 color = SimpleMath::Vector4(1.f, 1.f, 1.f, 0.3f);
    //	���������Ă��Ȃ��Ȃ�F��ɂ���
    if (!col->GetIsValid()) color = SimpleMath::Vector4(0.f, 0.f, 1.f, 0.3f);
    //	�������Ă���Ȃ�F��Ԃɂ���
    if (col->GetIsHit()) color = SimpleMath::Vector4(1.f, 0.f, 0.f, 0.3f);

    //	�~���̕\��
    SimpleMath::Matrix cyllinderWorld = SimpleMath::Matrix::Identity;
    cyllinderWorld *= SimpleMath::Matrix::CreateFromQuaternion(col->GetRotate());
    cyllinderWorld *= SimpleMath::Matrix::CreateTranslation(col->GetPosition());
    cylinder->Draw(cyllinderWorld, m_view, m_proj, color);

    //	��̋�
    SimpleMath::Matrix upSphereWorld = SimpleMath::Matrix::Identity;
    upSphereWorld *= SimpleMath::Matrix::CreateTranslation(col->GetUpPosition());
    sphere->Draw(upSphereWorld, m_view, m_proj, color);

    //	���̋�
    SimpleMath::Matrix downSphereWorld = SimpleMath::Matrix::Identity;
    downSphereWorld *= SimpleMath::Matrix::CreateTranslation(col->GetDownPosition());
    sphere->Draw(downSphereWorld, m_view, m_proj, color);
}

#pragma endregion
