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

		bool isWorking() const { return countFrame_ >= 0; }; // �^�C�}�[���ғ����ł��邩��Ԃ�

	private:
		//! �J�E���g�t���[��
		int countFrame_;
	};
}