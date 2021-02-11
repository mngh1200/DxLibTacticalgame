#include "SelectUnitArea.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	SelectUnitArea::SelectUnitArea() 
		: selectUnitId_(-1), countPos_{0, 0}, countMax_(0), count_(0)
	{
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING,
			COUNT_WIDTH + UNIT_MARGIN + (UNIT_MARGIN + CHIP_SIZE) * UNIT_KIND_LEN, BUI_H - BUI_PADDING * 2);

		// ���j�b�g�I�����̋�`�Z�b�g
		for (int i = 0; i < UNIT_KIND_LEN; ++i)
		{
			unitList_.push_back(Shape(shape_.x + COUNT_WIDTH + UNIT_MARGIN + (UNIT_MARGIN + CHIP_SIZE) * i, UNIT_Y, CHIP_SIZE, CHIP_SIZE));
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void SelectUnitArea::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		
		// �J�E���g����
		string countText = to_string(count_) + "/" + to_string(countMax_);
		DxLib::DrawStringToHandle(countPos_.first, countPos_.second, countText.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FONT_TYPE));

		// ���j�b�g�I��
		int count = 0;
		for (auto itr = unitList_.begin(); itr != unitList_.end(); ++itr)
		{
			int colorType = ColorType::MAIN_COLOR_ON;

			if (selectUnitId_ == count)
			{
				colorType = ColorType::MAIN_COLOR_ON_DARK;
			}

			DxLib::DrawBox((*itr).x, (*itr).y, (*itr).getX2(), (*itr).getY2(), rm.getColor(colorType), TRUE);
			DxLib::DrawGraph((*itr).x, (*itr).y, rm.getImage(ImageType::UNIT, UnitImageKind::NORMAL, count), TRUE);
			++count;
		}
	}

	/**
	 * @fn
	 * �N���b�N�C�x���g
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	void SelectUnitArea::onMouseClick(int x, int y)
	{
		// ���j�b�g�I��
		int count = 0;
		for (auto itr = unitList_.begin(); itr != unitList_.end(); ++itr)
		{
			if ((*itr).isHit(x, y) && selectUnitId_ != count)
			{
				Utility::ResourceManager::playSound(SoundKind::CHECK);
				selectUnitId_ = count;
			}
			++count;
		}
	}


	/**
	 * @fn
	 * �ő�z�u�\�����Z�b�g
	 * @param (countMax) �ő�z�u�\��
	 */
	void SelectUnitArea::setCountMax(int countMax)
	{
		countMax_ = countMax;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// ���������ɂ���
		int x, y, w, h, lineCount;
		string tmpText = to_string(countMax_) + "/" + to_string(countMax_);

		DxLib::GetDrawStringSizeToHandle(&w, &h, &lineCount, tmpText.c_str(),
			DxLib::GetStringLength(tmpText.c_str()), rm.getHdlFont(FONT_TYPE));

		x = shape_.x + (COUNT_WIDTH - w) / 2;
		y = shape_.y + (shape_.h - h) / 2;

		countPos_ = make_pair(x, y);
	}

	/**
	 * @fn
	 * �J�E���g����
	 * @return �ő�\���ɂ�鐧���ŃJ�E���g�����ł��Ȃ������ꍇ��false��Ԃ�
	 */
	bool SelectUnitArea::addCount()
	{
		++count_;

		if (countMax_ < count_)
		{
			count_ = countMax_;
			return false;
		}
		return true;
	}

	/**
	 * @fn
	 * �J�E���g����
	 */
	void SelectUnitArea::removeCount()
	{
		--count_;
		if (count_ < 0)
		{
			count_ = 0;
		}
	}
}