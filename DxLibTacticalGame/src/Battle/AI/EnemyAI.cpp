#include "EnemyAI.h"

namespace Battle {
	/**
	 * @fn
	 * ��������
	 * @param (map) map�|�C���^
	 */
	void EnemyAI::init(shared_ptr<Map> map)
	{
		setBaseScore(map);
	}

	/**
	 * @fn
	 * AI�̎u���������Z�b�g����
	 * @param (oriented) �u������
	 */
	void EnemyAI::setOriented(Oriented oriented)
	{
		oriented_ = oriented;
	}

	/**
	 * @fn
	 * �G�ɋ߂Â�
	 * @param (map) map�|�C���^
	 */
	void closeWithEnemy(shared_ptr<Map> map, shared_ptr<Unit> unit, pair<int, int>* targetPos)
	{
		// ���T���p�̍��W���ix, y, �R�X�g, �ړ��\�͈͒��̈ړ������WX, �ړ��\�͈͒��̈ړ������WY�j
		deque<tuple<int, int, int, int, int>> posDeque;

		posDeque.push_back(make_tuple(unit->getMassX(), unit->getMassY(), 1, unit->getMassX(), unit->getMassY()));

		// ���K��}�X��-1�ɂ��Ă���
		vector<vector<int> > dist(MAP_MASS_W, vector<int>(MAP_MASS_H, -1));

		dist[unit->getMassX()][unit->getMassY()] = 0; // �����n�_��0

		while (!posDeque.empty())
		{
			tuple<int, int, int, int, int> current_pos = posDeque.front();
			
			int x = get<0>(current_pos);
			int y = get<1>(current_pos);
			int cost = get<2>(current_pos);
			int movX = get<3>(current_pos);
			int movY = get<4>(current_pos);

			posDeque.pop_front();

			--cost;
			if (cost > 0) // �c��ړ��R�X�g��0��葽���ꍇ�A���
			{
				posDeque.push_back(make_tuple(x, y, cost, movX, movY));
				continue;
			}


			// �אڒ��_��T��
			for (int direction = 0; direction < 4; ++direction) {
				int nextX = x + dx[direction];
				int nextY = y + dy[direction];

				if (nextX < 0 || nextX >= MAP_MASS_W || nextY < 0 || nextY >= MAP_MASS_H)
				{
					continue; // ��O
				}

				shared_ptr<Mass> nextMass = map->getMass(nextX, nextY);

				if (!nextMass || nextMass->getCost() == INT_MAX)
				{
					continue; // �ʍs�s��
				}

				shared_ptr<Unit> unit = map->getUnit(nextX, nextY);

				// �v���C���[���j�b�g�����݂���ꍇ�A�T���I��
				if (unit && !unit->isEnemy())
				{
					*targetPos = make_pair(movX, movY);
					return;
				}

				
				// �܂����Ă��Ȃ����_�Ȃ� push
				if (dist[nextX][nextY] == -1) {
					if (nextMass->passingMov > -1) // �ړ��\�͈͂̏ꍇ�A�ړ����X�V
					{
						movX = nextX;
						movY = nextY;
					}
					posDeque.push_back(make_tuple(nextX, nextY, nextMass->getCost(), movX, movY));
				}
			}
		}
	}

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

			// ���݃}�X�ɗ��܂�u���X�R�A
			if (oriented_.stay != 0 && itr->first == unit->getMassX() && itr->second == unit->getMassY())
			{
				point += oriented_.stay;
			}

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

		// �蕉���ōU���Ώۂ����Ȃ������ꍇ�A�Čv�Z
		if (THRESHOLD_CLOSE_OR_STAY > oriented_.stay && unit->isStricken() && !targetUnit)
		{
			// �˒��O����U������Ă��邱�Ƃ��l�����ėD�悵�āA�U���Ώۂ�T��
			closeWithEnemy(map, unit, &targetPos);
		}

		// �ړ�����
		orders_.push_back(Order{ActionKind::MOVE_ACT, unit, targetPos.first, targetPos.second});

		if (targetUnit)
		{
			// �U������
			orders_.push_back(Order{ ActionKind::ATACK_ACT, unit, targetUnit->getMassX(), targetUnit->getMassY()});
		}
		else
		{
			// �ҋ@����
			orders_.push_back(Order{ ActionKind::WAIT_ACT});
		}

