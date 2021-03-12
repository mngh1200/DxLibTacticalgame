#include "RadioButton.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	RadioButton::RadioButton() :
		relationTextY_(0),
		selectedNum_(0),
		items_ {},
		itemW_ (0),
		itemH_ (0)
	{
		// �f�t�H���g�w��
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		font_ = rm.getHdlFont(FontType::NORMAL_S24);

		backColor_ = rm.getColor(ColorType::SUB_COLOR_LITE);
		textColor_ = rm.getColor(ColorType::SUB_COLOR);
		selectedBackColor_ = rm.getColor(ColorType::MAIN_COLOR_ON);
	}

	/**
	 * @fn
	 * �{�^���ʒu�w��
	 * @param (x) X���W
	 * @param (y) Y���W
	 */
	void RadioButton::setPos(int x, int y)
	{
		// �e�L�X�g�ƃ{�^���̑��΍��W���v�Z
		shape_.x = x;
		shape_.y = y;

		adjustShape();
	}

	/**
	 * @fn
	 * ���ڃ{�^���̃T�C�Y�w��
	 * @param (w) ��
	 * @param (h) ����
	 */
	void RadioButton::setItemSize(int w, int h)
	{
		itemW_ = w;
		shape_.h = itemH_ = h;
		
		relationTextY_ = (h - DxLib::GetFontSizeToHandle(font_)) / 2;

		adjustShape();
	}

	/**
	 * @fn
	 * ���ڂ̃e�L�X�g�w��
	 * @param (textList) ���ڂ̃e�L�X�g���X�g
	 */
	void RadioButton::setItems(vector<string>& textList)
	{
		for (auto itr = textList.begin(); itr != textList.end(); ++itr)
		{
			items_.push_back(RadioButtonItem{ *itr });
		}

		adjustShape();
	}

	/**
	 * @fn
	 * �F�w��
	 * @param (backColorType)  �w�i�F
	 * @param (selectedBackColor)  �I�����̔w�i�F
	 * @param (textColorType)  �e�L�X�g�F
	 */
	void RadioButton::setColor(int backColorType, int selectedBackColorType, int textColorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		backColor_ = rm.getColor(backColorType);
		selectedBackColor_ = rm.getColor(selectedBackColorType);
		textColor_ = rm.getColor(textColorType);
	}

	/**
	 * @fn
	 * �t�H���g�w��
	 * @param (fontType) �t�H���g�^�C�v
	 */
	void RadioButton::setFont(int fontType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		font_ = rm.getHdlFont(fontType);

		// �e�L�X�g�̑��Έʒu����
		relationTextY_ = (shape_.h - DxLib::GetFontSizeToHandle(font_)) / 2;

		setTextCenter();
	}

	/**
	 * @fn
	 * ����̍��ڑI��
	 * @param (num) �I�����鍀�ڔԍ�
	 */
	void RadioButton::select(int num)
	{
		selectedNum_ = num;
	}

	/**
	 * @fn
	 * �`��
	 */
	void RadioButton::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �e����
		int count = 0;
		const int textY = shape_.y + relationTextY_;
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			RadioButtonItem item = *itr;

			int backColor = backColor_;

			if (count == selectedNum_)
			{
				backColor = selectedBackColor_;
			}

			// �w�i
			DxLib::DrawBox(item.shape.x, item.shape.y, item.shape.getX2(), item.shape.getY2(), backColor, TRUE);

			// �e�L�X�g
			DxLib::DrawFormatStringToHandle(item.shape.x + item.relationTextX, textY, textColor_, font_, item.name.c_str());
		
			++count;
		}
	}

	/**
	 * @fn
	 * �N���b�N�C�x���g
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	void RadioButton::onMouseClick(int x, int y)
	{
		int count = 0;
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			if ((*itr).shape.isHit(x, y)) // ���ڑI����
			{
				selectedNum_ = count;
				Utility::ResourceManager::playSound(SoundKind::CHECK);
				break;
			}
			++count;
		}
	}

	/**
	 * @fn
	 * �I���e�L�X�g
	 * @param (text) �I�����Ă���e�L�X�g�ԋp�p�̎Q��
	 */
	void RadioButton::getSelectedText(string& text) const
	{
		text = items_.at(selectedNum_).name;
	}

	/**
	 * @fn
	 * �e�L�X�g����������
	 */
	void RadioButton::setTextCenter()
	{
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, (*itr).name.c_str());
			(*itr).relationTextX = (itemW_ - width) / 2;
		}
	}

	/**
	 * @fn
	 * �S�̂̕�����
	 */
	void RadioButton::adjustShape()
	{
		shape_.w = itemW_ * items_.size();

		int i = 0;
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			(*itr).shape = Shape(shape_.x + i * (itemW_ + ITEM_MARGIN), shape_.y, itemW_, itemH_);
			++i;
		}

		setTextCenter();
	}

}