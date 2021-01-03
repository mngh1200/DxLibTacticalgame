#include "Fight.h"

namespace Battle
{

	void Fight::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * 戦闘予測
	 * @return 内容が更新された場合はtrueを返す
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit)
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
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		if (fightData->isAtk) // 攻撃する場合
		{
			fightData->damage = atkUnit->getAtk() - defUnit->getDef() - mass->getDef();
			fightData->hitRate = 100 - mass->getAgl();

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
		reset();
		return true;
	}

	/**
	 * @fn
	 * 攻撃実行
	 * @return 攻撃実行有無
	 */
	bool Fight::atack(bool isActSideAtack)
	{
		FightData& atkSide = actSide_;
		FightData& defSide = psvSide_;

		if (!isActSideAtack) // 防御側の攻撃
		{
			atkSide = psvSide_;
			defSide = actSide_;
		}

		// 攻撃不可判定
		if (!atkSide.isAtk || atkSide.unit->getHp() <= 0)
		{
			return false;
		}

		// 攻撃実行
		atkSide.unit->atack(defSide.unit->getX(), defSide.unit->getY());

		// 命中判定
		if (atkSide.hitRate > DxLib::GetRand(99))
		{
			// ダメージ
			if (defSide.unit->damage(atkSide.damage))
			{
				// 死亡時処理
				map_->eraseUnit(defSide.unit);
			}
		}
		else
		{
			defSide.unit->avoid(); // 回避
		}

		return true;
	}


	/**
	 * @fn
	 * FightData取得
	 */
	FightData Fight::getFightData(bool isAtk) const
	{
		if (isAtk)
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

