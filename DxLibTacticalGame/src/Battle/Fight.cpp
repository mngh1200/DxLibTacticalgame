#include "Fight.h"
#include "Screen/BattleScreen.h"

namespace Battle
{
	/**
	 * @fn
	 * 初期処理
	 * @param (map) マップのポインタを取得
	 */
	void Fight::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
 * @fn
	 * 攻撃された方向を取得
	 * @param (defUnit) 防御側ユニット
	 * @param (atkUnit) 攻撃側ユニット
	 * @return 方向
	 */
	int getAtkDirection(shared_ptr<Unit> defUnit, int atkUnitX, int atkUnitY)
	{
		int atkDirection = Direction::NONE_DIRECTION; // 攻撃された方向

		int defUnitX = defUnit->getMassX();
		int defUnitY = defUnit->getMassY();

		if (atkUnitX == defUnitX) // 上下のどちらか
		{
			if (atkUnitY > defUnitY)
			{
				atkDirection = Direction::BOTTOM;
			}
			else if (atkUnitY < defUnitY)
			{
				atkDirection = Direction::TOP;
			}
		}
		else if (atkUnitY == defUnitY)
		{
			if (atkUnitX > defUnitX)
			{
				atkDirection = Direction::LEFT;
			}
			else if (atkUnitX < defUnitX)
			{
				atkDirection = Direction::RIGHT;
			}
		}
		return atkDirection;
	}

	/**
	 * @fn
	 * 連携、挟撃判定
	 * @param (atkDirection) 攻撃された方向
	 * @param (atkedLogs) 攻撃された方向の履歴
	 * @return 連携、挟撃判定結果
	 */
	int Fight::getCoordinatedAttack(int atkDirection, int atkedLogs)
	{
		int result = CoordinatedAttack::NONE;

		if (atkDirection == Direction::TOP || atkDirection == Direction::BOTTOM)
		{
			int tmpLog = atkedLogs & (Direction::TOP | Direction::BOTTOM); // 反対方向の攻撃方向履歴（仮）
			if (tmpLog != 0 && tmpLog != atkDirection) // 挟撃判定
			{
				return CoordinatedAttack::PINCHING;
			}
			else if (atkedLogs & (Direction::LEFT | Direction::RIGHT)) // 連携判定
			{
				return CoordinatedAttack::COORDINATED;
			}
		}
		else if (atkDirection == Direction::LEFT || atkDirection == Direction::RIGHT)
		{
			int tmpLog = atkedLogs & (Direction::LEFT | Direction::RIGHT); // 反対方向の攻撃方向履歴（仮）
			if (tmpLog != 0 && tmpLog != atkDirection) // 挟撃判定
			{
				return CoordinatedAttack::PINCHING;
			}
			else if (atkedLogs & (Direction::TOP | Direction::BOTTOM)) // 連携判定
			{
				return CoordinatedAttack::COORDINATED;
			}
		}

		return result;
	}

