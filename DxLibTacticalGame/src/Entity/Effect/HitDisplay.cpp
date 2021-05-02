#include "HitDisplay.h"
#include "Screen/BattleScreen.h"

namespace Entity {
	/**
	 * @fn
	 * 表示
	 * @param (atkUnit) 攻撃側ユニット
	 * @param (defUnit) 防御側ユニット
	 * @param (hitRate) 命中率
	 */
	void HitDisplay::show(Fight* fight)
	{
		const FightData& actData = fight->getFightData(true);
		const FightData& psvData = fight->getFightData(false);

		if (actData.unit->isEnemy())
		{
			return; // プレイヤーユニットでない場合は、表示しない
		}

		isShow_ = true;
		hitRate_ = actData.hitRate;

		// 表示位置計算
		Shape defUnitShape = psvData.unit->getShape();
		Shape atkUnitShape = actData.unit->getShape();

		shape_.x = defUnitShape.x;
		shape_.y = defUnitShape.y - MARGIN - H;

		shape_.w = LABEL_W + VALUE_W;
		shape_.h = H;

		// 上側に表示できない場合
		if (shape_.isHit(atkUnitShape) || shape_.y < 0)
		{
			// 右側に表示
			shape_.x = defUnitShape.getX2() + MARGIN;
			shape_.y = defUnitShape.y + MARGIN; 

			// 右側に表示できない場合
			if (shape_.isHit(atkUnitShape) || shape_.getX2() > WIN_W)
			{
				shape_.x = defUnitShape.x - MARGIN - shape_.w; // 左側に表示

				if (shape_.isHit(atkUnitShape) || shape_.x < 0) // 左側に表示できない場合
				{
					// 下側に表示
					shape_.x = defUnitShape.x;
					shape_.y = defUnitShape.getY2() + MARGIN;
				}
			}
		}

		// 上または下側に設置した場合に、右側がはみ出している場合、左側にずらす
		if (shape_.x == defUnitShape.x && shape_.getX2() > WIN_W)
		{
			shape_.x = WIN_W - MARGIN - shape_.w;
		}
	}

	/**
	 * @fn
	 * 非表示化
	 */
	void HitDisplay::hide()
	{
		isShow_ = false;
	}

	/**
	 * @fn
	 * 描画
	 */
	void HitDisplay::render() const
	{
		if (!isShow_)
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int valX = shape_.x + LABEL_W; // 値表示部分のx座標

		// 背景
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.x + LABEL_W, shape_.getY2(), rm.getColor(ColorType::SUB_COLOR), TRUE);
		DxLib::DrawBox(valX, shape_.y, valX + VALUE_W, shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 文字
		int colorType = ColorType::SUB_COLOR;
		if (hitRate_ < 100)
		{
			colorType = ColorType::ENEMY_COLOR;
		}

		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING, "Hit", rm.getColor(ColorType::MAIN_COLOR), rm.getHdlFont(FontType::NORMAL_S14));
		DxLib::DrawStringToHandle(valX + PADDING, shape_.y + PADDING, (to_string(hitRate_) + "%").c_str(), rm.getColor(colorType), rm.getHdlFont(FontType::NORMAL_S18));
	}
}