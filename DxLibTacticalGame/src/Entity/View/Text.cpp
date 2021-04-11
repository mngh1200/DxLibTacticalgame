#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Text::Text() :
		text_(""),
		font_(-1),
		color_(-1),
		align_(Align::LEFT),
		baseX_(0),
		backgroundColor_(-1),
		padding_(0)
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

		// �T�C�Y�擾
		int lineCount;
		DxLib::GetDrawStringSizeToHandle(&shape_.w, &shape_.h, &lineCount, text, DxLib::GetStringLength(text), font_);

		shape_.w += padding_ * 2; // �]���ǉ�
		shape_.h += padding_ * 2; // �]���ǉ�

		adjustAlign();
	}

	/**
	 * @fn
	 * �]���Z�b�g
	 * @param (padding) �]��
	 */
	void Text::setPadding(int padding)
	{
		// �]���ύX�ɂ��T�C�Y����
		shape_.w += padding - padding_;
		shape_.h += padding - padding_;

		padding_ = padding;

		adjustAlign();
	}
	
	/**
	 * @fn
	 * �w�i�F�Z�b�g
	 * @param (backgroundColorType) �Z�b�g����J���[�^�C�v
	 */
	void Text::setBackgroundColor(int backgroundColorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		backgroundColor_ = rm.getColor(backgroundColorType);
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
		// �w�i�F���Z�b�g����Ă���ꍇ
		if (backgroundColor_ != -1)
		{
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), backgroundColor_, TRUE);
		}

		DxLib::DrawFormatStringToHandle(shape_.x + padding_, shape_.y + padding_, color_, font_, text_.c_str());
	}

	/**
	 * @fn
	 * ���������ɂ�钲��
	 */
	void Text::adjustAlign()
	{
		if (align_ == CENTER) // ��������
		{
			shape_.x = baseX_ - shape_.w / 2;
		}
		else if (align_ == RIGHT) // �E����
		{
			shape_.x = baseX_ - shape_.w;
		}
		else // ������
		{
			shape_.x = baseX_;
		}
	}
}