	/**
	 * @fn
	 * 戦闘予測
	 * @param (actUnit) 攻撃を仕掛けた側のユニット
	 * @param (psvUnit) 攻撃を仕掛けられた側のユニット
	 * @return 内容が更新された場合はtrueを返す
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit)
	{
		return setPrepare(actUnit, actUnit->getMassX(), actUnit->getMassY(), psvUnit);
	}

	/**
	 * @fn
	 * 戦闘予測
	 * @param (actUnit) 攻撃を仕掛けた側のユニット
	 * @param (actMassX) 攻撃を仕掛けるマス座標X
	 * @param (actMassY) 攻撃を仕掛けるマス座標Y
	 * @param (psvUnit) 攻撃を仕掛けられた側のユニット
	 * @return 内容が更新された場合はtrueを返す
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, int actMassX, int actMassY, shared_ptr<Unit> psvUnit)
	{
		if (actUnit && psvUnit)
		{
			if (actUnit == actSide_.unit && psvSide_.unit == psvUnit)
			{
				return false; // 対象が変わっていない場合
			}

			// 元の予測ダメージ表示を非表示
			hidePredictUnitDamage();

			// 特殊効果一覧クリア
			actSide_.extraEffects.clear();
			psvSide_.extraEffects.clear();

			// shared_ptr<Mass> mass = map_->getMass(psvUnit->getMassX(), psvUnit->getMassY()); // 防御側のマス
			actSide_.isFirst = true;

			UnitInfo actInfo = actUnit->getInfo();
			// UnitInfo psvInfo = psvUnit->getInfo();

			// 攻撃方向
			actSide_.direction = getAtkDirection(psvUnit, actMassX, actMassY);

			// 連携、挟撃判定
			int coordinatedAttack = CoordinatedAttack::NONE;
			if (actInfo.isCloseAtack)
			{
				coordinatedAttack = getCoordinatedAttack(actSide_.direction, psvUnit->getCloseAttackLogs());
			}

			makeFightData(&psvSide_, psvUnit, actUnit, false, coordinatedAttack); // 防御側計算 (迎撃判定があるため先に処理)

			if (psvSide_.isFirst)
			{
				actSide_.isFirst = false;
			}

			makeFightData(&actSide_, actUnit, psvUnit, true, coordinatedAttack);  // 攻撃側計算

			// 連携、挟撃のダメージ計算
			if (coordinatedAttack == CoordinatedAttack::COORDINATED) // 連携 (ダメージ倍率 1.5)
			{
				actSide_.damage = actSide_.damage * 3 / 2;
				actSide_.extraEffects.push_back("連携");
			}
			else if (coordinatedAttack == CoordinatedAttack::PINCHING) // 挟撃 (ダメージ倍率 2)
			{
				actSide_.damage = actSide_.damage * 2;
				actSide_.extraEffects.push_back("挟撃");
			}
			

			return true;
		}
		return false;
	}


	/**
	 * @fn
	 * リセット
	 */
	void Fight::reset()
	{
		hidePredictUnitDamage();
		actSide_ = { nullptr, true };
		psvSide_ = { nullptr, false };
		phase_ = Phase::NO_FIGHT;
	}

	/**
	 * @fn
	 * 攻撃の命中状況をセット
	 * @param (actHitState) 攻撃しかけた側の命中状況
	 * @param (psvHitState) 攻撃された側の命中状況
	 */
	void Fight::setHitState(int actHitState, int psvHitState)
	{
		if (actHitState != FightData::HitState::UNSETTLED)
		{
			actSide_.hitState = actHitState;
		}

		if (psvHitState != FightData::HitState::UNSETTLED)
		{
			psvSide_.hitState = psvHitState;
		}
	}

