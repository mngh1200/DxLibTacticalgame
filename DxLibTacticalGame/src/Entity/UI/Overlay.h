#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Mask.h
 * @brief ��ʑJ�ڎ��̃I�[�o�[���C
 */

namespace
{
	const int COLOR = DxLib::GetColor(0, 0, 0);
	constexpr int ADD_ALPHA = 25;
}

namespace Entity
{
	class Overlay : public Object
	{
	public:
		Overlay();
		Overlay(bool isOpen);
		~Overlay() {};

		bool animationUpdate() override;

		void render() const override;

	private:
		//! ��ʏI�������\������
		bool isOpen_;

		//! ���ߓx
		int alpha_;

	};
}