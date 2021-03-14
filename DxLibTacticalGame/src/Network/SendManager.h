#pragma once
#include <climits>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "DxLib.h"
#include "Network/NetworkDefine.h"

using namespace std;

/**
 * @file SendManager.h
 * @brief �l�b�g���[�N���M�Ǘ��p�N���X
 */

namespace Network
{
	class SendManager
	{
	public:
		SendManager() : netHandle_(-1) {};
		~SendManager() {};

		void setNetHandle(int netHandle);

		void sendRuleData(RuleData& ruleData);
		void sendPlayerContLog(ContLog& PlayerContLog);

	private:
		int netHandle_; //! �l�b�g���[�N�n���h��
	};
}