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
	 * �U�����ꂽ�������擾
	 * @param (defUnit) �h�䑤���j�b�g
	 * @param (atkUnit) �U�������j�b�g
	 * @return ����
	 */
	int getAtkDirection(shared_ptr<Unit> defUnit, int atkUnitX, int atkUnitY)
	{
		int atkDirection = Direction::NONE_DIRECTION; // �U�����ꂽ����

		int defUnitX = defUnit->getMassX();
		int defUnitY = defUnit->getMassY();

		if (atkUnitX == defUnitX) // �㉺�̂ǂ��炩
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
	 * �A�g�A��������
	 * @param (atkDirection) �U�����ꂽ����
	 * @param (atkedLogs) �U�����ꂽ�����̗���
	 * @return �A�g�A�������茋��
	 */
	int Fight::getCoordinatedAttack(int atkDirection, int atkedLogs)
	{
		int result = CoordinatedAttack::NONE;

		if (atkDirection == Direction::TOP || atkDirection == Direction::BOTTOM)
		{
			int tmpLog = atkedLogs & (Direction::TOP | Direction::BOTTOM); // ���Ε����̍U�����������i���j
			if (tmpLog != 0 && tmpLog != atkDirection) // ��������
			{
				return CoordinatedAttack::PINCHING;
			}
			else if (atkedLogs & (Direction::LEFT | Direction::RIGHT)) // �A�g����
			{
				return CoordinatedAttack::COORDINATED;
			}
		}
		else if (atkDirection == Direction::LEFT || atkDirection == Direction::RIGHT)
		{
			int tmpLog = atkedLogs & (Direction::LEFT | Direction::RIGHT); // ���Ε����̍U�����������i���j
			if (tmpLog != 0 && tmpLog != atkDirection) // ��������
			{
				return CoordinatedAttack::PINCHING;
			}
			else if (atkedLogs & (Direction::TOP | Direction::BOTTOM)) // �A�g����
			{
				return CoordinatedAttack::COORDINATED;
			}
		}

		return result;
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

			// ������ʈꗗ�N���A
			actSide_.extraEffects.clear();
			psvSide_.extraEffects.clear();

			shared_ptr<Mass> mass = map_->getMass(psvUnit->getMassX(), psvUnit->getMassY()); // �h�䑤�̃}�X
			actSide_.isFirst = true;

			UnitInfo actInfo = actUnit->getInfo();
			// UnitInfo psvInfo = psvUnit->getInfo();

			// �U������
			actSide_.direction = getAtkDirection(psvUnit, actMassX, actMassY);

			// �A�g�A��������
			int coordinatedAttack = CoordinatedAttack::NONE;
			if (actInfo.isCloseAtack)
			{
				coordinatedAttack = getCoordinatedAttack(actSide_.direction, psvUnit->getCloseAttackLogs());
			}

			makeFightData(&psvSide_, psvUnit, actUnit, mass, false, coordinatedAttack); // �h�䑤�v�Z (�}�����肪���邽�ߐ�ɏ���)

			if (psvSide_.isFirst)
			{
				actSide_.isFirst = false;
			}

			makeFightData(&actSide_, actUnit, psvUnit, mass, true, coordinatedAttack);  // �U�����v�Z

			// �A�g�A�����̃_���[�W�v�Z
			if (coordinatedAttack == CoordinatedAttack::COORDINATED) // �A�g (�_���[�W�{�� 1.5)
			{
				actSide_.damage = actSide_.damage * 3 / 2;
				actSide_.extraEffects.push_back("�A�g");
			}
			else if (coordinatedAttack == CoordinatedAttack::PINCHING) // ���� (�_���[�W�{�� 2)
			{
				actSide_.damage = actSide_.damage * 2;
				actSide_.extraEffects.push_back("����");
			}
			

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
		actSide_ = { nullptr, true };
		psvSide_ = { nullptr, false };
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
	 * @param (coordinatedAttack) �A�g�A�������茋��
	 */
	void Fight::makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct, int coordinatedAttack)
	{
		fightData->unit = atkUnit;
		fightData->isAtk = isAct;

		UnitInfo atkInfo = atkUnit->getInfo();
		UnitInfo defInfo = defUnit->getInfo();

		// �}���X�L������
		if (!isAct && atkInfo.ability.kind == Ability::Kind::AMBUSH && coordinatedAttack == CoordinatedAttack::NONE)
		{
			if (atkInfo.len > defInfo.len)
			{
				fightData->extraEffects.push_back(atkInfo.ability.getLabel());
				fightData->isAtk = true;
				fightData->isFirst = true;
			}
		}

		if (fightData->isAtk) // �U������ꍇ
		{
			// �ߐڍU������
			if (atkInfo.isCloseAtack)
			{
				fightData->isCloseAttack = true;
				fightData->extraEffects.push_back("�ߐ�");
			}
			else
			{
				fightData->isCloseAttack = false;
				fightData->extraEffects.push_back("�ˌ�");
			}

			// �ȉ��A�_���[�W�v�Z
			int def = defInfo.def + mass->getDef();

			// �ђʃX�L��
			if (atkInfo.ability.kind == Ability::Kind::THROUGH)
			{
				def = 0;
				fightData->extraEffects.push_back(atkInfo.ability.getLabel());
			}

			fightData->damage = atkInfo.atk - def;
			fightData->hitRate = 100 - mass->getAgl();

			// ��������
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

			// �ˌ��X�L��
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
			atack(isActSideFirst());
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
			if (atack(!isActSideFirst()))
			{
				// �h�䑤�̍U������
				phase_ = Phase::SECOND_ATK;
				return false;
			}
		}

		// �퓬�I��
		actSide_.unit->endAction();
		// reset();
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
			if (defSide->unit->damage(atkSide->damage, atkSide->direction, atkSide->isCloseAttack))
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

