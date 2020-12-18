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
	 * @param (fontType) �Ώۃt�H���g�ifontManager��FontType��enum�l�j
	 * @param (colorType) �e�L�X�g�F�ifontManager��ColorType��enum�l�j
	 * @param (align) x���W�̊�i���A�����A�E�������w��j
	 */
	Text::Text(string text, int x, int y, int fontType, int colorType, int align) : Text()
	{
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();

		text_ = text;
		shape_.y = y;
		font_ = resourceManager.getHdlFont(fontType);
		color_ = resourceManager.getColor(colorType);

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