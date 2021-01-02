#include "TerrainEffectDisplay.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	TerrainEffectDisplay::TerrainEffectDisplay() 
		: animation_{}
	{
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING, WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
	}

	/**
	 * @fn
	 * 描画
	 */
	void TerrainEffectDisplay::render() const
	{
		if (targetMass_ && targetMass_->getKind() != Mass::Kind::OUT_OF_MAP)
		{

			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// エリア
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);

			// ラベル
			BUI::drawLabel(shape_.x, shape_.y, targetMass_->getText(0), shape_.w - (BUI_LINE_MARGIN + BUI_LINE_PADDING) * 2);
			
			// 説明文
			BUI::drawValue(shape_.x, shape_.y + BUI_LINE_HEIGHT, targetMass_->getText(1), shape_.w);
		}
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool TerrainEffectDisplay::animationUpdate()
	{
		if (animationId_ == AnimationKind::DISPLAY)
		{
			return animation_.update(&shape_.y, ANIMATION_Y0, BATTLE_UI_AREA_Y + BUI_PADDING);
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション生成
	 * @param (animationId) アニメーションの種類
	 */
	bool TerrainEffectDisplay::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::DISPLAY)
		{
			shape_.y = ANIMATION_Y0;
			animation_ = Animation(500, 0, 1, 0, Easing::InOutBounce<float>);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 対象マス指定
	 * @param (mass) 対称マス
	 */
	void TerrainEffectDisplay::setTargetMass(shared_ptr<Mass> mass)
	{
		if (!mass || targetMass_ == mass) // 選択済みのマスの場合
		{
			return;
		}

		// 前回値
		int prevKind = -1;
		
		if (targetMass_)
		{
			prevKind = targetMass_->getKind();
		}

		targetMass_ = mass;

		if (mass->getKind() != prevKind) // 地形の種類か表示位置が変わっている場合のみアニメーション実施
		{
			changeAnimation(AnimationKind::DISPLAY);
		}
	}

	/**
	 * @fn
	 * 対象マスクリア
	 */
	void TerrainEffectDisplay::clear()
	{
		targetMass_.reset();
		animation_.reset();
	}


}