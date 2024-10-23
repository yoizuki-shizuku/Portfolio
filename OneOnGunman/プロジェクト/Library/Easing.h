/*/
 *  ファイル名		：	Easing.h
 *  概要			：	Easing系の関数をまとめたヘッダーファイル
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/16
/*/
#pragma once

#include <math.h>
#include <functional>
#include <array>

#define PI		(3.14159265359f)

namespace Easing
{
	inline float Lerp(float a, float b, float t, std::function<float(float)> rate = [](float t) { return t; })
	{
		t = rate(t);

		return b * t + a * (1.f - t);
	}

	inline float NonEasing(float t)
	{
		return t;
	}

	//	javaScriptにある===(厳密等価演算子)の代わり。
	//	(調べてみたけど仕様よくわからん...)
	//	ひとまずfloat型の==のような形で実装
	inline bool StrictEqualityOperator(float a, float b)
	{
		float error = a - b;

		if (fabsf(error) < FLT_EPSILON) return true;
		else return false;
	}

#pragma region Sine

	inline float InSine(float t)
	{
		return 1.f - cosf((t * PI) / 2.f);
	}

	inline float OutSine(float t)
	{
		return sinf((t * PI) / 2.f);
	}

	inline float InOutSine(float t)
	{
		return -(cosf(PI * t) - 1.f) / 2.f;
	}

	inline float OutInSine(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutSine);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InSine);
		}
	}

#pragma endregion

#pragma region Quad

	inline float InQuad(float t)
	{
		return t * t;
	}

	inline float OutQuad(float t)
	{
		return  1.f - (1.f - t) * (1.f - t);
	}

	inline float InOutQuad(float t)
	{
		return t < 0.5f ? 2.f * t * t : 1.f - powf(-2.f * t + 2.f, 2.f) / 2.f;
	}

	inline float OutInQuad(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutQuad);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InQuad);
		}
	}

#pragma endregion

#pragma region Cubic

	inline float InCubic(float t)
	{
		return t * t * t;
	}

	inline float OutCubic(float t)
	{
		return 1.f - powf(1.f - t, 3.f);
	}

	inline float InOutCubic(float t)
	{
		return t < 0.5f ? 4.f * t * t * t : 1 - powf(-2.f * t + 2.f, 3.f) / 2.f;
	}

	inline float OutInCubic(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutCubic);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InCubic);
		}
	}

#pragma endregion

#pragma region Quart

	inline float InQuart(float t)
	{
		return t * t * t * t;
	}

	inline float OutQuart(float t)
	{
		return 1.f - powf(1.f - t, 5.f);
	}

	inline float InOutQuart(float t)
	{
		return t < 0.5f ? 16.f * t * t * t * t * t : 1.f - powf(-2.f * t + 2.f, 5.f) / 2.f;
	}

	inline float OutInQuart(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutQuart);
		}
		else
		{
			return Lerp(b / 2, b, (t * 2) - 1.f, InQuart);
		}
	}

#pragma endregion

#pragma region Quint

	inline float InQuint(float t)
	{
		return t * t * t * t * t;
	}

	inline float OutQuint(float t)
	{
		return 1.f - powf(1.f - t, 5.f);
	}

	inline float InOutQuint(float t)
	{
		return t < 0.5f ? 16.f * t * t * t * t * t : 1.f - powf(-2.f * t + 2.f, 5.f) / 2.f;
	}

	inline float OutInQuint(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutQuint);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InQuint);
		}
	}

#pragma endregion

#pragma region Expo

	//	動作未保障
	inline float InExpo(float t)
	{
		return StrictEqualityOperator(t, 0.f) ? 0.f : powf(2.f, 10.f * t - 10.f);
	}

	//	動作未保障
	inline float OutExpo(float t)
	{
		return StrictEqualityOperator(t, 1.f) ? 1.f : 1.f - powf(2.f, -10.f * t);
	}

	//	動作未保障
	inline float InOutExpo(float t)
	{
		return StrictEqualityOperator(t, 0.f)
			? 0.f
			: StrictEqualityOperator(t, 1.f)
			? 1.f
			: t < 0.5f ? powf(2.f, 20.f * t - 10.f) / 2.f
			: (2.f - powf(2.f, -20.f * t + 10.f)) / 2.f;
	}

	//	動作未保障
	inline float OutInExpo(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutExpo);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InExpo);
		}
	}

#pragma endregion