	/**
	 * @fn
	 * FightData生成
	 * @param (fightData) 生成したFightData取得用
	 * @param (actUnit) 攻撃を仕掛けた側のユニット
	 * @param (psvUnit) 攻撃を仕掛けられた側のユニット
	 * @param (mass) 攻撃を仕掛けられた側のいるマス
	 * @param (isAct) 攻撃可能か
	 * @param (coordinatedAttack) 連携、挟撃判定結果
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, bool isAct, int coordinatedAttack)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		UnitInfo atkInfo = atkUnit->getInfo();
		UnitInfo defInfo = defUnit->getInfo();

		shared_ptr<Mass> mass = map_->getMass(defUnit->getMassX(), defUnit->getMassY());

		// 迎撃スキル判定
		if (!isAct && atkInfo.ability.kind == Ability::Kind::AMBUSH && coordinatedAttack == CoordinatedAttack::NONE)
		{
			if (atkInfo.len > defInfo.len)
			{
				fightData->extraEffects.push_back(atkInfo.ability.getLabel());
				fightData->isAtk = true;
				fightData->isFirst = true;
			}
		}

		if (fightData->isAtk) // 攻撃する場合
		{
			// 近接攻撃判定
			if (atkInfo.isCloseAtack)
			{
				fightData->isCloseAttack = true;
				fightData->extraEffects.push_back("近接");
			}
			else
			{
				fightData->isCloseAttack = false;
				fightData->extraEffects.push_back("射撃");
			}

			// 以下、ダメージ計算
			int def = defInfo.def + mass->getDef();

			// 貫通スキル
			if (atkInfo.ability.kind == Ability::Kind::THROUGH)
			{
				def = 0;
				fightData->extraEffects.push_back(atkInfo.ability.getLabel());
			}

			fightData->damage = atkInfo.atk - def;
			fightData->hitRate = 100 - mass->getAgl();

			// 距離減衰
			int distance = Map::getMassDistance(atkUnit->getMassX(), atkUnit->getMassY(), defUnit->getMassX(), defUnit->getMassY());
			fightData->hitRate -= (distance - 1) * Gunner::ATTENUATION;

			if (fightData->hitRate < 0)
			{
				fightData->hitRate = 0;
			}
			else if (fightData->hitRate > 100)
			{
				fightData->hitRate = 100;
			}

			// 突撃スキル
			if (isAct && isActSideFirst())
			{
				if (actSide_.unit->getInfo().ability.kind == Ability::Kind::RUSH)
				{
					if (isActSideFirst() && mass->getKind() == Mass::PLAIN)
					{
						actSide_.extraEffects.push_back(actSide_.unit->getInfo().ability.getLabel());
						actSide_.damage += 5;
					}
				}
			}
		}
		else // 攻撃しない場合
		{
			fightData->damage = 0;
			fightData->hitRate = 0;
		}

		// 命中判定
		if (fightData->hitRate > DxLib::GetRand(99))
		{
			fightData->hitState = FightData::HitState::HITTED;
		}
		else
		{
			fightData->hitState = FightData::HitState::MISS;
		}
		
	}

	/**
	 * @fn
	 * 戦闘開始
	 */
	void Fight::start()
	{
		if (isPrepared())
		{
			phase_ = Phase::FIRST_EFFECT;

			FightData atkSide;
			FightData defSide;
			getAttackState(isActSideFirst(), atkSide, defSide);
			showExtraEffect(&atkSide, &defSide);

			// atack(isActSideFirst());
		}
	}


	/**
	 * @fn
	 * 戦闘時の更新処理
	 * @return 終了時はtrueを返す
	 */
	bool Fight::checkUpdate()
	{
		if (!psvSide_.unit && !actSide_.unit) // 対象ユニットが存在しない場合
		{
			return true;
		}

		
		if (phase_ == Phase::FIRST_ATK || phase_ == Phase::SECOND_ATK) // ユニットアニメーション中の処理
		{
			// アニメーション継続
			if (psvSide_.unit->isAnimation() || actSide_.unit->isAnimation())
			{
				return false;
			}
			
			// アニメーション終了時
			if (phase_ == Phase::FIRST_ATK)
			{
				FightData atkSide;
				FightData defSide;
				if (getAttackState(!isActSideFirst(), atkSide, defSide)) // 攻撃された側の攻撃があるか
				{
					showExtraEffect(&atkSide, &defSide);
					phase_ = Phase::SECOND_EFFECT;
					return false;
				}				
			}
		}
		if (phase_ == Phase::FIRST_EFFECT || phase_ == Phase::SECOND_EFFECT) // 特殊効果エフェクト中の処理
		{
			// アニメーション中の特殊エフェクトがあるか
			for (auto itr = extraEffectList_.begin(); itr != extraEffectList_.end(); ++itr)
			{
				if ((*itr)->isAnimation())
				{
					return false;
				}
			}

			// 特殊エフェクト終了時
			FightData atkSide, defSide;
			bool isFirstAttack = true; //! 攻撃を仕掛けた側の攻撃

			if (phase_ == Phase::SECOND_EFFECT)
			{
				isFirstAttack = false;
			}


			if (getAttackState(isActSideFirst() == isFirstAttack, atkSide, defSide)) // 攻撃可能か
			{
				attack(&atkSide, &defSide);

				if (isFirstAttack)
				{
					phase_ = Phase::FIRST_ATK;
				}
				else
				{
					phase_ = Phase::SECOND_ATK;
				}

				return false;
			}
		}

		// 特殊効果エフェクトを非表示化
		for (auto itr = extraEffectList_.begin(); itr != extraEffectList_.end(); ++itr)
		{
			(*itr)->hide();
		}

		// 戦闘終了
		actSide_.unit->endAction();

		// reset();
		return true;
	}

