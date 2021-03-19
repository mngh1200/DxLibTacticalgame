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
 * @brief ネットワーク受信したデータ管理用クラス
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

		bool isReceivedRule() const { return isReceivedRule_; } // ルールデータを受信済みか
		int getNetHandle() const { return netHandle_; } // ネットハンドルをを返す

	private:
		void setRuleData(vector<string>& valList);
		void addEnemyPlayerContLogs(vector<string>& valList);
		void addSignal(vector<string>& valList);

		int netHandle_; //! ネットワークハンドル

		bool isReceivedRule_; //! ルールデータを受信済みか

		RuleData ruleData_; //! 受信したルール設定

		queue<ContLog> enemyPlayerContLogs_; //! 敵プレイヤーの操作ログ 

		queue<int> signalList_; //! 単一の数値情報受信リスト
	};
}