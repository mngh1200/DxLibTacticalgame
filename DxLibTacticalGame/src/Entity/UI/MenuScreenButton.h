#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "Utility/FontManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file Button.h
 * @brief ボタン要素
 */

namespace Entity
{
	class MenuScreenButton : public Object
	{
	public:
		MenuScreenButton();
		MenuScreenButton(string text, int x, int y, int w, int h);
		~MenuScreenButton() {};

		void render() const override;

		bool animationUpdate() override;

		void onMouseLeftDown() override;
		void onMouseLeftUp() override;
		void onMouseOver() override;
		void onMouseOut() override;

	protected:
		bool createAnimation(int animationId);

	private:
		
		static const int ACCENT_WIDTH = 25;			//! アクセント部分の幅
		static const int TEXT_PADDING_LEFT = 25;	//! テキストの左余白
		static const int TEXT_SIZE = 32;			//! フォントサイズ
		static const int ANIMATION_MS = 100;		//! 拡大、縮小の時間
		static const float ANIMATION_SCALE;			//! サイズ倍率(floatはcppで定義する必要あり)

		//! テキストのY座標
		int textY_;

		//! テキスト
		string text_;

		//! ボタン色
		int backgroundColor_;

		//! 文字色
		int textColor_;

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