#include "SendManager.h"

namespace Network
{
	/**
	 * @fn
	 * ネットワークハンドルをセット
	 * @param (netHandle) ネットワークハンドル
	 */
	void SendManager::setNetHandle(int netHandle)
	{
		netHandle_ = netHandle;
	}

	/**
	 * @fn
	 * ルール設定を送信
	 * @param (ruleData) ルール設定
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
	 * プレイヤーの操作ログを送信
	 * @param (PlayerContLog) 操作ログ
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