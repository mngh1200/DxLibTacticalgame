#include "SelectUnitArea.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	SelectUnitArea::SelectUnitArea() 
		: selectUnitId_(-1)
	{
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING,
			UNIT_MARGIN + (UNIT_MARGIN + CHIP_SIZE) * UNIT_KIND_LEN, BUI_H - BUI_PADDING * 2);

		// ユニット選択肢の矩形セット
		for (int i = 0; i < UNIT_KIND_LEN; ++i)
		{
			unitList_.push_back(Shape(shape_.x + UNIT_MARGIN + (UNIT_MARGIN + CHIP_SIZE) * i, UNIT_Y, CHIP_SIZE, CHIP_SIZE));
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
}