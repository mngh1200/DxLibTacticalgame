#include "Fight.h"

namespace Battle
{

	void Fight::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * �퓬�\��
	 * @return ���e���X�V���ꂽ�ꍇ��true��Ԃ�
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit)
	{
		if (actUnit && psvUnit)
		{
			if (actUnit == actSide_.unit && psvSide_.unit == psvUnit)
			{
				return false; // �Ώۂ��ς���Ă��Ȃ��ꍇ
			}

			shared_ptr<Mass> mass = map_->getMass(psvUnit->getMassX(), psvUnit->getMassY()); // �h�䑤�̃}�X
			isActSideFirst_ = true;

			makeFightData(&actSide_, actUnit, psvUnit, mass, true);  // �U�����v�Z
			makeFightData(&psvSide_, psvUnit, actUnit, mass, false); // �h�䑤�v�Z
			return true;
		}
		return false;
	}


	/**
	 * @fn
	 * ���Z�b�g
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
	 * FightData����
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		if (fightData->isAtk) // �U������ꍇ
		{
			fightData->damage = atkUnit->getAtk() - defUnit->getDef() - mass->getDef();
			fightData->hitRate = 100 - mass->getAgl();

			// ��������
			int distance = Map::getMassDistance(atkUnit->getMassX(), atkUnit->getMassY(), defUnit->getMassX(), defUnit->getMassY());
			fightData->hitRate -= (distance - 1) * 20;

			if (fightData->hitRate < 0)
			{
				fightData->hitRate = 0;
			}
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
	 * �퓬���̍X�V����
	 * @return �I������true��Ԃ�
	 */
	bool Fight::checkUpdate()
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
		reset();
		return true;
	}

	/**
	 * @fn
	 * �U�����s
	 * @return �U�����s�L��
	 */
	bool Fight::atack(bool isActSideAtack)
	{
		FightData& atkSide = actSide_;
		FightData& defSide = psvSide_;

		if (!isActSideAtack) // �h�䑤�̍U��
		{
			atkSide = psvSide_;
			defSide = actSide_;
		}

		// �U���s����
		if (!atkSide.isAtk || atkSide.unit->getHp() <= 0)
		{
			return false;
		}

		// �U�����s
		atkSide.unit->atack(defSide.unit->getX(), defSide.unit->getY());

		// ��������
		if (atkSide.hitRate > DxLib::GetRand(99))
		{
			// �_���[�W
			if (defSide.unit->damage(atkSide.damage))
			{
				// ���S������
				map_->eraseUnit(defSide.unit);
			}
		}
		else
		{
			defSide.unit->avoid(); // ���
		}

		return true;
	}


	/**
	 * @fn
	 * FightData�擾
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
	 * �퓬�\���ς݂ł��邩
	 * @return �\���ς݂̏ꍇtrue��Ԃ�
	 */
	bool Fight::isPrepared() const
	{
		return actSide_.unit != nullptr;
	}
}

