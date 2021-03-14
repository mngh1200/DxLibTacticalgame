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
	 * 敵プレイヤーの操作ログ取得
	 * @param (enemyPlayerContLogs) 敵プレイヤーの操作ログ参照用変数
	 */
	void ReceiveManager::getEnemeyPlayerContLogs(vector<ContLog>& enemyPlayerContLogs)
	{
		enemyPlayerContLogs = enemyPlayerContLogs_;
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
		if (DxLib::NetWorkRecv(netHandle_, &charBuf, dataLength) == 0)
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
				

				if (dataType == NetworkDataType::RULE) // ルール設定
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

			// 命中乱数値はあるときだけ
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