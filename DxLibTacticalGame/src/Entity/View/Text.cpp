#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Text::Text() : text_(""), font_(-1), color_(-1)
	{
		type_ = TEXT;
	}

	/**
	 * @fn
	 * �R���X�g���N�^
	 * @param (text) �\������
	 * @param (x) x���W
	 * @param (y) y���W
	 * @param (font) �t�H���g
	 */
	Text::Text(string text, int x, int y, int font, int align, int color) : Text()
	{
		text_ = text;
		font_ = font;
		shape_.y = y;
		color_ = color;

		if (align == CENTER) // ��������
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text.c_str());
			shape_.x = x - width / 2;
		}
		else if (align == RIGHT) // �E����
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text.c_str());
			shape_.x = x - width;
		}
		else // ������
		{
			shape_.x = x;
			
		}
		font_ = DxLib::CreateFontToHandle("���C���I", 24, 3, DX_FONTTYPE_ANTIALIASING);

	}

	/**
	 * @fn
	 * �����F���Z�b�g
	 * @param (color) �F
	 */
	void Text::setColor(int color)
	{
		color_ = color;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Text::render() const
	{
		DxLib::DrawFormatStringToHandle(shape_.x, shape_.y, color_, font_, text_.c_str());
	}
}