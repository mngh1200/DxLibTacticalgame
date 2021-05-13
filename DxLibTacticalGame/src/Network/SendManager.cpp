#include "SendManager.h"

namespace Network
{
	/**
	 * @fn
	 * �l�b�g���[�N�n���h�����Z�b�g
	 * @param (netHandle) �l�b�g���[�N�n���h��
	 */
	void SendManager::setNetHandle(int netHandle)
	{
		netHandle_ = netHandle;
	}

	/**
	 * @fn
	 * �P�ꐔ�l���𑗐M
	 * @param (signal) �P�ꐔ�l���
	 */
	void SendManager::sendSignal(int signal)
	{
		string data = to_string(NetworkDataType::SIGNAL) + VALUE_SPLIT + to_string(signal) + DATA_SPLIT;
		DxLib::NetWorkSend(netHandle_, data.c_str(), data.length());
	}

	/**
	 * @fn
	 * ���[���ݒ�𑗐M
	 * @param (ruleData) ���[���ݒ�
	 */
	void SendManager::sendRuleData(RuleData& ruleData)
	{
		string data = to_string(NetworkDataType::RULE) + VALUE_SPLIT +
			to_string(ruleData.unitNum) + VALUE_SPLIT +
			to_string(ruleData.mapId) + VALUE_SPLIT + 
			(ruleData.isClientFirst ? "1" : "0") + DATA_SPLIT;

		DxLib::NetWorkSend(netHandle_, data.c_str(), data.length());
	}

	/**
	 * @fn
	 * �v���C���[�̑��샍�O�𑗐M
	 * @param (PlayerContLog) ���샍�O
	 */
	void SendManager::sendPlayerContLog(ContLog& PlayerContLog)
	{
		// DxLib::printfDx(("send(" + to_string(PlayerContLog.extraValue1) + "," + to_string(PlayerContLog.extraValue2) + ");").c_str());

		string data = to_string(NetworkDataType::CONT_LOG) + VALUE_SPLIT +
			to_string(PlayerContLog.x) + VALUE_SPLIT +
			to_string(PlayerContLog.y) + VALUE_SPLIT +
			to_string(PlayerContLog.unitId) + VALUE_SPLIT +
			to_string(PlayerContLog.actionKind) + VALUE_SPLIT +
			to_string(PlayerContLog.extraValue1) + VALUE_SPLIT +
			to_string(PlayerContLog.extraValue2) + DATA_SPLIT;

		DxLib::NetWorkSend(netHandle_, data.c_str(), data.length());
	}
}