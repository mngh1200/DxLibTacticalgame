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
	 * 単一数値情報を送信
	 * @param (signal) 単一数値情報
	 */
	void SendManager::sendSignal(int signal)
	{
		string data = to_string(NetworkDataType::SIGNAL) + VALUE_SPLIT + to_string(signal) + DATA_SPLIT;
		DxLib::NetWorkSend(netHandle_, data.c_str(), data.length());
	}

	/**
	 * @fn
	 * ルール設定を送信
	 * @param (ruleData) ルール設定
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
	 * プレイヤーの操作ログを送信
	 * @param (PlayerContLog) 操作ログ
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