/*/
 *  �t�@�C����		�F	Collider.h
 *  �T�v			�F	�����蔻����_�u���f�B�X�p�b�`�Ŏ���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/01
/*/
#pragma once

class Sphere;
class Capsule;

class Collider
{
public:
    Collider() : m_isHit(false), m_isValid(true) {};
    virtual ~Collider() = default;

    virtual bool CheckHit(Collider* other) = 0;

public:
    virtual bool Intersects(Sphere* other) { UNREFERENCED_PARAMETER(other); return false; }
    virtual bool Intersects(Capsule* other) { UNREFERENCED_PARAMETER(other); return false; }

    virtual void SetIsHit(bool hit) final { m_isHit = hit; }
    virtual void SetIsValid(bool valid) final { m_isValid = valid; }

    virtual bool GetIsHit() final { return m_isHit; }
    virtual bool GetIsValid() final { return m_isValid; }

private:
    //  �������Ă��邩�ǂ���
    bool m_isHit;
    //  �L���ȏ�Ԃ��ǂ���
    bool m_isValid;

};

class Sphere : public Collider
{
public:
    Sphere(float r = 1.f,
        DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3::Zero);
    ~Sphere() = default;

    bool CheckHit(Collider* other) override { return other->Intersects(this); }

    DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
    float GetRadius() const { return m_radius; }

    void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

public:
    bool Intersects(Sphere* other) override;
    bool Intersects(Capsule* other) override;

private:
    //  �ʒu
    DirectX::SimpleMath::Vector3 m_position;
    //  ���a
    float m_radius;

};

class Capsule : public Collider
{
public:
    Capsule(float length = 1.f,
        float r = 1.f,
        DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3::Zero,
        DirectX::SimpleMath::Quaternion rotate = DirectX::SimpleMath::Quaternion::Identity);
    ~Capsule() = default;

    bool CheckHit(Collider* other) override { return other->Intersects(this); }

    DirectX::SimpleMath::Vector3 GetPosition()      const { return m_position; }
    DirectX::SimpleMath::Quaternion GetRotate()     const { return m_rotate; }
    DirectX::SimpleMath::Vector3 GetStartPosition() const { return m_upPosition; }
    DirectX::SimpleMath::Vector3 GetEndPosition()   const { return m_downPositon; }
    DirectX::SimpleMath::Vector3 GetVector()        const { return m_lineVec; }
    DirectX::SimpleMath::Vector3 GetUpPosition()    const { return m_upPosition; }
    DirectX::SimpleMath::Vector3 GetDownPosition()  const { return m_downPositon; }
    float GetRadius()                               const { return m_radius; }
    float GetLength()                               const { return m_length; }

    void SetPosition(DirectX::SimpleMath::Vector3 position);

public:
    bool Intersects(Sphere* other) override;
    bool Intersects(Capsule* other) override;

private:
    //  �ʒu
    DirectX::SimpleMath::Vector3 m_position;
    //	��]
    DirectX::SimpleMath::Quaternion m_rotate;
    //	position���猩�ď�̓_�̕����x�N�g��(�n�_)
    DirectX::SimpleMath::Vector3 m_up;
    //	position���猩�ĉ��̓_�̕����x�N�g��(�I�_)
    DirectX::SimpleMath::Vector3 m_down;
    //	�n�_�̈ʒu
    DirectX::SimpleMath::Vector3 m_upPosition;
    //	�I�_�̈ʒu
    DirectX::SimpleMath::Vector3 m_downPositon;
    //	�n�_����I�_�ւ̕����x�N�g��
    DirectX::SimpleMath::Vector3 m_lineVec;

    //	���a
    float m_radius;
    //	����
    float m_length;

};

//	�R���C�_�[�̃f�o�b�O�\��
class ColliderDebugRender final
{
public:
    ColliderDebugRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

    //	���̃f�o�b�O�\��
    void DebugRender(Sphere* col);
    //	�J�v�Z���̃f�o�b�O�\��
    void DebugRender(Capsule* col);

private:
    //	�r���[�s��
    DirectX::SimpleMath::Matrix m_view;
    //	�ˉe�s��
    DirectX::SimpleMath::Matrix m_proj;

};