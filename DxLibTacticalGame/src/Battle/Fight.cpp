#include "Fight.h"



void Battle::Fight::init(shared_ptr<Map> map)
{
	map_ = map;
}

/**
 * @fn
 * ν¬\ͺ
 */
void Battle::Fight::setPrepare(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
{
	if (atkUnit && defUnit)
	{
		shared_ptr<Mass> mass = map_->getMass(defUnit->getMassX(), defUnit->getMassY()); // hδ€Μ}X
		isActSideFirst_ = true;

		makeFightData(&actSide_, atkUnit, defUnit, mass, true);  // U€vZ
		makeFightData(&psvSide_, defUnit, atkUnit, mass, false); // hδ€vZ
	}
}

/**
 * @fn
 * FightDataΆ¬
 */
void Battle::Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
{
	fightData->unit = atkUnit;
	fightData->isAtk = isAct;

	if (fightData->isAtk) // U·ικ
	{
		fightData->damage = atkUnit->getAtk() - defUnit->getDef();
		fightData->hitRate = 100;
	}
	else // U΅Θ’κ
	{
		fightData->damage = 0;
		fightData->hitRate = 0;
	}

	
}

/**
 * @fn
 * ν¬Jn
 */
void Battle::Fight::start()
{
	phase_ = Phase::FIRST_ATK;
	atack(isActSideFirst_);
}


/**
 * @fn
 * ν¬ΜXV
 * @return IΉΝtrueπΤ·
 */
bool Battle::Fight::checkUpdate()
{
	if (!psvSide_.unit && !actSide_.unit)
	{
		return true;
	}
	
	// Aj[Vp±
	if (psvSide_.unit->isAnimation() || actSide_.unit->isAnimation())
	{
		return false;
	}

	// Aj[VIΉ
	if (phase_ == Phase::FIRST_ATK)
	{
		if (atack(!isActSideFirst_))
		{
			// hδ€ΜU θ
			phase_ = Phase::SECOND_ATK;
			return false;
		}
	}

	// ν¬IΉ
	phase_ = Phase::NO_FIGHT;
	return true;
}

/**
 * @fn
 * Uΐs
 * @return UΐsL³
 */
bool Battle::Fight::atack(bool isActSideAtack)
{
	FightData& atkSide = actSide_;
	FightData& defSide = psvSide_;

	if (!isActSideAtack) // hδ€ΜU
	{
		atkSide = psvSide_;
		defSide = actSide_;
	}

	if (!atkSide.isAtk || atkSide.unit->getHp() <=0 )
	{
		return false;
	}
	
	atkSide.unit->atack(defSide.unit->getX(), defSide.unit->getY());
	
	if (defSide.unit->damage(atkSide.damage))
	{
		// S
		map_->eraseUnit(defSide.unit);
	}

	return true;
}


/**
 * @fn
 * FightDataζΎ
 */
void Battle::Fight::getFightData(const FightData* fightData, bool isAtk)
{
	if (isAtk)
	{
		fightData = &actSide_;
	}

	fightData = &psvSide_;
}


