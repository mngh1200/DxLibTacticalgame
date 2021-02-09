#include "FightPredictDisplay.h"


namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	FightPredictDisplay::FightPredictDisplay() 
		: animation_{}
	{
		fight_ = nullptr;
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING);
		shape_.setSize(AREA_WIDTH * 2 + CENTER_WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * 描画
	 */
	void FightPredictDisplay::render() const
	{
		if (fight_ != nullptr && fight_->isPrepared())
		{
			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// 戦闘予測データ取得
			const Battle::FightData* playerFightData; 
			const Battle::FightData* enemyFightData;

			Battle::FightData actFightData = fight_->getFightData(true);
			Battle::FightData psvFightData = fight_->getFightData(false);

			if (!actFightData.unit->isEnemy())
			{
				playerFightData = &actFightData;
				enemyFightData = &psvFightData;
			}
			else
			{
				playerFightData = &psvFightData;
				enemyFightData = &actFightData;
			}

			// 戦闘予測データ描画
			drawFightPredict(shape_.x, enemyFightData, false); // 敵側
			drawFightPredict(shape_.x + PLAYER_X, playerFightData, true); // プレイヤー側

			// 攻撃方向描画
			if (playerFightData->isAtk)
			{
				if (!enemyFightData->isAtk)
				{
					drawAtackDirection(shape_.y + shape_.h / 4, true); // 攻撃側だけ
				}
				else if (fight_->isActSideFirst()) // 攻撃実行側先攻
				{
					drawAtackDirection(shape_.y, true);
					drawAtackDirection(shape_.y + shape_.h / 2, false);
				}
				else if (fight_->isActSideFirst()) // 防御側先攻
				{
					drawAtackDirection(shape_.y, false);
					drawAtackDirection(shape_.y + shape_.h / 2, true);
				}
			}
			else if (enemyFightData->isAtk)
			{
				drawAtackDirection(shape_.y + shape_.h / 4, false); // 防御側だけ
			}
		}
	}

	/**
	 * @fn
	 * 片方のユニットの戦闘予測欄描画
	 * @param (areaX) 表示エリアのx座標
	 * @param (fightData) 戦闘情報
	 * @param (isPlayer) プレイヤー側のユニットであるか
	 */
	void FightPredictDisplay::drawFightPredict(int areaX, const Battle::FightData* fightData, bool isPlayer) const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		
		// エリア
		DxLib::DrawBox(areaX, shape_.y, areaX + AREA_WIDTH, shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);


		

		/* 一行目 ここから */

		int x = areaX;
		UnitInfo info = fightData->unit->getInfo();

		// 名前表示
		int nameColorType = isPlayer ? ColorType::PLAYER_COLOR : ColorType::ENEMY_COLOR;
		x = BUI::drawLabel(x, shape_.y, info.name, BUI::getZenW(3), nameColorType, ColorType::WHITE);

		// 特殊効果
		for (auto itr = fightData->extraEffects.begin(); itr != fightData->extraEffects.end(); ++itr)
		{
			x += EXTRA_EFFECT_MARGIN;
			int len = (*itr).size() / 2;
			x = BUI::drawLabel(x, shape_.y, (*itr), BUI::getZenW(len), ColorType::MAIN_COLOR_ON, ColorType::SUB_COLOR);
		}



		/* 二行目 ここから */

		x = areaX;
		int y = shape_.y + BUI_LINE_HEIGHT;

		// HP
		x = BUI::drawLabel(x, y, "HP", BUI::getHanW(2));
		x = BUI::drawValue(x, y, to_string(info.hp) + " / " + to_string(info.hpm), BUI::getHanW(7));

		// 与ダメージ
		string value = !fightData->isAtk ? "-" : to_string(fightData->damage);

		x = BUI::drawLabel(x, y, "ダメージ", BUI::getZenW(4));
		x = BUI::drawValue(x, y, value, BUI::getHanW(3));

		// 命中率
		value = !fightData->isAtk ? "-" : to_string(fightData->hitRate) + "%";
		x = BUI::drawLabel(x, y, "命中率", BUI::getZenW(3));
		x = BUI::drawValue(x, y, value, BUI::getHanW(4));
	}

	/**
	 * @fn
	 * 攻撃方向の描画
	 * @param (y) y座標
	 * @param (isPlayer) プレイヤー側のユニットであるか
	 */
	void FightPredictDisplay::drawAtackDirection(int y, bool isPlayer) const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		float backX = (float)(shape_.x + AREA_WIDTH + CENTER_PADDING + DIRECTION_W); // 矢印の元のx座標
		float forwardX = (float)(shape_.x + AREA_WIDTH + CENTER_PADDING); // 矢印の先のx座標

		float yf = (float)y;

		if (!isPlayer)
		{
			float tmp = backX;
			backX = forwardX;
			forwardX = tmp;
		}

		int color = isPlayer ? rm.getColor(ColorType::PLAYER_COLOR_LITE) : rm.getColor(ColorType::ENEMY_COLOR_LITE);

		DxLib::DrawTriangleAA(backX, yf, forwardX, (float)(yf + shape_.h / 4), backX, (float)(yf + shape_.h / 2), color, TRUE);
	}



	/**
	 * @fn
	 * アニメーション更新
	 */
	bool FightPredictDisplay::animationUpdate()
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
	bool FightPredictDisplay::createAnimation(int animationId)
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
	 * 戦闘予測生成
	 * @return 戦闘予測を新しくセットしたときのみtrueを返す
	 */
	void FightPredictDisplay::emplaceFight(const Battle::Fight* fight)
	{
		fight_ = fight;
		changeAnimation(AnimationKind::DISPLAY);
		shape_.disabledHit = false;
	}

	/**
	 * @fn
	 * リセット
	 */
	void FightPredictDisplay::reset()
	{
		fight_ = nullptr;
		animation_.reset();
		shape_.disabledHit = true;
	}


}