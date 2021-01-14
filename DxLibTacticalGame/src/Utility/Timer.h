#pragma once
#include <climits>
#include <functional>
#include "FrameWork/Game.h"
#include "DxLib.h"

using namespace std;

/**
 * @file Timer.h
 * @brief �^�C�}�[�N���X
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

	private:
		//! �J�E���g�t���[��
		int countFrame_;
	};
}