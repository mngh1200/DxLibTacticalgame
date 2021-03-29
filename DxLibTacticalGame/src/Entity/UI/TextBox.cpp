#include "TextBox.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	TextBox::TextBox() :
		relationTextX_(10),
		relationTextY_(0),
		inputHandle_(-1),
		text_(""),
		borderWidth_(2),
		maxLength_(INT_MAX),
		dataType_(DataType::ALL)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		font_ = rm.getHdlFont(FontType::NORMAL_S32);

		backColor_ = rm.getColor(ColorType::MAIN_COLOR);
		textColor_ = rm.getColor(ColorType::SUB_COLOR);
		borderColor_ = rm.getColor(ColorType::SUB_COLOR_LITE);
		borderColorOnFocus_ = rm.getColor(ColorType::SUB_COLOR);

		type_ = Type::TEXT_BOX;
	}

	/**
	 * @fn
	 * �f�X�g���N�^
	 */
	TextBox::~TextBox()
	{
		DxLib::DeleteKeyInput(inputHandle_);
	}

	/**
	 * @fn
	 * �{�^���ʒu�ƃT�C�Y�w��
	 * @param (x) X���W
	 * @param (y) Y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	void TextBox::setShape(int x, int y, int w, int h)
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
	void TextBox::setPos(int x, int y)
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
	void TextBox::setSize(int w, int h)
	{
		shape_.w = w;
		shape_.h = h;
		
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		relationTextY_ = (shape_.h - DxLib::GetFontSizeToHandle(font_)) / 2;
	}

	/**
	 * @fn
	 * �����̗]���w��
	 * @param (paddingLeft) �����̗]��
	 */
	void TextBox::setPaddingLeft(int paddingLeft)
	{
		relationTextX_ = paddingLeft;
	}

	/**
	 * @fn
	 * �e�L�X�g�Z�b�g
	 * @param (text) �e�L�X�g
	 */
	void TextBox::setText(const char* text)
	{
		text_ = text;
	}

	/**
	 * @fn
	 * �F�w��
	 * @param (backColorType)  �w�i�F
	 * @param (textColorType)  �e�L�X�g�F
	 * @param (borderColorType) �g���F
	 * @param (borderColorTypeOnFocus) �t�H�[�J�X���̘g���F 
	 */
	void TextBox::setColor(int backColorType, int textColorType, int borderColorType, int borderColorTypeOnFocus)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		backColor_ = rm.getColor(backColorType);
		textColor_ = rm.getColor(textColorType);
		borderColor_ = rm.getColor(borderColorType);
		borderColorOnFocus_ = rm.getColor(borderColorTypeOnFocus);
	}

	/**
	 * @fn
	 * �t�H���g�w��
	 * @param (fontType) �t�H���g�^�C�v
	 */
	void TextBox::setFont(int fontType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		font_ = rm.getHdlFont(fontType);

		relationTextY_ = (shape_.h - DxLib::GetFontSizeToHandle(font_)) / 2;
	}

	/**
	 * @fn
	 * ���̓f�[�^�^�C�v�w��
	 * @param (dataType) �f�[�^�^�C�v
	 */
	void TextBox::setDataType(DataType dataType)
	{
		dataType_ = dataType;
	}

	/**
	 * @fn
	 * �ő啶���񒷎w��
	 * @param (maxLength) �ő啶����
	 */
	void TextBox::setMaxLength(int maxLength)
	{
		maxLength_ = maxLength;
	}

	/**
	 * @fn
	 * �g���̕��w��
	 * @param (borderWidth) �g���̕�
	 */
	void TextBox::setBorderWidth(int borderWidth)
	{
		borderWidth_ = borderWidth;
	}

	/**
	 * @fn
	 * ���Ƀt�H�[�J�X����IP�A�h���X���͗�
	 */
	void TextBox::setNextInput(shared_ptr<TextBox> nextInput)
	{
		nextInput_ = nextInput;
	}

	/**
	 * @fn
	 * �`��
	 */
	void TextBox::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �g���Ɣw�i�F����
		int borderColor = borderColor_;

		if (isFocus_)
		{
			borderColor = borderColorOnFocus_;
		}


		// �g��
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), borderColor, TRUE);

		// �w�i
		DxLib::DrawBox(shape_.x + borderWidth_, shape_.y + borderWidth_,
			shape_.getX2() - borderWidth_, shape_.getY2() - borderWidth_, backColor_, TRUE);


		// �e�L�X�g
		if (DxLib::CheckKeyInput(inputHandle_) != -1)
		{
			DxLib::DrawKeyInputString(shape_.x + relationTextX_, shape_.y + relationTextY_, inputHandle_);
		}
		else
		{
			DxLib::DrawFormatStringToHandle(shape_.x + relationTextX_, shape_.y + relationTextY_, textColor_, font_, text_.c_str());
		}
	}

	/**
	 * @fn
	 * �t�H�[�J�X�C�x���g
	 */
	void TextBox::onFocus()
	{
		int state = DxLib::CheckKeyInput(inputHandle_); // �L�[���̓n���h���̏�Ԏ擾
		
		if (state == -1) // �L�[���̓n���h�������݂��Ȃ��ꍇ�A�쐬
		{
			// ���̓f�[�^�^�C�v�𔻒�
			if (dataType_ == DataType::ALL)
			{
				inputHandle_ = DxLib::MakeKeyInput(maxLength_, FALSE, FALSE, FALSE);
			}
			else if (dataType_ == DataType::HAN)
			{
				inputHandle_ = DxLib::MakeKeyInput(maxLength_, FALSE, TRUE, FALSE);
			}
			else if (dataType_ == DataType::NUM)
			{
				inputHandle_ = DxLib::MakeKeyInput(maxLength_, FALSE, FALSE, TRUE);
			}

			// ���͒������̐F��t�H���g���w��
			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
			DxLib::SetKeyInputStringColor(textColor_, textColor_,textColor_, textColor_, textColor_,
				textColor_, textColor_, textColor_);
			DxLib::SetKeyInputStringFont(font_);

			DxLib::SetKeyInputString(text_.c_str(), inputHandle_);
		}

		DxLib::SetActiveKeyInput(inputHandle_);
	}

	/**
	 * @fn
	 * �t�H�[�J�X�A�E�g�C�x���g
	 */
	void TextBox::onBlur()
	{
		DxLib::DeleteKeyInput(inputHandle_);
	}

	/**
	 * @fn
	 * �ύX�����������̊m�F�ƍX�V�����i�L�[�C�x���g�Ɠ��̓`�F�b�N�j
	 */
	bool TextBox::checkChangeAndUpdate()
	{
		if (!isFocus_)
		{
			return false; // �t�H�[�J�X�ȊO�͏����s�v
		}

		FrameWork::Controller& cont = FrameWork::Controller::getInstance();

		// ���͒��̓��e�擾
		char text[1024];
		DxLib::GetKeyInputString(text, inputHandle_);

		bool isChange = text_ != text; // �ύX��������������

		text_ = text;

		// �G���^�[�܂��̓^�u���������� �܂��́A�ő咷�܂œ��͂������@���̗v�f�ֈړ�
		if (cont.isKeyPressedNow(KEY_INPUT_TAB) || cont.isKeyPressedNow(KEY_INPUT_RETURN) || ((int)(text_.length()) >= maxLength_ && isChange))
		{
			shared_ptr<TextBox> nextInput = nextInput_.lock();
			if (nextInput)
			{
				nextInput->onForceFocus(); // ���̗v�f�t�H�[�J�X
			}
			onBlurBase(); // �t�H�[�J�X����
		}

		return isChange;
	}

}