#include "SelectUnitArea.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	SelectUnitArea::SelectUnitArea() 
		: selectUnitId_(-1), countPos_{0, 0}, countMax_(0), count_(0)
	{
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING,
			COUNT_WIDTH + UNIT_MARGIN + (UNIT_MARGIN + CHIP_SIZE) * UNIT_KIND_LEN, BUI_H - BUI_PADDING * 2);

		// ユニット選択肢の矩形セット
		for (int i = 0; i < UNIT_KIND_LEN; ++i)
		{
			unitList_.push_back(Shape(shape_.x + COUNT_WIDTH + UNIT_MARGIN + (UNIT_MARGIN + CHIP_SIZE) * i, UNIT_Y, CHIP_SIZE, CHIP_SIZE));
		}
	}

	/**
	 * @fn
	 * 描画
	 */
	void SelectUnitArea::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		
		// カウント部分
		string countText = to_string(count_) + "/" + to_string(countMax_);
		DxLib::DrawStringToHandle(countPos_.first, countPos_.second, countText.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FONT_TYPE));

		// ユニット選択
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
	 * クリックイベント
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void SelectUnitArea::onMouseClick(int x, int y)
	{
		// ユニット選択
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
	 * 最大配置可能数をセット
	 * @param (countMax) 最大配置可能数
	 */
	void SelectUnitArea::setCountMax(int countMax)
	{
		countMax_ = countMax;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 中央揃えにする
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
	 * カウント増加
	 * @return 最大可能数による制限でカウント増加できなかった場合はfalseを返す
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
	 * カウント減少
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