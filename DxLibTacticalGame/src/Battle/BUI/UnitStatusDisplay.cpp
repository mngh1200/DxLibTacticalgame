#include "UnitStatusDisplay.h"


namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	UnitStatusDisplay::UnitStatusDisplay() 
		: animation_{}, extraStatusHoverId_ (EXTRA_STATUS_ID_NONE)
	{
		shape_.set(X, WIN_H, WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
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

			UnitInfo info = targetUnit_->getInfo();

			// 名前表示
			int nameColorType = targetUnit_->isEnemy() ? ColorType::ENEMY_COLOR : ColorType::PLAYER_COLOR;
			BUI::drawLabel(shape_.x, shape_.y, info.name, NAME_W, nameColorType, ColorType::WHITE);

			// 特殊ステータス
			for (auto itr = extraStatusList_.begin(); itr != extraStatusList_.end(); ++itr)
			{
				BUI::drawLabel(itr->shape.x, shape_.y, itr->label, itr->shape.w - BUI_LINE_PADDING * 2);
			}

			/* 二行目 ここから */

			int x = shape_.x;
			int y = shape_.y + BUI_LINE_HEIGHT;

			// HP
			x = BUI::drawLabel(x, y, "HP", BUI::getHanW(2));
			x = BUI::drawValue(x, y, to_string(info.hp) + " / " + to_string(info.hpm), BUI::getHanW(7));

			// 攻撃力
			x = BUI::drawLabel(x, y, "攻", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(info.atk), BUI::getHanW(3));

			// 防御
			x = BUI::drawLabel(x, y, "防", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(info.def), BUI::getHanW(3));

			// 射程
			x = BUI::drawLabel(x, y, "射程", BUI::getZenW(2));
			x = BUI::drawValue(x, y, info.getLenText(), BUI::getHanW(2));
		}
	}

	/**
	 * @fn
	 * マウスイベントによる更新処理
	 */
	void UnitStatusDisplay::updateByEvents(int x, int y)
	{
		// 特殊ステータスのツールチップの表示/非表示判定
		bool isHoverExtraStatus = false;
		if (isMouseOver_)
		{
			int count = 0;
			for (auto itr = extraStatusList_.begin(); itr != extraStatusList_.end(); ++itr)
			{
				if (itr->shape.isHit(x, y))
				{
					isHoverExtraStatus = true;
					if (extraStatusHoverId_ != count)
					{
						// ツールチップ表示
						extraStatusHoverId_ = count;

						if (!tooltip_)
						{
							// ツールチップオブジェクト追加
							FrameWork::Game& game = FrameWork::Game::getInstance();
							Entity::ObjectsControl& objectsControl = game.objectsControl;

							tooltip_ = make_shared<Tooltip>();
							objectsControl.addFigure(getLayerId(), tooltip_);
						}

						tooltip_->show(shape_.x, shape_.y - BUI_PADDING - 5, itr->description);
					}
					break;
				}
				++count;
			}
		}

		if (!isHoverExtraStatus)
		{
			// ツールチップ非表示
			extraStatusHoverId_ = EXTRA_STATUS_ID_NONE;
			if (tooltip_)
			{
				tooltip_->hide();
			}
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
	 * @param (animationId) アニメーションの種類
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
	 * @param (unit) 対象ユニット
	 */
	void UnitStatusDisplay::setTargetUnit(shared_ptr<Unit> unit)
	{
		if (!unit || targetUnit_ == unit) // 選択済みのユニットの場合
		{
			return;
		}

		targetUnit_ = unit;

		// 特殊ステータスの表示データ生成
		extraStatusList_.clear();

		vector<pair<string, string>> extraStatusTexts;
		unit->getExtraStatusList(extraStatusTexts);

		int x = shape_.x + NAME_W + BUI_LINE_PADDING * 2 + BUI_LINE_MARGIN + EXTRA_STATUS_MARGIN;
		const int y = shape_.y + BUI_LINE_MARGIN;
		const int h = BUI_FONT_SIZE + BUI_LINE_PADDING * 2;

		for (auto itr = extraStatusTexts.begin(); itr != extraStatusTexts.end(); ++itr)
		{
			int w = BUI::getZenW((itr->first).size() / 2) + BUI_LINE_PADDING * 2;
			extraStatusList_.push_back(ExtraStatus{ itr->first, itr->second, Shape(x, y ,w ,h)});
			x = x + w + EXTRA_STATUS_MARGIN;
		}

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