#pragma region Circ

	inline float InCirc(float t)
	{
		return 1.f - sqrtf(1.f - powf(t, 2.f));
	}

	inline float OutCirc(float t)
	{
		return sqrtf(1.f - powf(t - 1.f, 2.f));
	}

	inline float InOutCirc(float t)
	{
		return t < 0.5f
			? (1.f - sqrtf(1.f - powf(2.f * t, 2.f))) / 2.f
			: (sqrtf(1.f - powf(-2.f * t + 2.f, 2.f)) + 1.f) / 2.f;
	}

	inline float OutInCirc(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutCirc);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InCirc);
		}
	}

#pragma endregion

#pragma region Back

	inline float InBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.f;

		return c3 * t * t * t - c1 * t * t;
	}

	inline float OutBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.f;

		return 1.f + c3 * powf(t - 1.f, 3.f) + c1 * powf(t - 1.f, 2.f);
	}

	inline float InOutBack(float t)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		return t < 0.5f
			? (powf(2.f * t, 2.f) * ((c2 + 1.f) * 2.f * t - c2)) / 2.f
			: (powf(2.f * t - 2.f, 2.f) * ((c2 + 1.f) * (t * 2.f - 2.f) + c2) + 2.f) / 2.f;
	}

	inline float OutInBack(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutBack);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InBack);
		}
	}

#pragma endregion

#pragma region Elastic

	//	動作未保障
	inline float InElastic(float t)
	{
		const float c4 = (2.f * PI) / 3.f;

		return StrictEqualityOperator(t, 0.f)
			? 0.f
			: StrictEqualityOperator(t, 1.f)
			? 1.f
			: -powf(2.f, 10.f * t - 10.f) * sinf((t * 10.f - 10.75f) * c4);
	}

	//	動作未保障
	inline float OutElastic(float t)
	{
		const float c4 = (2.f * PI) / 3.f;

		return StrictEqualityOperator(t, 0.f)
			? 0.f
			: StrictEqualityOperator(t, 1.f)
			? 1.f
			: powf(2.f, -10.f * t) * sinf((t * 10.f - 0.75f) * c4) + 1.f;
	}

	//	動作未保障
	inline float InOutElastic(float t)
	{
		const float c5 = (2.f * PI) / 4.5f;

		return StrictEqualityOperator(t, 0.f)
			? 0.f
			: StrictEqualityOperator(t, 1.f)
			? 1.f
			: t < 0.5f
			? -(powf(2.f, 20.f * t - 10.f) * sinf((20.f * t - 11.125f) * c5)) / 2.f
			: (powf(2.f, -20.f * t + 10.f) * sinf((20.f * t - 11.125f) * c5)) / 2.f + 1.f;
	}

	//	動作未保障
	inline float OutInElastic(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutElastic);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InElastic);
		}
	}

#pragma endregion

#pragma region Bounce

	inline float OutBounce(float t)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (t < 1.f / d1) {
			return n1 * t * t;
		}
		else if (t < 2.f / d1) {
			return n1 * (t -= 1.5f / d1) * t + 0.75f;
		}
		else if (t < 2.5f / d1) {
			return n1 * (t -= 2.25f / d1) * t + 0.9375f;
		}
		else {
			return n1 * (t -= 2.625f / d1) * t + 0.984375f;
		}
	}

	inline float InBounce(float t)
	{
		return 1.f - OutBounce(1.f - t);
	}


	inline float InOutBounce(float t)
	{
		return t < 0.5f
			? (1.f - OutBounce(1.f - 2.f * t)) / 2.f
			: (1.f + OutBounce(2.f * t - 1.f)) / 2.f;
	}

	inline float OutInBounce(float a, float b, float t)
	{
		if (t < 0.5f)
		{
			return Lerp(a, b / 2, t * 2.f, OutBounce);
		}
		else
		{
			return Lerp(a + b / 2, b, (t * 2) - 1.f, InBounce);
		}
	}

#pragma endregion

	//	配列形式で指定できるようにしておく
	const std::array<std::function<float(float)>, 31> EASING_ARRAY
	{
		NonEasing,								//	0
		InSine, OutSine, InOutSine,				//	1,2,3
		InQuad, OutQuad, InOutQuad,				//	4,5,6
		InCubic, OutCubic, InOutCubic,			//	7,8,9
		InQuart, OutQuart, InOutQuart,			//	10,11,12
		InQuint, OutQuint, InOutQuint,			//	13,14,15
		InExpo, OutExpo, InOutExpo,				//	16,17,18
		InCirc, OutCirc, InOutCirc,				//	19,20,21
		InBack, OutBack, InOutBack,				//	22,23,24
		InElastic, OutElastic, InOutElastic,	//	25,26,27
		InBounce, OutBounce,InOutBounce			//	28,29,30
	};
}