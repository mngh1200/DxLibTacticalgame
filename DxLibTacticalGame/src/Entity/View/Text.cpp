#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Text::Text() : text_("")
	{
		objectType = ViewObject::ObjectType::TEXT;
	}

	/**
	 * @fn
	 * �R���X�g���N�^
	 * @param (text) �\������
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	Text::Text(string text, int x, int y) : Text()
	{
		text_ = text;
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Text::render() const
	{
		DxLib::DrawString(shape_.x, shape_.y, text_.c_str(), DxLib::GetColor(90, 50, 0));
	}
}