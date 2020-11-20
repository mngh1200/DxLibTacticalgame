#include "Button.h"

namespace Entity {

	Button::Button()
	{
		objectType = ObjectType::BUTTON;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Button::render() const
	{
		// テスト
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), DxLib::GetColor(227, 221, 210), TRUE);
		DrawString(shape_.x, shape_.y, "Close", DxLib::GetColor(90, 50, 0));
	}
}