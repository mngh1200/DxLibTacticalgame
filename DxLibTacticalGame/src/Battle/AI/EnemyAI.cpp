#include "EnemyAI.h"

namespace Battle {

	/**
	 * @fn
	 * ����菇��Ԃ�
	 * @param (map) map�|�C���^
	 */
	bool EnemyAI::createOrders(shared_ptr<Map> map)
	{
		shared_ptr<Unit> unit = getNextUnit(map);

		if (!unit)
		{
			return false; // ������̃��j�b�g���Ȃ��ꍇ�͏I��
		}

		// �ړ��\�͈͎擾
		vector<pair<int, int>> movPosList;
		map->getMovableMasses(unit, unit->getMassX(), unit->getMassY(), unit->getInfo().mov, movPosList);

		int maxPoint = 0; // �ړ��Ώۃ}�X�̓��_ 
		pair<int, int> targetPos = make_pair(unit->getMassX(), unit->getMassY()); // �ړ��Ώۃ}�X
		shared_ptr<Unit> targetUnit; // �U���Ώۃ��j�b�g

		for (auto itr = movPosList.begin(); itr != movPosList.end(); ++itr)
		{
			// �Ώۃ}�X�̓��_�v�Z
			int point = 0;

			// �퓬�\������̃|�C���g�Z�o
			shared_ptr<Unit> tmpTargetUnit;
			point += getMassPoint(map, unit, itr->first, itr->second, tmpTargetUnit);

			// ���_��������Έړ��Ώۃ}�X�Ɏw��
			if (point > maxPoint)
			{
				targetPos = *itr;
				maxPoint = point;
				targetUnit = tmpTargetUnit;
			}
		}

		// �ړ�����
		orders_.push_back(Order{Order::Kind::MOVE, unit, targetPos.first, targetPos.second});

		if (targetUnit)
		{
			// �U������
			orders_.push_back(Order{ Order::Kind::ATACK, unit, targetUnit->getMassX(), targetUnit->getMassY()});
		}
		else
		{
			// �ҋ@����
			orders_.push_back(Order{ Order::Kind::WAIT});
		}

		return true;
	}

	/**
	 * @fn
	 * �Ώۃ}�X�̓��_��Ԃ�
	 * @param (map) map�|�C���^
	 * @param (unit) �Ώۃ��j�b�g
	 * @param (x) �U���N�_�̃}�XX
	 * @param (y) �U���N�_�̃}�XY
	 * @param (targetUnit) �ő哾�_�̍U���Ώۃ��j�b�g��Ԃ����߂̎Q��
	 * @return ���_
	 */
	int EnemyAI::getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit)
	{
		int point = 0;

		// �U���\�}�X�擾
		vector<pair<int, int>> atkPosList;
		map->getAtackAbleMasses(unit, x, y, atkPosList, true);

		Fight fight = Fight();
		fight.init(map);

		if (atkPosList.size() > 0)
		{
			for (auto itr = atkPosList.begin(); itr != atkPosList.end(); ++itr)
			{
				// �Ώۃ}�X�U�����̓��_�v�Z
				int tmpPoint = 0;

				int targetMassX = itr->first;
				int targetMassY = itr->second;

				shared_ptr<Unit> tmpTargetUnit = map->getUnit(targetMassX, targetMassY);

				fight.setPrepare(unit, x, y, tmpTargetUnit);

				FightData actFight = fight.getFightData(true);
				FightData defFight = fight.getFightData(false);

				tmpPoint = actFight.damage * actFight.hitRate / 100;
				tmpPoint -= defFight.damage * defFight.hitRate / 100;

				if (tmpPoint > point) // ���_�������ꍇ
				{
					targetUnit = tmpTargetUnit;
					point = tmpPoint;
				}
			}
		}

		return point;
	}


	/**
	 * @fn
	 * �ŐV�̑�����e��Ԃ��i�ŐV�̑�����e�����X�g����폜�j
	 * @return ������e�i���݂��Ȃ��ꍇ�́j
	 */
	Order EnemyAI::getNextOrder()
	{
		if (orders_.size() > 0)
		{
			Order order = orders_.at(0);
			orders_.erase(orders_.begin());
			return order;
		}
		return Order();
	}

	/**
	 * @fn
	 * ���ɑ��삷�郆�j�b�g���擾
	 * @param (map) map�|�C���^�̎Q��
	 * @return �{���Ώۃ��j�b�g
	 */
	shared_ptr<Unit> EnemyAI::getNextUnit(shared_ptr<Map> map)
	{
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			shared_ptr<Unit> unit = itr->second;
			if (unit->isEnemy() && !unit->isActed())
			{
				return unit;
			}
		}
		return shared_ptr<Unit>();
	}

}