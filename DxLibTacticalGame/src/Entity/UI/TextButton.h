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
	class TextButton : public Object
	{
	public:
		TextButton() = delete;
		TextButton(int colorType, int textColorType);
		~TextButton() {};

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setText(string text);
		void setText(string text, int x, int fontType);
		void setTextCenter(string text, int fontType);
		void setFont(int fontType);

		void setColor(int backgroundColor, int textColor, int state = NORMAL);
		void render() const override;

		void setSizeAnimation(float scale, int timeMs);
		bool animationUpdate() override;

		void onMouseLeftDown() override;
		void onMouseLeftUp() override;
		void onMouseOver() override;
		void onMouseOut() override;

		// ボタンの状態
		enum State
		{
			NORMAL,
			MOUSE_DOWN,
			MOUSE_OVER,
			LEN
		};

	protected:
		bool createAnimation(int animationId);

	private:
		/*
		static const int ACCENT_WIDTH = 25;			//! アクセント部分の幅
		static const int TEXT_PADDING_LEFT = 25;	//! テキストの左余白
		static const int TEXT_SIZE = 32;			//! フォントサイズ
		static const int ANIMATION_MS = 100;		//! 拡大、縮小の時間
		static const float ANIMATION_SCALE;			//! サイズ倍率(floatはcppで定義する必要あり)
		*/

		bool ableSizeAnimation() const;

		//! テキストの座標
		int textX_;
		int textY_;

		//! テキスト
		string text_;

		//! フォント
		int font_;

		//! 現在のボタンの状態
		int nowState_;

		//! ボタン色(ボタンの各状態の場合を管理)
		int color_[State::LEN];

		//! 文字色(ボタンの各状態の場合を管理)
		int textColor_[State::LEN];

		//! 基準の大きさ
		Shape baseShape_;

		//! サイズアニメーションのサイズ変化率(1の場合はアニメーションなし)
		float sizeAnimationScale_;

		//! サイズアニメーション時間

		//! アニメーションクラス
		Animation sizeAnimation_;

		//! アニメーションID
		enum SizeAnimationId
		{
			EXPANSION,
			SHRINK
		};
	};


}