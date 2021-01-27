#include "CheckWin.h"

namespace Battle {

	/**
	 * @fn
	 * �^�[�������ɂ�鏟�s�����ǉ�
	 * @param (turnNum) �^�[������
	 * @param (isPlayerWinOverLimit) �^�[�������𒴂����Ƃ��Ƀv���C���[���������̏ꍇ: true
	*/
	void CheckWin::setLimitTurn(int limitTurn, bool isPlayerWinOverLimit)
	{
		limitTurn_ = limitTurn;
		isPlayerWinOverLimit_ = isPlayerWinOverLimit;
	}

	/**
	 * @fn
	 * �h�q�n�_�ɂ�鏟�s�����ǉ�
	 * @param (isPlayer) �v���C���[���̖h�q�n�_�̏ꍇ: true
	*/
	void CheckWin::setBaseDefense(bool isPlayer)
	{
		if (isPlayer)
		{
			isPlayerBaseDefense_ = true;
		}
		else
		{
			isEnemyBaseDefense_ = true;
		}
	}

	/**
	 * @fn
	 * ���s����
	 * @param (turnNum) �o�߃^�[��
	*/
	void CheckWin::checkWin(int turnNum)
	{
		if (limitTurn_ == 0)
		{
			return;
		}

		// �^�[���I�[�o�[
		if (turnNum > limitTurn_)
		{
			if (isPlayerWinOverLimit_)
			{
				winner_ = Winner::PLAYER;
			}
			else
			{
				winner_ = Winner::ENEMY;
			}
		}
	}

	/**
	 * @fn
	 * ���s����
	 * @param (map) Map�̃X�}�[�g�|�C���^
	*/
	void CheckWin::checkWin(shared_ptr<Map> map)
	{
		winner_ = checkWipeOut(map); // ���j�b�g�S�Ŕ���

		if (winner_ != Winner::UNDECIDED)
		{
			return;
		}

		if (isPlayerBaseDefense_) // �v���C���[���h�q���_����
		{
			pair<int, int> fortPos = map->getPlayerFortMass();
			shared_ptr<Unit> unit = map->getUnit(fortPos.first, fortPos.second);

			if (unit && unit->isEnemy())
			{
				winner_ = Winner::ENEMY;
				return;
			}
		}

		if (isEnemyBaseDefense_) // �G���h�q���_����
		{
			pair<int, int> fortPos = map->getEnemyFortMass();
			shared_ptr<Unit> unit = map->getUnit(fortPos.first, fortPos.second);

			if (unit && !unit->isEnemy())
			{
				winner_ = Winner::PLAYER;
			}
		}
	}

	/**
	 * @fn
	 * ���j�b�g�S�Ŕ���
	 * @return ���s���茋��
	*/
	int CheckWin::checkWipeOut(shared_ptr<Map> map)
	{
		bool existEnemyUnit = false;
		bool existPlayerUnit = false;

		// �ǂ��炩�̃��j�b�g���S�Ă��Ȃ��Ȃ��Ă��邩�`�F�b�N
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			if (itr->second->isEnemy())
			{
				existEnemyUnit = true;
			}
			else
			{
				existPlayerUnit = true;
			}

			if (existEnemyUnit && existPlayerUnit)
			{
				return Winner::UNDECIDED;
			}
		}

		if (!existPlayerUnit)
		{
			return Winner::ENEMY;
		}
		else if (!existEnemyUnit)
		{
			return Winner::PLAYER;
		}

		return Winner::UNDECIDED;
	}
}