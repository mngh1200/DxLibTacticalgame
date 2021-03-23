#include "ReceiveManager.h"

namespace Network
{
	/**
	 * @fn
	 * �l�b�g���[�N�n���h�����Z�b�g
	 * @param (netHandle) �l�b�g���[�N�n���h��
	 */
	void ReceiveManager::setNetHandle(int netHandle)
	{
		netHandle_ = netHandle;
	}

	/**
	 * @fn
	 * ���[���ݒ���擾
	 * @param (ruleData) ���[���ݒ�Q�Ɨp�ϐ�
	 */
	void ReceiveManager::getRuleData(RuleData& ruleData)
	{
		ruleData = ruleData_;

	}

	/**
	 * @fn
	 * ���̓G�v���C���[�̑��샍�O�擾
	 * @param(isDelete) true�̏ꍇ�A�擪�̑��샍�O�폜
	 * @return ���̓G�v���C���[�̑��샍�O�i�Ȃ��ꍇ��actionKind = -1�̏�ԂŕԂ��j
	 */
	const ContLog& ReceiveManager::getNextContLog()
	{
		if (enemyPlayerContLogs_.size() > 0)
		{
			ContLog nextLog = enemyPlayerContLogs_.front();
			enemyPlayerContLogs_.pop();
			return nextLog;
		}
		return ContLog();
	}

	/**
	 * @fn
	 * ����̐M������M���Ă��邩 (��M���Ă����ꍇ�A�X�^�b�N���Ă���M��������)
	 * @return ��M���Ă���ꍇ true��Ԃ�
	 */
	bool ReceiveManager::checkReceiveSignal(int signal)
	{
		if (signalList_.size() > 0)
		{
			for (auto itr = signalList_.begin(); itr != signalList_.end(); ++itr)
			{
				if ((*itr) == signal)
				{
					signalList_.erase(itr); // �M������
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * @fn
	 * ��M�����f�[�^�����ɓG�v���C���[�̑�������s
	 * @return �G�^�[���I������true��Ԃ�
	 */
	bool ReceiveManager::execEnemyAction(Battle::BattleManager* bm, shared_ptr<Map> map, int enemyLayer)
	{
		if (bm->isAnimation()) // �A�j���[�V�������͑�������s���Ȃ�
		{
			return false;
		}

		if (enemyPlayerContLogs_.size() == 0 && checkReceiveSignal(SignalKind::TURN_END)) // �G�^�[���I��
		{
			return true;
		}

		ContLog log = getNextContLog(true);

		if (log.actionKind == -1) // ��M�������샍�O���Ȃ��ꍇ
		{
			return false;
		}

		// �Ώۃ��j�b�g�擾
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(objectsControl.getObjectWp(enemyLayer, log.unitId).lock());

		if (!unit) // �Ώۃ��j�b�g�����݂��Ȃ��ꍇ
		{
			return false;
		}

		if (log.actionKind == ActionKind::MOVE_ACT) // �ړ�
		{
			bm->selectUnit(unit);
			bm->moveAction(log.x, log.y);
		}
		else if (log.actionKind == ActionKind::WAIT_ACT) // �ҋ@
		{
			bm->waitAction();
		}

		return false;
	}

	/**
	 * @fn
	 * �f�[�^��M�ƃf�[�^�ێ�
	 * @return ��M�f�[�^���L��ꍇ true��Ԃ�
	 */
	bool ReceiveManager::receive()
	{
		bool isReceive = false;

		int dataLength = DxLib::GetNetWorkDataLength(netHandle_);

		char charBuf[1024];
		if (dataLength > 0 && DxLib::NetWorkRecv(netHandle_, &charBuf, dataLength) == 0)
		{
			DxLib::printfDx(charBuf);

			string strBuf = string(charBuf);

			std::istringstream iStream(strBuf);
			string strDataBuf; // ��̃f�[�^

			while (getline(iStream, strDataBuf, DATA_SPLIT))
			{
				std::istringstream streamVal(strDataBuf);
				string strValBuf; // ��̒l

				vector<string> valList = {};
				while (getline(streamVal, strValBuf, VALUE_SPLIT))
				{
					valList.push_back(strValBuf);
				}

				// �f�[�^�̎��
				int dataType = NetworkDataType::NONE; 

				try
				{
					dataType = stoi(valList.at(0));
				}
				catch (const std::invalid_argument& e) {}
				catch (const std::out_of_range& e) {}
				

				if (dataType == NetworkDataType::SIGNAL) // �P�ꐔ�l���
				{
					addSignal(valList);
				}
				else if (dataType == NetworkDataType::RULE) // ���[���ݒ�
				{
					setRuleData(valList);
					isReceive = true;
				}
				else if (dataType == NetworkDataType::CONT_LOG) // ���샍�O
				{
					addEnemyPlayerContLogs(valList);
					isReceive = true;
				}
			}
		}

		return isReceive;
	}

	/**
	 * @fn
	 * ���[���ݒ���Z�b�g
	 * @param (valList) �l���Ƃɋ�؂�����M�f�[�^
	 */
	void ReceiveManager::setRuleData(vector<string>& valList)
	{
		try
		{
			int unitNum = stoi(valList.at(1));
			int mapId = stoi(valList.at(2));
			ruleData_ = RuleData{ unitNum , mapId };

			isReceivedRule_ = true;
		}
		catch (const std::invalid_argument& e) {}
		catch (const std::out_of_range& e) {}
	}

	/**
	 * @fn
	 * �G�v���C���[���샍�O��ǉ�
	 * @param (valList) �l���Ƃɋ�؂�����M�f�[�^
	 */
	void ReceiveManager::addEnemyPlayerContLogs(vector<string>& valList)
	{
		try
		{
			int x = stoi(valList.at(1));
			int y = stoi(valList.at(2));
			int unitId = stoi(valList.at(3));
			int actionKind = stoi(valList.at(4));

			// ���̑��ϐ��͂���Ƃ�����
			int extraValue = 0;
			if (valList.size() > 5)
			{
				extraValue = stoi(valList.at(5));
			}
			
			enemyPlayerContLogs_.push(ContLog{ x, y, unitId, actionKind, extraValue });
		}
		catch (const std::invalid_argument& e) {}
		catch (const std::out_of_range& e) {}
	}

	/**
	 * @fn
	 * �P��̐��l���
	 * @param (valList) �l���Ƃɋ�؂�����M�f�[�^
	 */
	void ReceiveManager::addSignal(vector<string>& valList)
	{
		try
		{
			int signal = stoi(valList.at(1));
			signalList_.push_back(signal);
		}
		catch (const std::invalid_argument& e) {}
		catch (const std::out_of_range& e) {}
	}
}