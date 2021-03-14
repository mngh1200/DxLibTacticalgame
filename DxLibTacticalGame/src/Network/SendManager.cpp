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
	 * ���[���ݒ�𑗐M
	 * @param (ruleData) ���[���ݒ�
	 */
	void SendManager::sendRuleData(RuleData& ruleData)
	{
		string data = to_string(NetworkDataType::RULE) + "," + 
			to_string(ruleData.unitNum) + "," +
			to_string(ruleData.mapId);

		DxLib::NetWorkSend(netHandle_, data.c_str(), data.length());
	}

	/**
	 * @fn
	 * �v���C���[�̑��샍�O�𑗐M
	 * @param (PlayerContLog) ���샍�O
	 */
	void SendManager::sendPlayerContLog(ContLog& PlayerContLog)
	{
		string data = to_string(NetworkDataType::CONT_LOG) + "," +
			to_string(PlayerContLog.x) + "," +
			to_string(PlayerContLog.y) + "," +
			to_string(PlayerContLog.unitId) + "," +
			to_string(PlayerContLog.actionKind) + "," +
			to_string(PlayerContLog.hitValue);

		DxLib::NetWorkSend(netHandle_, data.c_str(), data.length());
	}
}