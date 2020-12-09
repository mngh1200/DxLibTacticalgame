#include "TextButton.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	TextButton::TextButton(int colorType, int textColorType) :
		text_(""),
		textX_(0),
		textY_(0),
		sizeAnimationScale_(1),
		sizeAnimation_{},
		nowState_(State::NORMAL)
	{
		type_ = Type::BUTTON;
		baseShape_ = shape_;

		Utility::FontManager& fontManager = Utility::FontManager::getInstance();

		for (int i = 0; i < State::LEN; ++i)
		{
			color_[State::NORMAL] = fontManager.getColor(colorType);
			textColor_[State::NORMAL] = fontManager.getColor(textColorType);
		}

		font_ = fontManager.getHdlFont(FontType::NORMAL);
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
		baseShape_.x = shape_.x = x;
		baseShape_.y = shape_.y = y;
	}

	/**
	 * @fn
	 * �{�^���T�C�Y�w��
	 * @param (w) ��
	 * @param (h) ����
	 */
	void TextButton::setSize(int w, int h)
	{
		baseShape_.w = shape_.w = w;
		baseShape_.h = shape_.h = h;

		// ���������ɂ��邽�ߍ��W�擾
		int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text_.c_str());
		textX_ = (shape_.w - width) / 2;

		// ���i�ɕ\�����邽�߂̑��΍��W�擾
		int fontHeight = DxLib::GetFontSizeToHandle(font_);
		textY_ = (shape_.h - fontHeight) / 2;
	}

	/**
	 * @fn
	 * �e�L�X�g�w��
	 * @param (text) �e�L�X�g
	 */
	void TextButton::setText(string text)
	{
		text_ = text;
	}

	/**
	 * @fn
	 * �e�L�X�g�w��
	 * @param (text) �e�L�X�g
	 * @param (x) �{�^���ƃe�L�X�g�̑��΍��W
	 * @param (fontType) �t�H���g
	 */
	void TextButton::setText(string text, int x, int fontType)
	{
		text_ = text;
		textX_ = x;
		setFont(fontType);
	}

	/**
	 * @fn
	 * �e�L�X�g�w��(�{�^���̒����ɔz�u)
	 * @param (text) �e�L�X�g
	 * @param (fontType) �t�H���g
	 */
	void TextButton::setTextCenter(string text, int fontType)
	{
		text_ = text;
		setFont(fontType);

		// ���������ɂ��邽�ߍ��W�擾
		int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text_.c_str());
		textX_ = (shape_.w - width) / 2;
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

		// ���i�ɕ\�����邽�߂̑��΍��W�擾
		int fontHeight = DxLib::GetFontSizeToHandle(font_);
		textY_ = (shape_.h - fontHeight) / 2;
	}

	void TextButton::setColor(int backgroundColor, int textColor, int state)
	{
		color_[state] = backgroundColor;
		textColor_[state] = textColor;
	}

	/**
	 * @fn
	 * �`��
	 */
	void TextButton::render() const
	{
		// �{�^��
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), color_[nowState_], TRUE);

		// �e�L�X�g
		DxLib::DrawFormatStringToHandle(shape_.x + textX_, shape_.y + textY_, textColor_[nowState_], font_, text_.c_str());
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬(ObjectsControl::addAnimationObj���\�b�h��p�ŌĂяo��)
	 * @return �A�j���[�V�����쐬�\�ȏꍇtrue��Ԃ�
	 */
	bool TextButton::createAnimation(int animationId)
	{
		// ���Ɏ��s�ς݂̃A�j���[�V����������ꍇ�͍쐬�s��
		if (animationId == animationId_)
		{
			return false;
		}

		if (animationId == EXPANSION) // �g��
		{
			sizeAnimation_.reset(Animation::NORMAL);
			return true;
		}
		else if (animationId == SHRINK) // �k��
		{
			sizeAnimation_.reset(Animation::REVERSE);
			sizeAnimation_.adjustFrame(shape_, baseShape_, sizeAnimationScale_);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * �g��A�k���A�j���[�V�������Z�b�g
	 * @param (scale) �{��
	 * @param (timeMs) ����[ms]
	 */
	void TextButton::setSizeAnimation(float scale, int timeMs)
	{
		sizeAnimationScale_ = scale;
		sizeAnimation_ = Animation(timeMs);
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 * @return true:�I��
	 */
	bool TextButton::animationUpdate()
	{
		if (animationId_ == EXPANSION || animationId_ == SHRINK)
		{
			return sizeAnimation_.update(&shape_, baseShape_, sizeAnimationScale_);
		}
		return true;
	}


	/**
	 * @fn
	 * �}�E�X�����{�^���ŉ������ꂽ�u�Ԃ̏���
	 */
	void TextButton::onMouseLeftDown()
	{
		if (ableSizeAnimation())
		{
			joinAnimationList(EXPANSION); // �g��
		}

		// �T�E���h
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		DxLib::PlaySoundMem(fontM.getSound(SoundKind::MOUSE_DOWN), DX_PLAYTYPE_BACK);

	}

	/**
	 * @fn
	 * �}�E�X�̍��{�^���������グ��ꂽ�u�Ԃ̏���
	 */
	void TextButton::onMouseLeftUp()
	{
		if (ableSizeAnimation())
		{
			joinAnimationList(SHRINK); // �k��
		}
	}

	/**
	 * @fn
	 * �}�E�X�Ƀz�o�[���ꂽ�u�Ԃ̏���
	 */
	void TextButton::onMouseOver()
	{
		if (ableSizeAnimation())
		{
			joinAnimationList(EXPANSION);
		}
	}

	/**
	 * @fn
	 * �}�E�X���z�o�[��Ԃ��痣�ꂽ�Ƃ��̏���
	 */
	void TextButton::onMouseOut()
	{
		nowState_ = State::NORMAL;

		if (isMouseDown_ && ableSizeAnimation())
		{
			joinAnimationList(SHRINK); // �k��
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����\��
	 * @return true:�\
	 */
	bool TextButton::ableSizeAnimation() const
	{
		return sizeAnimationScale_ != 0;
	}

}