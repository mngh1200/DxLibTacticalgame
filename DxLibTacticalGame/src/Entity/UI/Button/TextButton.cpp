#include "TextButton.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	TextButton::TextButton() :
		text_(""),
		isCenter_(true),
		textX_(0),
		textY_(0),
		color_{},
		textColor_{}
	{
		type_ = Type::BUTTON;

		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		font_ = fontManager.getHdlFont(FontType::NORMAL);
	}

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	TextButton::TextButton(int colorType, int textColorType) : TextButton()
	{
		setColor(colorType, textColorType, State::ALL);
	}

	/**
	 * @fn
	 * �{�^���ʒu�ƃT�C�Y�w��
	 * @param (x) X���W
	 * @param (y) Y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	void TextButton::setShape(int x, int y, int w, int h)
	{
		setPos(x, y);
		setSize(w, h);
	}

	/**
	 * @fn
	 * �{�^���ʒu�w��
	 * @param (x) X���W
	 * @param (y) Y���W
	 */
	void TextButton::setPos(int x, int y)
	{
		// �e�L�X�g�ƃ{�^���̑��΍��W���v�Z
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * �{�^���T�C�Y�w��
	 * @param (w) ��
	 * @param (h) ����
	 */
	void TextButton::setSize(int w, int h)
	{
		shape_.w = w;
		shape_.h = h;
		
		// ���S�Ƀe�L�X�g�𑵂���
		setTextCenter();
		setTextMiddle();
	}

	/**
	 * @fn
	 * �e�L�X�g�w��
	 * @param (text) �e�L�X�g
	 */
	void TextButton::setText(string text)
	{
		text_ = text;
		setTextCenter();
	}

	/**
	 * @fn
	 * �e�L�X�g�w��
	 * @param (text) �e�L�X�g
	 * @param (x) �{�^���ƃe�L�X�g�̑��΍��W
	 * @param (fontType) �t�H���g
	 */
	void TextButton::setTextX(int x)
	{
		textX_ = x;
		isCenter_ = false;
	}

	/**
	 * @fn
	 * �e�L�X�g�w��(�{�^���̒����ɔz�u)
	 * @param (text) �e�L�X�g
	 * @param (fontType) �t�H���g
	 */
	void TextButton::setText(string text, int fontType)
	{
		text_ = text;
		setFont(fontType);
	}

	/**
	 * @fn
	 * �t�H���g�w��
	 * @param (fontType) �t�H���g
	 */
	void TextButton::setFont(int fontType)
	{
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		font_ = fontM.getHdlFont(fontType);
		setTextCenter();
		setTextMiddle();
	}

	/**
	 * @fn
	 * �{�^���̐F�w��
	 * @param (backgroundColor) �{�^���F
	 * @param (textColor) �e�L�X�g�F
	 * @param (state) �Ώۂ̏��
	 */
	void TextButton::setColor(int backgroundColorType, int textColorType, int state)
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		if (state == State::ALL)
		{
			
			for (int i = 0; i < State::ALL; ++i)
			{
				color_[i] = fontManager.getColor(backgroundColorType);
				textColor_[i] = fontManager.getColor(textColorType);
			}
		}
		else
		{
			color_[state] = fontManager.getColor(backgroundColorType);
			textColor_[state] = fontManager.getColor(textColorType);
		}
	}

	/**
	 * @fn
	 * �{�^���̔w�i�F�w��
	 * @param (backgroundColor) �{�^���F
	 * @param (state) �Ώۂ̏��
	 */
	void TextButton::setBackgroundColor(int backgroundColorType, int state)
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		if (state == State::ALL)
		{

			for (int i = 0; i < State::ALL; ++i)
			{
				color_[i] = fontManager.getColor(backgroundColorType);
			}
		}
		else
		{
			color_[state] = fontManager.getColor(backgroundColorType);
		}
	}

	/**
	 * @fn
	 * �{�^���̃e�L�X�g�F�w��
	 * @param (textColorType) �e�L�X�g�F
	 * @param (state) �Ώۂ̏��
	 */
	void TextButton::setTextColor(int textColorType, int state)
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		if (state == State::ALL)
		{

			for (int i = 0; i < State::ALL; ++i)
			{
				textColor_[i] = fontManager.getColor(textColorType);
			}
		}
		else
		{
			textColor_[state] = fontManager.getColor(textColorType);
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void TextButton::render() const
	{
		// ���݂̏�Ԏ擾
		int nowState = State::NORMAL;
		if (isMouseDown_)
		{
			nowState = State::MOUSE_DOWN;
		}
		else if (isMouseOver_)
		{
			nowState = State::MOUSE_OVER;
		}

		// �{�^��
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), color_[nowState], TRUE);

		// �e�L�X�g
		DxLib::DrawFormatStringToHandle(shape_.x + textX_, shape_.y + textY_, textColor_[nowState], font_, text_.c_str());
	}

	/**
	 * @fn
	 * �e�L�X�g�𒆉�����
	 */
	void TextButton::setTextCenter()
	{
		if (isCenter_)
		{
			// ���������ɂ��邽�ߍ��W�擾
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text_.c_str());
			textX_ = (shape_.w - width) / 2;
		}
	}

	/**
	 * @fn
	 * �e�L�X�g�𒆒i����
	 */
	void TextButton::setTextMiddle()
	{
		// ���i�ɕ\�����邽�߂̑��΍��W�擾
		int fontHeight = DxLib::GetFontSizeToHandle(font_);
		textY_ = (shape_.h - fontHeight) / 2;
	}

}