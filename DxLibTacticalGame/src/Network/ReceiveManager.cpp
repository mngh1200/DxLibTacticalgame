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
	 * �G�v���C���[�̑��샍�O�擾
	 * @param (enemyPlayerContLogs) �G�v���C���[�̑��샍�O�Q�Ɨp�ϐ�
	 */
	void ReceiveManager::getEnemeyPlayerContLogs(vector<ContLog>& enemyPlayerContLogs)
	{
		enemyPlayerContLogs = enemyPlayerContLogs_;
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
		if (DxLib::NetWorkRecv(netHandle_, &charBuf, dataLength) == 0)
		{
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
				

				if (dataType == NetworkDataType::RULE) // ���[���ݒ�
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

			// ���������l�͂���Ƃ�����
			int hitValue = 0;
			if (valList.size() > 5)
			{
				hitValue = stoi(valList.at(1));
			}
			
			enemyPlayerContLogs_.push_back(ContLog{ x, y, unitId, actionKind, hitValue });
		}
		catch (const std::invalid_argument& e) {}
		catch (const std::out_of_range& e) {}
	}
}