		return true;
	}

	/**
	 * @fn
	 * �����ȊO�̖������j�b�g�����邩����
	 * @param (unit) ��̃��j�b�g
	 * @param (x) �Ώۂ�x���W
	 * @param (y) �Ώۂ�y���W
	 * @return �אڎ� true��Ԃ�
	 */
	bool isFriendExist(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y)
	{
		shared_ptr<Unit> targetUnit = map->getUnit(x, y);
		if (targetUnit && targetUnit != unit && targetUnit->isEnemy() == unit->isEnemy())
		{
			return true;
		}
		return false;
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

				tmpPoint = (actFight.damage * actFight.hitRate / 100) * oriented_.atack;
				tmpPoint -= (defFight.damage * defFight.hitRate / 100)* oriented_.survive;

				if (tmpPoint > point) // ���_�������ꍇ
				{
					targetUnit = tmpTargetUnit;
					point = tmpPoint;
				}
			}
		}

		// �������j�b�g�Ƃ̗אڃ{�[�i�X
		int adjacentCount = 0; // �אڐ�

		if (isFriendExist(map, unit, x, y + 1))
		{
			++adjacentCount;
		}

		if (isFriendExist(map, unit, x, y - 1))
		{
			++adjacentCount;
		}

		if (isFriendExist(map, unit, x + 1, y))
		{
			++adjacentCount;
		}

		if (isFriendExist(map, unit, x - 1, y))
		{
			++adjacentCount;
		}

		point += adjacentCount * SCORE_BY_FRIEND * oriented_.friendship;

		// ��{�X�R�A�����Z
		if (massBaseScoreMap.count(make_pair(x, y)) != 0)
		{
			point += massBaseScoreMap.at(make_pair(x, y));
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
	 * �e�}�X���W�̊�{�X�R�A���Z�b�g
	 * @param (map) map�|�C���^�̎Q��
	 */
	void EnemyAI::setBaseScore(shared_ptr<Map> map)
	{
		int y = 0;
		int x = 0;

		for (auto yItr = map->massBegin(); yItr != map->massEnd(); ++yItr)
		{
			auto lineItr = *yItr;
			for (auto xItr = lineItr.begin(); xItr != lineItr.end(); ++xItr)
			{
				int kind = (*xItr)->getKind();

				if (kind == Mass::Kind::FORT_ENEMY || kind == Mass::Kind::FORT_PLAYER) // �ԃ}�X
				{
					std::map<pair<int, int>, int> tmpBaseScoreMap;
					massBaseScoreMap[make_pair(x, y)] = FORT_SCORE * oriented_.targetPoint;

					setBaseScoreByFort(map, x, y, SCORE_BY_FORT * oriented_.targetPoint, tmpBaseScoreMap);
					map->clearMassState();

					// �Ԃ���g�y�����{�X�R�A���Z�b�g
					for (auto mapItr = tmpBaseScoreMap.begin(); mapItr != tmpBaseScoreMap.end(); ++mapItr)
					{
						auto key = mapItr->first;
						auto val = mapItr->second;
						if (massBaseScoreMap.count(key) == 0) // ���w��̏ꍇ�̓Z�b�g
						{
							massBaseScoreMap.emplace(key, val);
						}
						else // ���łɂ���ꍇ�͉��Z
						{
							massBaseScoreMap[key] += val;
						}
					}
				}

				++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * �ԃ}�X�̎���ɔg�y�����{�X�R�A�̏㏸����ǉ�
	 * @param (map) map�|�C���^�̎Q��
	 */
	void EnemyAI::setBaseScoreByFort(shared_ptr<Map> map, int x, int y, int move, std::map<pair<int, int>, int>& tmpBaseScoreMap)
	{
		shared_ptr<Mass> nowMass = map->getMass(x, y);

		// �}�b�v�O
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		if (nowMass->getCost() == INT_MAX)
		{
			move = 0;
		}
		else
		{
			move = move - nowMass->getCost() * FORT_MOVE_SCORE_RATE * oriented_.targetPoint;
		}
		
		if (move > nowMass->passingMov)
		{
			nowMass->passingMov = move;
			tmpBaseScoreMap[make_pair(x, y)] =  move;

			setBaseScoreByFort(map, x, y + 1, move, tmpBaseScoreMap);
			setBaseScoreByFort(map, x, y - 1, move, tmpBaseScoreMap);
			setBaseScoreByFort(map, x - 1, y, move, tmpBaseScoreMap);
			setBaseScoreByFort(map, x + 1, y, move, tmpBaseScoreMap);
		}
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