#include "Fight.h"



void Battle::Fight::init(shared_ptr<Map> map)
{
	map_ = map;
}

/**
 * @fn
 * í“¬—\‘ª
 */
void Battle::Fight::setPrepare(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
{
	if (atkUnit && defUnit)
	{
		shared_ptr<Mass> mass = map_->getMass(defUnit->getMassX(), defUnit->getMassY()); // –hŒä‘¤‚Ìƒ}ƒX
		isActSideFirst_ = true;

		makeFightData(&actSide_, atkUnit, defUnit, mass, true);  // UŒ‚‘¤ŒvZ
		makeFightData(&psvSide_, defUnit, atkUnit, mass, false); // –hŒä‘¤ŒvZ
	}
}

/**
 * @fn
 * FightData¶¬
 */
void Battle::Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
{
	fightData->unit = atkUnit;
	fightData->isAtk = isAct;

	if (fightData->isAtk) // UŒ‚‚·‚éê‡
	{
		fightData->damage = atkUnit->getAtk() - defUnit->getDef();
		fightData->hitRate = 100;
	}
	else // UŒ‚‚µ‚È‚¢ê‡
	{
		fightData->damage = 0;
		fightData->hitRate = 0;
	}

	
}

/**
 * @fn
 * í“¬ŠJn
 */
void Battle::Fight::start()
{
	phase_ = Phase::FIRST_ATK;
	atack(isActSideFirst_);
}


/**
 * @fn
 * í“¬‚ÌXVˆ—
 * @return I—¹‚Ítrue‚ğ•Ô‚·
 */
bool Battle::Fight::checkUpdate()
{
	if (!psvSide_.unit && !actSide_.unit)
	{
		return true;
	}
	
	// ƒAƒjƒ[ƒVƒ‡ƒ“Œp‘±
	if (psvSide_.unit->isAnimation() || actSide_.unit->isAnimation())
	{
		return false;
	}

	// ƒAƒjƒ[ƒVƒ‡ƒ“I—¹
	if (phase_ == Phase::FIRST_ATK)
	{
		if (atack(!isActSideFirst_))
		{
			// –hŒä‘¤‚ÌUŒ‚‚ ‚è
			phase_ = Phase::SECOND_ATK;
			return false;
		}
	}

	// í“¬I—¹
	phase_ = Phase::NO_FIGHT;
	return true;
}

/**
 * @fn
 * UŒ‚Às
 * @return UŒ‚Às—L–³
 */
bool Battle::Fight::atack(bool isActSideAtack)
{
	FightData& atkSide = actSide_;
	FightData& defSide = psvSide_;

	if (!isActSideAtack) // –hŒä‘¤‚ÌUŒ‚
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
		// €–Sˆ—
		map_->eraseUnit(defSide.unit);
	}

	return true;
}


/**
 * @fn
 * FightDataæ“¾
 */
void Battle::Fight::getFightData(const FightData* fightData, bool isAtk)
{
	if (isAtk)
	{
		fightData = &actSide_;
	}

	fightData = &psvSide_;
}


