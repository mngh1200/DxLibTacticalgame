#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Text::Text() : text_(""), font_(-1), color_(-1), align_(Align::LEFT), baseX_(0)
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
	Text::Text(const char* text, int x, int y, int fontType, int colorType, int align) : Text()
	{
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();
		align_ = align;

		baseX_ = x;
		shape_.y = y;
		font_ = resourceManager.getHdlFont(fontType);
		color_ = resourceManager.getColor(colorType);

		setText(text);
	}

	/**
	 * @fn
	 * �e�L�X�g�Z�b�g
	 * @param (text) �ύX�e�L�X�g
	 */
	void Text::setText(const char* text)
	{
		text_ = text;

		if (align_ == CENTER) // ��������
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text);
			shape_.x = baseX_ - width / 2;
		}
		else if (align_ == RIGHT) // �E����
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text);
			shape_.x = baseX_ - width;
		}
		else // ������
		{
			shape_.x = baseX_;
		}
	}

	/**
	 * @fn
	 * y���W�Z�b�g
	 * @param (y) y���W
	 */
	void Text::setY(int y)
	{
		shape_.y = y;
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