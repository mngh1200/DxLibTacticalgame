#pragma once
#include <climits>
#include <functional>
#include "Easing.h"
#include "FrameWork/Game.h"
#include "DxLib.h"

using namespace std;

/**
 * @file Animation.h
 * @brief ��ʓ��v�f�̃A�j���[�V�������Ǘ�����悤�̃N���X
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
		//! �ω��ʌv�Z�p�̊֐���ێ�����
		function<float(float, float, float, float)> calcFunk_;

		//! �A�j���[�V�����J�n����̃t���[����
		float frameCount_;

		//! �I�����_�̃t���[����
		float frameMax_;

		//! �ω��ʂ̏����l
		float y0_;

		//! �ω��ʂ̏I���l
		float y1_;
	};
}