#include "ReceiveManager.h"

namespace Network
{
	/**
	 * @fn
	 * ネットワークハンドルをセット
	 * @param (netHandle) ネットワークハンドル
	 */
	void ReceiveManager::setNetHandle(int netHandle)
	{
		netHandle_ = netHandle;
	}

	/**
	 * @fn
	 * ルール設定を取得
	 * @param (ruleData) ルール設定参照用変数
	 */
	void ReceiveManager::getRuleData(RuleData& ruleData)
	{
		ruleData = ruleData_;

	}

	/**
	 * @fn
	 * 次の敵プレイヤーの操作ログ取得
	 * @param(isDelete) trueの場合、先頭の操作ログ削除
	 * @return 次の敵プレイヤーの操作ログ（ない場合はactionKind = -1の状態で返す）
	 */
	ContLog ReceiveManager::getNextContLog(bool isDelete)
	{
		if (enemyPlayerContLogs_.size() > 0)
		{
			ContLog nextLog = enemyPlayerContLogs_.front();
			if (isDelete)
			{
				enemyPlayerContLogs_.pop();
			}
			return nextLog;
		}
		return ContLog();
	}

	/**
	 * @fn
	 * 先頭の操作ログを削除
	 */
	void ReceiveManager::popContLog()
	{
		enemyPlayerContLogs_.pop();
	}

	/**
	 * @fn
	 * 次の受信した単一数値取得（取得したログは削除）
	 * @return 次の受信した単一数値（ない場合はNO_SIGNALを返す）
	 */
	int ReceiveManager::getNextSignal()
	{
		if (signalList_.size() > 0)
		{
			int signal = signalList_.front();
			signalList_.pop();
			return signal;
		}
		return SignalKind::NO_SIGNAL;
	}

	/**
	 * @fn
	 * データ受信とデータ保持
	 * @return 受信データが有る場合 trueを返す
	 */
	bool ReceiveManager::receive()
	{
		bool isReceive = false;

		int dataLength = DxLib::GetNetWorkDataLength(netHandle_);

		char charBuf[1024];
		if (dataLength > 0 && DxLib::NetWorkRecv(netHandle_, &charBuf, dataLength) == 0)
		{
			string strBuf = string(charBuf);

			std::istringstream iStream(strBuf);
			string strDataBuf; // 一つのデータ

			while (getline(iStream, strDataBuf, DATA_SPLIT))
			{
				std::istringstream streamVal(strDataBuf);
				string strValBuf; // 一つの値

				vector<string> valList = {};
				while (getline(streamVal, strValBuf, VALUE_SPLIT))
				{
					valList.push_back(strValBuf);
				}

				// データの種類
				int dataType = NetworkDataType::NONE; 

				try
				{
					dataType = stoi(valList.at(0));
				}
				catch (const std::invalid_argument& e) {}
				catch (const std::out_of_range& e) {}
				

				if (dataType == NetworkDataType::SIGNAL) // 単一数値情報
				{
					addSignal(valList);
				}
				else if (dataType == NetworkDataType::RULE) // ルール設定
				{
					setRuleData(valList);
					isReceive = true;
				}
				else if (dataType == NetworkDataType::CONT_LOG) // 操作ログ
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
	 * ルール設定をセット
	 * @param (valList) 値ごとに区切った受信データ
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
	 * 敵プレイヤー操作ログを追加
	 * @param (valList) 値ごとに区切った受信データ
	 */
	void ReceiveManager::addEnemyPlayerContLogs(vector<string>& valList)
	{
		try
		{
			int x = stoi(valList.at(1));
			int y = stoi(valList.at(2));
			int unitId = stoi(valList.at(3));
			int actionKind = stoi(valList.at(4));

			// その他変数はあるときだけ
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
	 * 単一の数値情報
	 * @param (valList) 値ごとに区切った受信データ
	 */
	void ReceiveManager::addSignal(vector<string>& valList)
	{
		try
		{
			int signal = stoi(valList.at(1));
			signalList_.push(signal);
		}
		catch (const std::invalid_argument& e) {}
		catch (const std::out_of_range& e) {}
	}
}