	/**
	 * @fn
	 * ユニットの予測ダメージ量を表示
	 */
	void Fight::showPredictUnitDamage()
	{
		if (actSide_.unit)
		{
			actSide_.unit->setPredictDamage(psvSide_.damage);
		}
		if (psvSide_.unit)
		{
			psvSide_.unit->setPredictDamage(actSide_.damage);
		}
	}

	/**
	 * @fn
	 * ユニットの予測ダメージ量を非表示
	 */
	void Fight::hidePredictUnitDamage()
	{
		if (actSide_.unit)
		{
			actSide_.unit->clearPredictDamage();
		}
		if (psvSide_.unit)
		{
			psvSide_.unit->clearPredictDamage();
		}
	}

	/**
	 * @fn
	 * 攻撃実行
	 * @param (isActSideAtack) 防御側の攻撃であるか
	 * @return 攻撃実行有無
	 */
	void Fight::attack(FightData* atkSide, FightData* defSide)
	{
		// 攻撃実行
		atkSide->unit->atack(defSide->unit->getX(), defSide->unit->getY());

		// 命中判定 (hitState == HITTEDの場合は確定命中、hitState == MISSの場合は確定ミス)
		if (atkSide->hitState != FightData::HitState::MISS)
		{
			// atkSide->hitState = FightData::HitState::HITTED;

			// ダメージ
			if (defSide->unit->damage(atkSide->damage, atkSide->direction, atkSide->isCloseAttack))
			{
				// 死亡時処理
				map_->eraseUnit(defSide->unit);
			}
		}
		else
		{
			//atkSide->hitState = FightData::HitState::MISS;
			defSide->unit->avoid(); // 回避
		}
	}

	/**
	 * @fn
	 * 攻撃の状況取得
	 */
	bool Fight::getAttackState(bool isActSideAtack, FightData& atkSide, FightData& defSide)
	{
		if (isActSideAtack) // 攻撃仕掛けた側の攻撃
		{
			atkSide = actSide_;
			defSide = psvSide_;
		}
		else // 攻撃仕掛けられた側の攻撃
		{
			atkSide = psvSide_;
			defSide = actSide_;
		}

		// 攻撃不可判定
		if (!atkSide.isAtk || atkSide.unit->getInfo().hp <= 0)
		{
			return false;
		}

		return true;
	}

	/**
	 * @fn
	 * 攻撃実行
	 * @param (isActSideAtack) 防御側の攻撃であるか
	 * @return 攻撃実行有無
	 */
	void Fight::showExtraEffect(FightData* atkSide, FightData* defSide)
	{
		// 特殊効果表示
		clearExtraEffect();
		int num = 0;
		for (auto itr = atkSide->extraEffects.begin(); itr != atkSide->extraEffects.end(); ++itr)
		{
			if ("近接" == (*itr) || "射撃" == (*itr))
			{
				continue; // 特殊効果でないものは無視
			}

			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			shared_ptr<ExtraEffect> extraEffect = make_shared<ExtraEffect>();
			objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, extraEffect);
			extraEffect->initExtraEffect(atkSide->unit, defSide->unit, (*itr).c_str(), num);
			extraEffectList_.push_back(extraEffect);
			++num;
		}
	}

	/**
	 * @fn
	 * 既存の特殊効果表記クリア
	 */
	void Fight::clearExtraEffect()
	{
		for (auto itr = extraEffectList_.begin(); itr != extraEffectList_.end(); ++itr)
		{
			(*itr)->destroy();
		}
		extraEffectList_.clear();
	}


	/**
	 * @fn
	 * FightData取得
	 * @param (isAct) true: 攻撃を仕掛けた側、false: 攻撃を仕掛けられた側
	 * @return FightData
	 */
	const FightData& Fight::getFightData(bool isAct) const
	{
		if (isAct)
		{
			return actSide_;
		}

		return psvSide_;
	}

	/**
	 * @fn
	 * 戦闘予測済みであるか
	 * @return 予測済みの場合trueを返す
	 */
	bool Fight::isPrepared() const
	{
		return actSide_.unit != nullptr;
	}
}

