#pragma once
#include <climits>
#include <functional>
#include "Easing.h"
#include "FrameWork/Game.h"
#include "DxLib.h"

using namespace std;

/**
 * @file Animation.h
 * @brief 画面内要素のアニメーションを管理するようのクラス
 */

namespace Entity
{
	class Animation
	{
	public:
		Animation() : 
			calcFunk_{}, 
			frameCount_(0),
			frameMax_(0),
			y0_(0),
			y1_(1)
		{};

		Animation(int timeMs, int y1 = 1, int y0 = 0, function<float(float, float, float, float)> func = Easing::Linear<float>);

		~Animation() {};

		bool update(float* y);
		bool update(int* y);

	private:
		//! 変化量計算用の関数を保持する
		function<float(float, float, float, float)> calcFunk_;

		//! アニメーション開始からのフレーム数
		float frameCount_;

		//! 終了時点のフレーム数
		float frameMax_;

		//! 変化量の初期値
		float y0_;

		//! 変化量の終了値
		float y1_;
	};
}