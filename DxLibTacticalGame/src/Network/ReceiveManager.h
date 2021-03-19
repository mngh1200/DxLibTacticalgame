#pragma once
#include <climits>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include "DxLib.h"
#include "Network/NetworkDefine.h"

using namespace std;

/**
 * @file ReceiveManager.h
 * @brief �l�b�g���[�N��M�����f�[�^�Ǘ��p�N���X
 */

namespace Network
{
	class ReceiveManager
	{
	public:
		ReceiveManager() : netHandle_(-1), isReceivedRule_(false) {};
		~ReceiveManager() {};

		void setNetHandle(int netHandle);

		bool receive();
		void getRuleData(RuleData& ruleData);
		ContLog getNextContLog(bool isDelete = true);
		void popContLog();
		int getNextSignal();

		bool isReceivedRule() const { return isReceivedRule_; } // ���[���f�[�^����M�ς݂�
		int getNetHandle() const { return netHandle_; } // �l�b�g�n���h������Ԃ�

	private:
		void setRuleData(vector<string>& valList);
		void addEnemyPlayerContLogs(vector<string>& valList);
		void addSignal(vector<string>& valList);

		int netHandle_; //! �l�b�g���[�N�n���h��

		bool isReceivedRule_; //! ���[���f�[�^����M�ς݂�

		RuleData ruleData_; //! ��M�������[���ݒ�

		queue<ContLog> enemyPlayerContLogs_; //! �G�v���C���[�̑��샍�O 

		queue<int> signalList_; //! �P��̐��l����M���X�g
	};
}