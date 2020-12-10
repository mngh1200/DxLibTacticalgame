#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "Utility/FontManager.h"

using namespace std;

/**
 * @file TextButton.h
 * @brief テキスト付きボタン要素
 */

namespace Entity
{
	class TextButton : public Object
	{
	public:
		TextButton();
		TextButton(int colorType, int textColorType);
		virtual ~TextButton() {};

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setText(string text);
		void setText(string text, int fontType);
		void setTextX(int x);
		void setFont(int fontType);

		void setColor(int backgroundColorType, int textColorType, int state = ALL);
		void render() const override;

		// ボタンの状態
		enum State
		{
			NORMAL,
			MOUSE_DOWN,
			MOUSE_OVER,
			ALL
		};


	private:
		void setTextCenter();
		void setTextMiddle();

		//! 中央揃えにするか
		bool isCenter_;

		//! テキストの座標
		int textX_;
		int textY_;

		//! テキスト
		string text_;

		//! フォント
		int font_;

		//! ボタン色(ボタンの各状態の場合を管理)
		int color_[State::ALL];

		//! 文字色(ボタンの各状態の場合を管理)
		int textColor_[State::ALL];
	};


}