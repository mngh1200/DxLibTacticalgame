#pragma once
#include <climits>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "DxLib.h"
#include "FrameWork/Game.h"
#include "Network/NetworkDefine.h"
#include "Battle/BattleManager.h"

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
		const ContLog& getNextContLog(bool isDelete);
		void popContLog();
		bool checkReceiveSignal(int signal);

		bool execEnemyAction(Battle::BattleManager* bm, shared_ptr<Map> map, int enemyLayer);

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

		vector<int> signalList_; //! �P��̐��l����M���X�g
	};
}