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
		MenuScreenButton(const char* text, int x, int y, int w, int h);
		~MenuScreenButton() {};

		bool animationUpdate() override;

		void onMouseLeftDown(int x, int y) override;
		void onMouseLeftUp(int x, int y) override;
		void onMouseOut(int x, int y) override;

	protected:
		bool createAnimation(int animationId);

	private:
		static const int TEXT_PADDING_LEFT = 25;	//! テキストの左余白
		static const int ANIMATION_MS = 100;		//! 拡大、縮小の時間
		static const float ANIMATION_SCALE;			//! サイズ倍率(floatはcppで定義する必要あり)

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