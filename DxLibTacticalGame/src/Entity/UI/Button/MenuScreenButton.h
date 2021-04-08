#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file Button.h
 * @brief ボタン要素
 */

namespace Entity
{
	class MenuScreenButton : public TextButton
	{
	public:
		MenuScreenButton();
		~MenuScreenButton() {};

		void setY(int y);

		bool animationUpdate() override;

		void onMouseLeftDown(int x, int y) override;
		void onMouseLeftUp(int x, int y) override;
		void onMouseOut(int x, int y) override;

		constexpr static int H = 60; //! ボタン幅

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int W = 400; //! ボタン幅
		constexpr static int X = (WIN_W - W) / 2; //! ボタンX座標

		constexpr static int ANIMATION_MS = 100;		//! 拡大、縮小の時間
		const static float ANIMATION_SCALE;			//! サイズ倍率(floatはcppで定義する必要あり)

		//! アニメーションクラス
		Animation animation_;

		//! 基準の大きさ
		Shape baseShape_;

		enum AnimationId 
		{
			EXPANSION,
			SHRINK
		};
	};

	
}