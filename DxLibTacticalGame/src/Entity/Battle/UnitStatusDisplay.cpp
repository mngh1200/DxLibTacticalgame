#include "UnitStatusDisplay.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	UnitStatusDisplay::UnitStatusDisplay() 
		: animation_{}
	{
		shape_.set(X, BATTLE_UI_AREA_Y + BATTLE_UI_AREA_MARGIN, WIDTH, LINE_HEIGHT * LINE_COUNT);
	}
	/**
	 * @fn
	 * 描画
	 */
	void UnitStatusDisplay::render() const
	{
		if (targetUnit_)
		{

			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// エリア
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		}
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool UnitStatusDisplay::animationUpdate()
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
	 */
	bool UnitStatusDisplay::createAnimation(int animationId)
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
	 * 対象ユニット指定
	 */
	void UnitStatusDisplay:: setTargetUnit(shared_ptr<Unit> unit)
	{
		if (!unit || targetUnit_ == unit) // 選択済みのユニットの場合
		{
			return;
		}

		targetUnit_ = unit;
		changeAnimation(AnimationKind::DISPLAY);
	}

	/**
	 * @fn
	 * 対象ユニットクリア
	 */
	void UnitStatusDisplay::clear()
	{
		targetUnit_.reset();
		animation_.reset();
	}


}