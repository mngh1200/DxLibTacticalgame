#include "TerrainEffectDisplay.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	TerrainEffectDisplay::TerrainEffectDisplay() 
		: animation_{}
	{
		shape_.set(BATTLE_UI_AREA_MARGIN, BATTLE_UI_AREA_Y + BATTLE_UI_AREA_MARGIN, WIDTH, LINE_HEIGHT * LINE_COUNT);
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

			// 各行の表示
			for (int i = 0; i < LINE_COUNT; i++)
			{
				int y = shape_.y + LINE_HEIGHT * i;

				if (i == 0)
				{
					DxLib::DrawBox(shape_.x + LINE_MARGIN, y + LINE_MARGIN, shape_.getX2() - LINE_MARGIN, y + LINE_HEIGHT - LINE_MARGIN, rm.getColor(ColorType::SUB_COLOR_LITE), TRUE);
				}

				string text = targetMass_->getText(i);
				if (text != "")
				{
					const int space = LINE_MARGIN + LINE_PADDING;
					DxLib::DrawStringToHandle(shape_.x + space, y + space, text.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S18));
				}
			}
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
			return animation_.update(&shape_.y, ANIMATION_Y0, BATTLE_UI_AREA_Y + BATTLE_UI_AREA_MARGIN);
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
			animation_ = Animation(800, 0, 1, 0, Easing::InOutBounce<float>);
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