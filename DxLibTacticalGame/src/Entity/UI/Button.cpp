#include "Button.h"

namespace Entity {

	Button::Button()
	{
		objectType = ObjectType::BUTTON;
	}

	/**
	 * @fn
	 * •`‰æˆ—
	 */
	void Button::render() const
	{
		// ƒeƒXƒg
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), DxLib::GetColor(227, 221, 210), TRUE);
		DrawString(shape_.x, shape_.y, "Close", DxLib::GetColor(90, 50, 0));
	}
}