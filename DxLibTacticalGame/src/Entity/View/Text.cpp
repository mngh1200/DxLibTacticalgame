#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	Text::Text()
	{
		objectType = ViewObject::ObjectType::TEXT;
		text_ = "";
	}

	/**
	 * @fn
	 * コンストラクタ
	 */
	Text::Text(string text, Shape shape)
	{
		text_ = text;
		shape_ = shape;
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