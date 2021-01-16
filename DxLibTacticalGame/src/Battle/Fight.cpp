#include "Fight.h"

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

			shared_ptr<Mass> mass = map_->getMass(psvUnit->getMassX(), psvUnit->getMassY()); // 防御側のマス
			isActSideFirst_ = true;

			makeFightData(&actSide_, actUnit, psvUnit, mass, true);  // 攻撃側計算
			makeFightData(&psvSide_, psvUnit, actUnit, mass, false); // 防御側計算
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
		actSide_ = { nullptr, 0, 0, true };
		psvSide_ = { nullptr, 0, 0, false };
		isActSideFirst_ = true;
		phase_ = Phase::NO_FIGHT;
	}

	/**
	 * @fn
	 * FightData生成
	 * @param (fightData) 生成したFightData取得用
	 * @param (actUnit) 攻撃を仕掛けた側のユニット
	 * @param (psvUnit) 攻撃を仕掛けられた側のユニット
	 * @param (mass) 攻撃を仕掛けられた側のいるマス
	 * @param (isAct) 攻撃可能か
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		UnitInfo atkInfo = atkUnit->getInfo();
		UnitInfo defInfo = defUnit->getInfo();

		// 迎撃スキル判定
		if (!isAct && atkInfo.ability.kind == Ability::Kind::AMBUSH)
		{
			if (atkInfo.len > defInfo.len)
			{
				fightData->isAtk = true;
				isActSideFirst_ = false;
			}
		}

		if (fightData->isAtk) // 攻撃する場合
		{
			int def = defInfo.def + mass->getDef();

			// 貫通スキル
			if (atkInfo.ability.kind == Ability::Kind::THROUGH)
			{
				def = 0;
			}

			fightData->damage = atkInfo.atk - def;
			fightData->hitRate = 100 - mass->getAgl();

			// 突撃スキル
			if (atkInfo.ability.kind == Ability::Kind::RUSH)
			{
				if (isActSideFirst_ && mass->getKind() == Mass::PLAIN)
				{
					fightData->damage += 5;
				}
			}

			// 距離減衰
			int distance = Map::getMassDistance(atkUnit->getMassX(), atkUnit->getMassY(), defUnit->getMassX(), defUnit->getMassY());
			fightData->hitRate -= (distance - 1) * 20;

			if (fightData->hitRate < 0)
			{
				fightData->hitRate = 0;
			}
		}
		else // 攻撃しない場合
		{
			fightData->damage = 0;
			fightData->hitRate = 0;
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
			phase_ = Phase::FIRST_ATK;
			atack(isActSideFirst_);
		}
	}


	/**
	 * @fn
	 * 戦闘時の更新処理
	 * @return 終了時はtrueを返す
	 */
	bool Fight::checkUpdate()
	{
		if (!psvSide_.unit && !actSide_.unit)
		{
			return true;
		}

		// アニメーション継続
		if (psvSide_.unit->isAnimation() || actSide_.unit->isAnimation())
		{
			return false;
		}

		// アニメーション終了時
		if (phase_ == Phase::FIRST_ATK)
		{
			if (atack(!isActSideFirst_))
			{
				// 防御側の攻撃あり
				phase_ = Phase::SECOND_ATK;
				return false;
			}
		}

		// 戦闘終了
		actSide_.unit->endAction();
		reset();
		return true;
	}

	/**
	 * @fn
	 * 攻撃実行
	 * @param (isActSideAtack) 防御側の攻撃であるか
	 * @return 攻撃実行有無
	 */
	bool Fight::atack(bool isActSideAtack)
	{
		FightData* atkSide;
		FightData* defSide;

		if (isActSideAtack) // 攻撃仕掛けた側の攻撃
		{
			atkSide = &actSide_;
			defSide = &psvSide_;
		}
		else // 攻撃仕掛けられた側の攻撃
		{
			atkSide = &psvSide_;
			defSide = &actSide_;
		}

		// 攻撃不可判定
		if (!atkSide->isAtk || atkSide->unit->getInfo().hp <= 0)
		{
			return false;
		}

		// 攻撃実行
		atkSide->unit->atack(defSide->unit->getX(), defSide->unit->getY());

		// 命中判定
		if (atkSide->hitRate > DxLib::GetRand(99))
		{
			// ダメージ
			if (defSide->unit->damage(atkSide->damage))
			{
				// 死亡時処理
				map_->eraseUnit(defSide->unit);
			}
		}
		else
		{
			defSide->unit->avoid(); // 回避
		}

		return true;
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

