#include "Button.h"

namespace Entity {

	Button::Button() : text_("")
	{
		type_ = BUTTON;
	}

	/**
	 * @fn
	 * コンストラクタ
	 * @param (text) ボタンの文言
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	Button::Button(string text, int& x, int& y, int& w, int& h) : Button()
	{
		shape_ = Shape(x, y, w, h);
		text_ = text;
	}
}