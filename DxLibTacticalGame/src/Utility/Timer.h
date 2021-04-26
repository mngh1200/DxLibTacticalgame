#pragma once
#include <climits>
#include <functional>
#include "FrameWork/Game.h"
#include "DxLib.h"

using namespace std;

/**
 * @file Timer.h
 * @brief タイマークラス
 */

namespace Utility
{
	class Timer
	{
	public:
		Timer() : countFrame_(-1) {};
		~Timer() {};

		void setTimer(int ms);
		bool checkAndCountTimer();

		bool isWorking() const { return countFrame_ >= 0; }; // タイマーが稼働中であるかを返す

	private:
		//! カウントフレーム
		int countFrame_;
	};
}