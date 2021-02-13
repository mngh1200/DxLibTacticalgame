#include "CheckWin.h"

namespace Battle {

	/**
	 * @fn
	 * �X�e�[�W�f�[�^�t�@�C������ǂݍ��񂾃e�L�X�g���珟�s���[���𐶐�
	*/
	void CheckWin::loadData(vector<int>& data)
	{
		if (data.size() > 1) // �^�[������
		{
			setLimitTurn(data.at(0), data.at(1) != 1);
		}

		if (data.size() > 2) // �h�q���[��
		{
			int rule = data.at(2);
			if (rule == Rule::DEFENSE || rule == Rule::CONFRICT)
			{
				setBaseDefense(true);
			}
			
			if (rule == Rule::ATTACK || rule == Rule::CONFRICT)
			{
				setBaseDefense(false);
			}
		}
	}

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
	 * �c��^�[�����b�Z�[�W
	 * @param (turnNum) �o�߃^�[��
	*/
	void CheckWin::showRemainingTurnMessage(shared_ptr<Message> message, int turnNum)
	{
		if (limitTurn_ == 0 || winner_ != Winner::UNDECIDED)
		{
			return; // �^�[���̏����������Ȃ��ꍇ�̓��b�Z�[�W��\�����Ȃ��A���s�m���͏o���Ȃ�
		}

		string text = "�c��" + to_string(limitTurn_ - turnNum + 1) + "�^�[����";
		
		if (isPlayerWinOverLimit_)
		{
			text = text + "����";
		}
		else
		{
			text = text + "�s�k";
		}

		message->show(text, false, 3000);
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
	 * ���������e�L�X�g�擾
	 * @param (text) �e�L�X�g�擾�p
	 * @param (lineCount) �s���擾�p
	*/
	void CheckWin::getWinConditionsText(string* text, int* lineCount) const
	{
		(*text) = "�E�G���j�b�g�̑S��";
		(*lineCount) = 1;

		if (isEnemyBaseDefense())
		{
			(*text) += "\n�E�G�Ԃ̐���";
			++(*lineCount);
		}

		if (isPlayerWinOverLimit() && getLimitTurn() > 0)
		{
			(*text) += "\n�E" + to_string(getLimitTurn()) + "�^�[���̌o��";
			++(*lineCount);
		}
		++(*lineCount);
	}

	/**
	 * @fn
	 * �s�k�����e�L�X�g�擾
	 * @param (text) �e�L�X�g�擾�p
	 * @param (lineCount) �s���擾�p
	*/
	void CheckWin::getLoseConditionsText(string* text, int* lineCount) const
	{
		// �s�k�������e
		(*text) = "�E���R���j�b�g�̑S��";
		(*lineCount) = 1;

		if (isPlayerBaseDefense())
		{
			(*text) += "\n�E���R�Ԃ�G������";
			++(*lineCount);
		}

		if (!isPlayerWinOverLimit() && getLimitTurn() > 0)
		{
			(*text) += "\n�E" + to_string(getLimitTurn()) + "�^�[���̌o��";
			++(*lineCount);
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