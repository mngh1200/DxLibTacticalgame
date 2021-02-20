#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Mask.h
 * @brief ��ʑJ�ڎ��̃I�[�o�[���C
 */

namespace Entity
{
	class Overlay : public Object
	{
	public:
		Overlay() = delete;
		Overlay(bool isOpen);
		~Overlay() {};

		bool createAnimation(int animationId) override;
		bool animationUpdate() override;

		void render() const override;

	private:
		const static int ANIMATION_TIME = 250; //! �A�j���[�V��������
		const static int COLOR;

		//! ��ʏI�������\������
		bool isOpen_;

		//! ���ߓx
		int alpha_;

		Animation animation_;

	};
}