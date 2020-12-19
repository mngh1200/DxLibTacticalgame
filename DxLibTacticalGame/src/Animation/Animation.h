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
			NORMAL,				// �ʏ���������s����i�����l�j
			AlTERNATE,			// �ʏ�����ƁA�t�������J��Ԃ�
			REVERSE,			// ����t���������s����
			AlTERNATE_REVERSE	// �ʏ�����ƁA�t�������J��Ԃ��i�t�Đ�����n�܂�j
		};

		static const int REPEAT_INFINITE = -1;

	private:
		void setDirection(int direction);

		//! �ω��ʌv�Z�p�̊֐���ێ�����
		function<float(float, float, float, float)> calcFunk_;

		//! �A�j���[�V�����J�n����̃t���[����
		float frameCount_;

		//! �I�����_�̃t���[����
		float frameMax_;

		//! �A�j���[�V�����J�n�x���t���[���i�l��0�ɂȂ������_����A�j���[�V�������J�n����j
		int delayFrame_;

		//! ���s��������̎��
		int direction_;

		//! ���݁A�ʏ�����ɃA�j���[�V���������s���Ă��邩
		bool isNormalOrder_;

		//! ���s�[�g��
		int repeat_;

		//! ���s�[�g�񐔁i�ő�l=�����l�j
		int repeatMax_;
	};
}