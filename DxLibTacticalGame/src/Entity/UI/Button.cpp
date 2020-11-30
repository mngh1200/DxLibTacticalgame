#include "Button.h"

namespace Entity {

	Button::Button() : text_("")
	{
		type_ = BUTTON;
	}

	/**
	 * @fn
	 * �R���X�g���N�^
	 * @param (text) �{�^���̕���
	 * @param (x) x���W
	 * @param (y) y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	Button::Button(string text, int& x, int& y, int& w, int& h) : Button()
	{
		shape_ = Shape(x, y, w, h);
		text_ = text;
	}
}