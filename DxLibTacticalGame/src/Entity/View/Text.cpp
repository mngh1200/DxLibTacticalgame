#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	Text::Text() : text_("")
	{
		objectType = ViewObject::ObjectType::TEXT;
	}

	/**
	 * @fn
	 * コンストラクタ
	 * @param (text) 表示文字
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	Text::Text(string text, int x, int y) : Text()
	{
		text_ = text;
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Text::render() const
	{
		DxLib::DrawString(shape_.x, shape_.y, text_.c_str(), DxLib::GetColor(90, 50, 0));
	}
}