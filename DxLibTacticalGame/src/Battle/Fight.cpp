#include "Fight.h"

namespace Battle
{
	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^���擾
	 */
	void Fight::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * �퓬�\��
	 * @param (actUnit) �U�����d�|�������̃��j�b�g
	 * @param (psvUnit) �U�����d�|����ꂽ���̃��j�b�g
	 * @return ���e���X�V���ꂽ�ꍇ��true��Ԃ�
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit)
	{
		return setPrepare(actUnit, actUnit->getMassX(), actUnit->getMassY(), psvUnit);
	}

	/**
	 * @fn
	 * �퓬�\��
	 * @param (actUnit) �U�����d�|�������̃��j�b�g
	 * @param (actMassX) �U�����d�|����}�X���WX
	 * @param (actMassY) �U�����d�|����}�X���WY
	 * @param (psvUnit) �U�����d�|����ꂽ���̃��j�b�g
	 * @return ���e���X�V���ꂽ�ꍇ��true��Ԃ�
	 */
	bool Fight::setPrepare(shared_ptr<Unit> actUnit, int actMassX, int actMassY, shared_ptr<Unit> psvUnit)
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
	 * @param (fightData) ��������FightData�擾�p
	 * @param (actUnit) �U�����d�|�������̃��j�b�g
	 * @param (psvUnit) �U�����d�|����ꂽ���̃��j�b�g
	 * @param (mass) �U�����d�|����ꂽ���̂���}�X
	 * @param (isAct) �U���\��
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		UnitInfo atkInfo = atkUnit->getInfo();
		UnitInfo defInfo = defUnit->getInfo();

		// �}���X�L������
		if (!isAct && atkInfo.ability.kind == Ability::Kind::AMBUSH)
		{
			if (atkInfo.len > defInfo.len)
			{
				fightData->isAtk = true;
				isActSideFirst_ = false;
			}
		}

		if (fightData->isAtk) // �U������ꍇ
		{
			int def = defInfo.def + mass->getDef();

			// �ђʃX�L��
			if (atkInfo.ability.kind == Ability::Kind::THROUGH)
			{
				def = 0;
			}

			fightData->damage = atkInfo.atk - def;
			fightData->hitRate = 100 - mass->getAgl();

			// �ˌ��X�L��
			if (atkInfo.ability.kind == Ability::Kind::RUSH)
			{
				if (isActSideFirst_ && mass->getKind() == Mass::PLAIN)
				{
					fightData->damage += 5;
				}
			}

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
		actSide_.unit->endAction();
		reset();
		return true;
	}

	/**
	 * @fn
	 * �U�����s
	 * @param (isActSideAtack) �h�䑤�̍U���ł��邩
	 * @return �U�����s�L��
	 */
	bool Fight::atack(bool isActSideAtack)
	{
		FightData* atkSide;
		FightData* defSide;

		if (isActSideAtack) // �U���d�|�������̍U��
		{
			atkSide = &actSide_;
			defSide = &psvSide_;
		}
		else // �U���d�|����ꂽ���̍U��
		{
			atkSide = &psvSide_;
			defSide = &actSide_;
		}

		// �U���s����
		if (!atkSide->isAtk || atkSide->unit->getInfo().hp <= 0)
		{
			return false;
		}

		// �U�����s
		atkSide->unit->atack(defSide->unit->getX(), defSide->unit->getY());

		// ��������
		if (atkSide->hitRate > DxLib::GetRand(99))
		{
			// �_���[�W
			if (defSide->unit->damage(atkSide->damage))
			{
				// ���S������
				map_->eraseUnit(defSide->unit);
			}
		}
		else
		{
			defSide->unit->avoid(); // ���
		}

		return true;
	}


	/**
	 * @fn
	 * FightData�擾
	 * @param (isAct) true: �U�����d�|�������Afalse: �U�����d�|����ꂽ��
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
	 * �퓬�\���ς݂ł��邩
	 * @return �\���ς݂̏ꍇtrue��Ԃ�
	 */
	bool Fight::isPrepared() const
	{
		return actSide_.unit != nullptr;
	}
}

