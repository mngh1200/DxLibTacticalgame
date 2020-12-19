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
			calcFunk_(Easing::Linear<float>),
			frameCount_(0),
			frameMax_(0),
			delayFrame_(0),
			direction_(NORMAL),
			isNormalOrder_(true),
			repeat_(1),
			repeatMax_(repeat_)
		{};

		Animation(int timeMs, int direction = NORMAL, int repeat = 1, int delayMs = 0, function<float(float, float, float, float)> func = Easing::Linear<float>);

		~Animation() {};

		bool increaseFrame();
		inline bool getAbleUpdate() const;

		void reset();
		void reset(int direction);

		void forceFinish();


		void adjustFrame(float y, float y0, float y1);
		void adjustFrame(Shape nowShape, Shape baseShape, float scale);

		bool update(float* y, float y0 = 0, float y1 = 1, bool isIncreaseFrame = true);
		bool update(int* y, int y0, int y1, bool isIncreaseFrame = true);

		bool update(int* nowX, int* nowY, int x0, int y0, int x1, int y1, bool isIncreaseFrame = true);
		bool update(Shape* nowShape, Shape baseShape, float scale, bool isIncreaseFrame = true);
		bool update(int* nowColor, int r0, int g0, int b0, int r1, int g1, int b1, bool isIncreaseFrame = true);

		enum Direction
		{
			NORMAL,				// 通常方向を実行する（初期値）
			AlTERNATE,			// 通常方向と、逆方向を繰り返す
			REVERSE,			// 毎回逆方向を実行する
			AlTERNATE_REVERSE	// 通常方向と、逆方向を繰り返す（逆再生から始まる）
		};

		static const int REPEAT_INFINITE = -1;

	private:
		void setDirection(int direction);

		//! 変化量計算用の関数を保持する
		function<float(float, float, float, float)> calcFunk_;

		//! アニメーション開始からのフレーム数
		float frameCount_;

		//! 終了時点のフレーム数
		float frameMax_;

		//! アニメーション開始遅延フレーム（値が0になった時点からアニメーションを開始する）
		int delayFrame_;

		//! 実行する方向の種類
		int direction_;

		//! 現在、通常方向にアニメーションを実行しているか
		bool isNormalOrder_;

		//! リピート回数
		int repeat_;

		//! リピート回数（最大値=初期値）
		int repeatMax_;
	};
}