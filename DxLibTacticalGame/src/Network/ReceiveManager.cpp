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
	const ContLog& ReceiveManager::getNextContLog(bool isDelete)
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
	 * 先頭の敵プレイヤーの操作ログ削除
	 */
	void ReceiveManager::popContLog()
	{
		enemyPlayerContLogs_.pop();
	}

	/**
	 * @fn
	 * 特定の信号を受信しているか (受信していた場合、スタックしている信号を除去)
	 * @return 受信している場合 trueを返す
	 */
	bool ReceiveManager::checkReceiveSignal(int signal)
	{
		if (signalList_.size() > 0)
		{
			for (auto itr = signalList_.begin(); itr != signalList_.end(); ++itr)
			{
				if ((*itr) == signal)
				{
					signalList_.erase(itr); // 信号除去
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * @fn
	 * 受信したデータを元に敵プレイヤーの操作を実行
	 * @return 敵ターン終了時にtrueを返す
	 */
	bool ReceiveManager::execEnemyAction(Battle::BattleManager* bm, shared_ptr<Map> map, int enemyLayer)
	{
		if (bm->isAnimation()) // アニメーション中は操作を実行しない
		{
			return false;
		}

		if (enemyPlayerContLogs_.size() == 0 && checkReceiveSignal(SignalKind::TURN_END)) // 敵ターン終了
		{
			return true;
		}

		ContLog log = getNextContLog(true);

		if (log.actionKind == -1) // 受信した操作ログがない場合
		{
			return false;
		}

		// 対象ユニット取得
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(objectsControl.getObjectWp(enemyLayer, log.unitId).lock());

		if (!unit) // 対象ユニットが存在しない場合
		{
			return false;
		}

		if (log.actionKind == ActionKind::MOVE_ACT) // 移動
		{
			bm->selectUnit(unit);
			bm->moveAction(log.x, log.y);
		}
		else if (log.actionKind == ActionKind::WAIT_ACT) // 待機
		{
			bm->waitAction();
		}
		else if (log.actionKind == ActionKind::ATACK_ACT) // 攻撃
		{
			bm->setFightPredict(map->getUnit(log.x, log.y));
			bm->atackAction(log.extraValue1, log.extraValue2);
		}

		return false;
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
			// DxLib::printfDx(charBuf);

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
			ruleData_ = RuleData{};

			ruleData_.unitNum = stoi(valList.at(1));
			ruleData_.mapId = stoi(valList.at(2));
			ruleData_.isClientFirst = valList.at(3) == "1";

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
			int extraValue1 = 0;
			if (valList.size() > 5)
			{
				extraValue1 = stoi(valList.at(5));
			}

			int extraValue2 = 0;
			if (valList.size() > 6)
			{
				extraValue2 = stoi(valList.at(6));
			}
			
			enemyPlayerContLogs_.push(ContLog{ x, y, unitId, actionKind, extraValue1, extraValue2 });
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
			signalList_.push_back(signal);
		}
		catch (const std::invalid_argument& e) {}
		catch (const std::out_of_range& e) {}
	}
}