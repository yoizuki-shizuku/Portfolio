//--------------------------------------------------------------------------------------
// File: Animation.h
//
// Simple animation playback system for CMO and SDKMESH for DirectX Tool Kit
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------
#pragma once

#include <DirectXMath.h>
#include <Model.h>

#include <memory>
#include <utility>
#include <vector>


namespace DX
{
    class AnimationSDKMESH
    {
    public:
        AnimationSDKMESH() noexcept;
        ~AnimationSDKMESH() = default;

        AnimationSDKMESH(AnimationSDKMESH&&) = default;
        AnimationSDKMESH& operator= (AnimationSDKMESH&&) = default;

        AnimationSDKMESH(AnimationSDKMESH const&) = delete;
        AnimationSDKMESH& operator= (AnimationSDKMESH const&) = delete;

        HRESULT Load(_In_z_ const wchar_t* fileName);

        void Release()
        {
            m_animTime = 0.0;
            m_animSize = 0;
            m_animData.reset();
            m_boneToTrack.clear();
            m_animBones.reset();
        }

        bool Bind(const DirectX::Model& model);

        void Update(float delta);

        void Apply(
            const DirectX::Model& model,
            size_t nbones,
            _Out_writes_(nbones) DirectX::XMMATRIX* boneTransforms) const;

    public:     //  追加分
        //  アニメーションを最初から開始する
        void RestartAnim() { m_animTime = m_startTime; }
        //  アニメーションの開始時間を指定する
        void SetStartTime(const double& startTime) { m_startTime = startTime; }
        //  アニメーションの終了時間を指定する
        void SetEndTime(const double& endTime) { m_endTime = endTime; }
        //  アニメーションのループを指定
        void SetLoop(const bool& loop) { m_loop = loop; }
        //  m_animTimeがm_endTimeを超えているかどうか
        bool GetFinishAnim() { return m_animTime > m_endTime; }

    private:
        double                              m_animTime;
        double                              m_startTime;    //  追加分。アニメーションの開始時間
        double                              m_endTime;      //  追加分。アニメーションの終了時間を指定
        bool                                m_loop;         //  追加分。アニメーションをループさせるかどうか
        std::unique_ptr<uint8_t[]>          m_animData;
        size_t                              m_animSize;
        std::vector<uint32_t>               m_boneToTrack;
        DirectX::ModelBone::TransformArray  m_animBones;
    };

    class AnimationCMO
    {
    public:
        AnimationCMO() noexcept;
        ~AnimationCMO() = default;

        AnimationCMO(AnimationCMO&&) = default;
        AnimationCMO& operator= (AnimationCMO&&) = default;

        AnimationCMO(AnimationCMO const&) = delete;
        AnimationCMO& operator= (AnimationCMO const&) = delete;

        HRESULT Load(_In_z_ const wchar_t* fileName, size_t offset, _In_opt_z_ const wchar_t* clipName = nullptr);

        void Release()
        {
            m_animTime = m_startTime = m_endTime = 0.f;
            m_keys.clear();
            m_transforms.reset();
            m_animBones.reset();
        }

        void Bind(const DirectX::Model& model);

        void Update(float delta);

        void Apply(
            const DirectX::Model& model,
            size_t nbones,
            _Out_writes_(nbones) DirectX::XMMATRIX* boneTransforms) const;

    private:
        using Key = std::pair<uint32_t, float>;

        float                               m_animTime;
        float                               m_startTime;
        float                               m_endTime;
        std::vector<Key>                    m_keys;
        DirectX::ModelBone::TransformArray  m_transforms;
        DirectX::ModelBone::TransformArray  m_animBones;
    };
}
