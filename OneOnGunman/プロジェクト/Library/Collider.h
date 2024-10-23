/*/
 *  ファイル名		：	Collider.h
 *  概要			：	当たり判定をダブルディスパッチで実装
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/01
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
    //  当たっているかどうか
    bool m_isHit;
    //  有効な状態かどうか
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
    //  位置
    DirectX::SimpleMath::Vector3 m_position;
    //  半径
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
    //  位置
    DirectX::SimpleMath::Vector3 m_position;
    //	回転
    DirectX::SimpleMath::Quaternion m_rotate;
    //	positionから見て上の点の方向ベクトル(始点)
    DirectX::SimpleMath::Vector3 m_up;
    //	positionから見て下の点の方向ベクトル(終点)
    DirectX::SimpleMath::Vector3 m_down;
    //	始点の位置
    DirectX::SimpleMath::Vector3 m_upPosition;
    //	終点の位置
    DirectX::SimpleMath::Vector3 m_downPositon;
    //	始点から終点への方向ベクトル
    DirectX::SimpleMath::Vector3 m_lineVec;

    //	半径
    float m_radius;
    //	長さ
    float m_length;

};

//	コライダーのデバッグ表示
class ColliderDebugRender final
{
public:
    ColliderDebugRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

    //	球のデバッグ表示
    void DebugRender(Sphere* col);
    //	カプセルのデバッグ表示
    void DebugRender(Capsule* col);

private:
    //	ビュー行列
    DirectX::SimpleMath::Matrix m_view;
    //	射影行列
    DirectX::SimpleMath::Matrix m_proj;

};