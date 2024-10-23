/*/
 *  ファイル名		：	Useful.h
 *  概要			：	計算で使える関数をまとめたヘッダー
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/25
/*/
#pragma once

#include <math.h>

namespace Useful
{
    /// <summary>
    /// 値を指定した範囲内にする
    /// </summary>
    /// <param name="value">値</param>
    /// <param name="min">最小値</param>
    /// <param name="max">最大値</param>
    /// <returns>min～maxの値</returns>
    template<typename T>
    inline T Clamp(const T& value, const T& min, const T& max)
    {
        return value < min ? min : value > max ? max : value;
    }

    /// <summary>
    /// 0とlengthの間で跳ね返る値をとる
    /// </summary>
    /// <param name="value">値</param>
    /// <param name="length">長さ</param>
    /// <returns>0～lengthの値</returns>
    inline float PingPong(const float& value, const float& length)
    {
        int y = static_cast<int>(value / length);
        return y % 2 ? fabsf(value - length * (y + 1)) : value - length * y;
    }

    /// <summary>
    /// 範囲を超えると範囲の反対側へ補正する
    /// </summary>
    /// <param name="value">値</param>
    /// <param name="min">最小値</param>
    /// <param name="max">最大値</param>
    /// <returns>min～maxの値</returns>
    template<typename T>
    inline T Loop(const T& value, const T& min, const T& max)
    {
        if (value < min)
        {
            return max - (value - min);
        }
        else if (value > max)
        {
            return min + (value - max);
        }

        return value;
    }

    /// <summary>
    /// lengthに対する割合をとる
    /// </summary>
    /// <param name="value">値</param>
    /// <param name="length">100%</param>
    /// <returns>割合</returns>
    inline float Ratio(const float& value, const float& length)
    {
        return value / length;
    }

    /// <summary>
    /// 割合を0から1の間にクランプ
    /// </summary>
    /// <param name="value">値</param>
    /// <param name="length">100%</param>
    /// <returns>0～1の割合</returns>
    inline float Ratio01(const float& value, const float& length)
    {
        float ratio = Ratio(value,length);
        return Clamp(ratio, 0.f, 1.f);
    }

    /// <summary>
    /// 2点間の角度を求める(Vector2)
    /// </summary>
    /// <param name="startX">基準点のx</param>
    /// <param name="startY">基準点のy</param>
    /// <param name="targetX">目標点のx</param>
    /// <param name="targetY">目標点のy</param>
    /// <returns>radian</returns>
    inline float GetAngle(float startX, float startY, float targetX, float targetY)
    {
        float dtX = targetX - startX;
        float dtY = targetY - startY;

        return atan2f(dtY, dtX);
    }
}