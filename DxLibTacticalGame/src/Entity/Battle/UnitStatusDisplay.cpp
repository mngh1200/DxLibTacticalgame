#include "UnitStatusDisplay.h"


namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	UnitStatusDisplay::UnitStatusDisplay() 
		: animation_{}
	{
		shape_.set(X, BATTLE_UI_AREA_Y + BUI_PADDING, WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
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

			/* 一行目 ここから */

			// 名前表示
			int nameColorType = targetUnit_->isEnemy() ? ColorType::ENEMY_COLOR : ColorType::PLAYER_COLOR;
			BUI::drawLabel(shape_.x, shape_.y, targetUnit_->getName(), BUI::getZenW(3), nameColorType, ColorType::WHITE);

			/* 二行目 ここから */

			int x = shape_.x;
			int y = shape_.y + BUI_LINE_HEIGHT;

			// HP
			x = BUI::drawLabel(x, y, "HP", BUI::getHanW(2));
			x = BUI::drawValue(x, y, to_string(targetUnit_->getHpv()) + " / " + to_string(targetUnit_->getHpm()), BUI::getHanW(7));

			// 攻撃力
			x = BUI::drawLabel(x, y, "攻", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(targetUnit_->getAtk()), BUI::getHanW(3));

			// 防御
			x = BUI::drawLabel(x, y, "防", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(targetUnit_->getDef()), BUI::getHanW(3));

			// 射程
			x = BUI::drawLabel(x, y, "射程", BUI::getZenW(2));
			x = BUI::drawValue(x, y, targetUnit_->getLenText(), BUI::getHanW(2));
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
			return animation_.update(&shape_.y, ANIMATION_Y0, BATTLE_UI_AREA_Y + BUI_PADDING);
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
			animation_ = Animation(500, 0, 1, 0, Easing::InOutBounce<float>);
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