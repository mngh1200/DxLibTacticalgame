#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Mask.h
 * @brief 画面遷移時のオーバーレイ
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
		const static int ANIMATION_TIME = 250; //! アニメーション時間
		const static int COLOR;

		//! 画面終了時か表示時か
		bool isOpen_;

		//! 透過度
		int alpha_;

		Animation animation_;

	};
}