#include "Button.h"

namespace Entity {

	Button::Button()
	{
		type_ = BUTTON;
	}

	Button::Button(Shape shape) : Button()
	{
		shape_ = shape;
	}

	// �e�X�g
	void Button::onMouseOver()
	{
		// �A�j���[�V�����ǉ�
		FrameWork::Game::getInstance().objectsControl.addAnimationObj(1, getLayerId(), getObjectId(), false);
	}

	// �e�X�g
	bool Button::animationUpdate()
	{
		if (animationId_ == 1)
		{
			if (shape_.x >= 500)
			{
				return true;
			}
			else
			{
				shape_.x += 1;
			}
		}

		return false;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Button::render() const
	{
		// �e�X�g
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), DxLib::GetColor(227, 221, 210), TRUE);
		DxLib::DrawString(shape_.x, shape_.y, "Close", DxLib::GetColor(90, 50, 0));
	}
}