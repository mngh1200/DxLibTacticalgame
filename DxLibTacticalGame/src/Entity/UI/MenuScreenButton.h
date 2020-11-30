#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "Utility/FontManager.h"

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


		void onMouseLeftDown() override;
		void onMouseLeftUp() override;
		void onMouseOver() override;
		void onMouseOut() override;

	private:
		//! アクセント部分の幅
		static const int ACCENT_WIDTH = 25;

		//! テキストの左余白
		static const int TEXT_PADDING_LEFT = 25;

		//! フォントサイズ
		static const int TEXT_SIZE = 32;

		//! テキストのY座標
		int textY_;

		//! テキスト
		string text_;

		//! ボタン色
		int backgroundColor_;

		//! 文字色
		int textColor_;
	};
}