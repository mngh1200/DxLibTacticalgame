#include "Fight.h"



void Battle::Fight::init(shared_ptr<Map> map)
{
	map_ = map;
}

/**
 * @fn
 * �퓬�\��
 */
void Battle::Fight::setPrepare(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
{
	if (atkUnit && defUnit)
	{
		shared_ptr<Mass> mass = map_->getMass(defUnit->getMassX(), defUnit->getMassY()); // �h�䑤�̃}�X
		isActSideFirst_ = true;

		makeFightData(&actSide_, atkUnit, defUnit, mass, true);  // �U�����v�Z
		makeFightData(&psvSide_, defUnit, atkUnit, mass, false); // �h�䑤�v�Z
	}
}

/**
 * @fn
 * FightData����
 */
void Battle::Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
{
	fightData->unit = atkUnit;
	fightData->isAtk = isAct;

	if (fightData->isAtk) // �U������ꍇ
	{
		fightData->damage = atkUnit->getAtk() - defUnit->getDef();
		fightData->hitRate = 100;
	}
	else // �U�����Ȃ��ꍇ
	{
		fightData->damage = 0;
		fightData->hitRate = 0;
	}

	
}

/**
 * @fn
 * �퓬�J�n
 */
void Battle::Fight::start()
{
	phase_ = Phase::FIRST_ATK;
	atack(isActSideFirst_);
}


/**
 * @fn
 * �퓬���̍X�V����
 * @return �I������true��Ԃ�
 */
bool Battle::Fight::checkUpdate()
{
	if (!psvSide_.unit && !actSide_.unit)
	{
		return true;
	}
	
	// �A�j���[�V�����p��
	if (psvSide_.unit->isAnimation() || actSide_.unit->isAnimation())
	{
		return false;
	}

	// �A�j���[�V�����I����
	if (phase_ == Phase::FIRST_ATK)
	{
		if (atack(!isActSideFirst_))
		{
			// �h�䑤�̍U������
			phase_ = Phase::SECOND_ATK;
			return false;
		}
	}

	// �퓬�I��
	phase_ = Phase::NO_FIGHT;
	return true;
}

/**
 * @fn
 * �U�����s
 * @return �U�����s�L��
 */
bool Battle::Fight::atack(bool isActSideAtack)
{
	FightData& atkSide = actSide_;
	FightData& defSide = psvSide_;

	if (!isActSideAtack) // �h�䑤�̍U��
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
		// ���S������
		map_->eraseUnit(defSide.unit);
	}

	return true;
}


/**
 * @fn
 * FightData�擾
 */
void Battle::Fight::getFightData(const FightData* fightData, bool isAtk)
{
	if (isAtk)
	{
		fightData = &actSide_;
	}

	fightData = &psvSide_;
}


