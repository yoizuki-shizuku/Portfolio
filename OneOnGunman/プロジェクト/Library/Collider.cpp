/*/
 *  ファイル名		：	Collider.cpp
 *  概要			：	当たり判定をダブルディスパッチで実装
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/01
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
    /// 点と線の最短距離
    /// </summary>
    /// <param name="p">点</param>
    /// <param name="s">線分の始点</param>
    /// <param name="v">線分の方向ベクトル</param>
    /// <param name="h">点から降ろした垂線の足(戻り値)</param>
    /// <param name="t">ベクトル係数(戻り値)</param>
    /// <returns>最短距離</returns>
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

    //	二つのベクトルが鋭角関係かどうか
    static bool IsSharpAngle(const DirectX::SimpleMath::Vector3& p1,
        const DirectX::SimpleMath::Vector3& p2)
    {
        return p1.Dot(p2) >= 0.f;
    }

    //	三つの位置関係が垂線の内側にあるかどうか
    static bool SharpAngleThree(const DirectX::SimpleMath::Vector3& p1,
        const DirectX::SimpleMath::Vector3& p2,
        const DirectX::SimpleMath::Vector3& p3)
    {
        return IsSharpAngle((p1 - p2), (p3 - p2));
    }

    /// <summary>
    /// 点と線分の最短距離
    /// </summary>
    /// <param name="p">点</param>
    /// <param name="s">線分の始点</param>
    /// <param name="e">線分の終点</param>
    /// <param name="h">最短距離となる端点(戻り値)</param>
    /// <param name="t">端点位置(0未満 : 始点の外, 1超過 : 終点の外</param>
    /// <returns>最短距離</returns>
    static float ClacPointSegmentDist(const DirectX::SimpleMath::Vector3& p,
        const DirectX::SimpleMath::Vector3& s,
        const DirectX::SimpleMath::Vector3& e,
        DirectX::SimpleMath::Vector3* h,
        float* t)
    {
        //	垂線の長さ、垂線の足の座標及びtを算出
        float len = ClacPointLineDist(p, s, (e - s), h, t);

        if (!SharpAngleThree(p, s, e))	//	始点側の外側
        {
            *h = s;
            return (s - p).Length();
        }
        else if (!SharpAngleThree(p, e, s))	//	終点側の外側
        {
            *h = e;
            return (e - p).Length();
        }

        return len;
    }

    //	平行かどうか
    static bool IsParallel(const DirectX::SimpleMath::Vector3& v1,
        const DirectX::SimpleMath::Vector3& v2)
    {
        float d = (v1.Cross(v2)).LengthSquared();
        return (abs(d) < FLT_EPSILON);		//	誤差範囲内なら平行と判定
    }

    /// <summary>
    /// 2直線の最短距離
    /// </summary>
    /// <param name="s1">線分1の始点</param>
    /// <param name="v1">線分1の方向ベクトル</param>
    /// <param name="s2">線分2の始点</param>
    /// <param name="v2">線分2の方向ベクトル</param>
    /// <param name="p1">一つ目の線分への垂線の足(戻り値)</param>
    /// <param name="p2">二つ目の線分への垂線の足(戻り値)</param>
    /// <param name="t1">一つ目の線分のベクトル係数(戻り値)</param>
    /// <param name="t2">二つ目の線分のベクトル係数(戻り値)</param>
    /// <returns>最短距離</returns>
    static float ClacLineLineDist(const DirectX::SimpleMath::Vector3& s1,
        const DirectX::SimpleMath::Vector3& v1,
        const DirectX::SimpleMath::Vector3& s2,
        const DirectX::SimpleMath::Vector3& v2,
        DirectX::SimpleMath::Vector3* p1,
        DirectX::SimpleMath::Vector3* p2,
        float* t1,
        float* t2)
    {
        //	2直線が平行かどうか
        if (IsParallel(v1, v2))
        {
            //	１つ目の直線の始点と二つ目の直線の最短距離問題に帰着
            float len = ClacPointLineDist(s1, s2, v2, p2, t2);
            *p1 = s1;
            *t1 = 0.f;

            return len;
        }

        //	2直線はねじれ関係
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
    /// 2線分の最短距離
    /// </summary>
    /// <param name="s1">線分の始点</param>
    /// <param name="s2">線分の始点</param>
    /// <param name="e1">線分の終点</param>
    /// <param name="e2">線分の終点</param>
    /// <param name="v1">線分のベクトル</param>
    /// <param name="v2">線分のベクトル</param>
    /// <returns></returns>
    static float ClacSegmentSegmentDist(const DirectX::SimpleMath::Vector3& s1,
        const DirectX::SimpleMath::Vector3& s2,
        const DirectX::SimpleMath::Vector3& e1,
        const DirectX::SimpleMath::Vector3& e2,
        const DirectX::SimpleMath::Vector3& v1,
        const DirectX::SimpleMath::Vector3& v2)
    {
        SimpleMath::Vector3 p1, p2;	//	垂線の足
        float t1, t2;	//	ベクトル係数

        //	v1が縮退している？
        if (v1.LengthSquared() < FLT_EPSILON)
        {
            //	v2も縮退している？
            if (v2.LengthSquared() < FLT_EPSILON)
            {
                //	点と点の距離の問題に帰着
                return (s2 - s1).Length();
            }
            else
            {
                //	s1と2つめの線分の最短問題に帰着
                return ClacPointSegmentDist(s1, s2, e2, &p2, &t2);
            }
        }
        //	v2が縮退している？
        else if (v2.LengthSquared() < FLT_EPSILON)
        {
            //	s2と1つめの線分の問題に帰着
            return ClacPointSegmentDist(s2, s1, e1, &p1, &t1);
        }

        //	線分同士

        //	2線分が平行だったら垂線の端点の1つをp1に仮決定
        if (IsParallel(v1, v2))
        {
            t1 = 0.f;
            p1 = s1;
            float len = ClacPointSegmentDist(s1, s2, e2, &p2, &t2);

            //	tが範囲内だったらそのまま返す
            if (t2 >= 0.f && t2 <= 1.0f) return len;
        }
        else
        {
            //	線分はねじれの関係
            //	2直線間の最短距離を求めて仮のt1、t2を求める
            float len = ClacLineLineDist(s1, v1, s2, v2, &p1, &p2, &t1, &t2);

            //	tが範囲内だったらそのまま返す
            if (t1 >= 0.f && t1 <= 1.f &&
                t2 >= 0.f && t2 <= 1.f)
            {
                return len;
            }
        }

        //	垂線の足が外にあることが判明
        //	t1を0～1の間にクランプして垂線を降ろす
        Useful::Clamp(t1, 0.f, 1.f);
        p1 = s1 + (t1 * v1);
        float len = ClacPointSegmentDist(p1, s2, e2, &p2, &t2);
        if (t2 >= 0.f && t2 <= 1.f) return len;		//	範囲内なら返す

        //	t2側が外だったのでt2側をクランプ、1つめの線分に垂線を降ろす
        Useful::Clamp(t2, 0.f, 1.f);
        p2 = s2 + (t2 * v2);
        len = ClacPointSegmentDist(p2, s1, e1, &p1, &t1);
        if (t1 >= 0.f && t1 <= 1.f) return len;		//	範囲内なら返す

        //	双方の端点が最短と判明
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
    //  有効かチェック
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  距離
    float dis = SimpleMath::Vector3::DistanceSquared(m_position, other->GetPosition());
    //  半径の合計
    float r = powf(m_radius + other->GetRadius(), 2.f);

    //  当たり判定を取る
    bool hit = dis <= r;

    //  判定を渡す
    SetIsHit(hit);
    other->SetIsHit(hit);

    return hit;
}

bool Sphere::Intersects(Capsule* other)
{
    //  有効かチェック
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  見やすくするため変数へ代入
    SimpleMath::Vector3 s = other->GetStartPosition();  //  始点の位置
    SimpleMath::Vector3 e = other->GetEndPosition();	//	終点の位置
    SimpleMath::Vector3 h;                              //  最短距離の端点
    float t;                                            //  端点位置の係数

    //  最短距離を計算
    float d = ColliderJudge::ClacPointSegmentDist(m_position, s, e, &h, &t);

    //  当たり判定を取る
    bool hit = d <= m_radius + other->GetRadius();

    //  判定を渡す
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
    //  始点と終点のベクトルを計算
    m_up = SimpleMath::Vector3(0.f, length / 2.f, 0.f);
    m_down = SimpleMath::Vector3(0.f, -length / 2.f, 0.f);

    //	始点と終点の位置を計算
    m_upPosition = SimpleMath::Vector3::Transform(m_up, rotate) + position;
    m_downPositon = SimpleMath::Vector3::Transform(m_down, rotate) + position;

    //	始点から終点へのベクトルを計算
    m_lineVec = m_downPositon - m_upPosition;
}

void Capsule::SetPosition(SimpleMath::Vector3 position)
{
    m_position = position;

    //	始点と終点の位置を計算
    m_upPosition = SimpleMath::Vector3::Transform(m_up, m_rotate) + position;
    m_downPositon = SimpleMath::Vector3::Transform(m_down, m_rotate) + position;

    //	始点から終点へのベクトルを計算
    m_lineVec = m_downPositon - m_upPosition;
}

bool Capsule::Intersects(Sphere* other)
{
    //  有効かチェック
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  見やすくするため変数へ代入
    SimpleMath::Vector3 p = other->GetPosition();   //  球の位置
    SimpleMath::Vector3 h;                          //  最短距離の端点
    float t;                                        //  端点位置の係数

    //  最短距離を計算
    float d = ColliderJudge::ClacPointSegmentDist(p, m_upPosition, m_downPositon, &h, &t);

    //  当たり判定を取る
    bool hit =  d <= m_radius + other->GetRadius();

    //  判定を渡す
    SetIsHit(hit);
    other->SetIsHit(hit);

    return hit;
}

bool Capsule::Intersects(Capsule* other)
{
    //  有効かチェック
    if (!GetIsValid() || !other->GetIsValid())
    {
        SetIsHit(false);
        other->SetIsHit(false);
        return false;
    }

    //  見やすくするため変数に代入
    SimpleMath::Vector3 s1 = m_upPosition;	                //	始点
    SimpleMath::Vector3 e1 = m_downPositon;		            //	終点
    SimpleMath::Vector3 v1 = m_lineVec;			            //	方向ベクトル

    SimpleMath::Vector3 s2 = other->GetStartPosition();	    //	始点
    SimpleMath::Vector3 e2 = other->GetEndPosition();		//	終点
    SimpleMath::Vector3 v2 = other->GetVector();			//	方向ベクトル

    //	最短距離を取得
    float d = ColliderJudge::ClacSegmentSegmentDist(s1, s2, e1, e2, v1, v2);

    //  当たり判定を取る
    bool hit = d <= m_radius + other->GetRadius();

    //  判定を渡す
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
    //	コンテキスト取得
    auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

    //	ジオメトリックプリミティブ作成
    std::unique_ptr<GeometricPrimitive> geometry =
        GeometricPrimitive::CreateSphere(context, col->GetRadius() * 2.f);

    //	ワールド行列を作成
    SimpleMath::Vector3 pos = col->GetPosition();
    SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
    world *= SimpleMath::Matrix::CreateTranslation(pos);

    //	色を作成
    SimpleMath::Vector4 color = SimpleMath::Vector4(1.f, 1.f, 1.f, 0.3f);
    //	判定を取っていないなら色を青にする
    if (!col->GetIsValid()) color = SimpleMath::Vector4(0.f, 0.f, 1.f, 0.3f);
    //	当たっているなら色を赤にする
    if (col->GetIsHit()) color = SimpleMath::Vector4(1.f, 0.f, 0.f, 0.3f);

    //	描画
    geometry->Draw(world, m_view, m_proj, color);
}

void ColliderDebugRender::DebugRender(Capsule* col)
{
    //	コンテキスト取得
    auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

    //	ジオメトリックプリミティブ作成
    std::unique_ptr<GeometricPrimitive> cylinder =
        GeometricPrimitive::CreateCylinder(context, col->GetLength(), col->GetRadius() * 2.f);
    std::unique_ptr<GeometricPrimitive> sphere =
        GeometricPrimitive::CreateSphere(context, col->GetRadius() * 2.f);

    //	色を作成
    SimpleMath::Vector4 color = SimpleMath::Vector4(1.f, 1.f, 1.f, 0.3f);
    //	判定を取っていないなら色を青にする
    if (!col->GetIsValid()) color = SimpleMath::Vector4(0.f, 0.f, 1.f, 0.3f);
    //	当たっているなら色を赤にする
    if (col->GetIsHit()) color = SimpleMath::Vector4(1.f, 0.f, 0.f, 0.3f);

    //	円柱の表示
    SimpleMath::Matrix cyllinderWorld = SimpleMath::Matrix::Identity;
    cyllinderWorld *= SimpleMath::Matrix::CreateFromQuaternion(col->GetRotate());
    cyllinderWorld *= SimpleMath::Matrix::CreateTranslation(col->GetPosition());
    cylinder->Draw(cyllinderWorld, m_view, m_proj, color);

    //	上の球
    SimpleMath::Matrix upSphereWorld = SimpleMath::Matrix::Identity;
    upSphereWorld *= SimpleMath::Matrix::CreateTranslation(col->GetUpPosition());
    sphere->Draw(upSphereWorld, m_view, m_proj, color);

    //	下の球
    SimpleMath::Matrix downSphereWorld = SimpleMath::Matrix::Identity;
    downSphereWorld *= SimpleMath::Matrix::CreateTranslation(col->GetDownPosition());
    sphere->Draw(downSphereWorld, m_view, m_proj, color);
}

#pragma endregion
