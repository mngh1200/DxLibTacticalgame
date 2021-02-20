#include "CheckWin.h"

namespace Battle {

	/**
	 * @fn
	 * ステージデータファイルから読み込んだテキストから勝敗ルールを生成
	*/
	void CheckWin::loadData(vector<int>& data)
	{
		if (data.size() > 1) // ターン制限
		{
			setLimitTurn(data.at(0), data.at(1) != 1);
		}

		if (data.size() > 2) // 防衛ルール
		{
			int rule = data.at(2);
			if (rule == Rule::DEFENSE || rule == Rule::CONFRICT)
			{
				setBaseDefense(true);
			}
			
			if (rule == Rule::ATTACK || rule == Rule::CONFRICT)
			{
				setBaseDefense(false);
			}
		}
	}

	/**
	 * @fn
	 * ターン制限による勝敗条件追加
	 * @param (turnNum) ターン制限
	 * @param (isPlayerWinOverLimit) ターン制限を超えたときにプレイヤー側が勝利の場合: true
	*/
	void CheckWin::setLimitTurn(int limitTurn, bool isPlayerWinOverLimit)
	{
		limitTurn_ = limitTurn;
		isPlayerWinOverLimit_ = isPlayerWinOverLimit;
	}

	/**
	 * @fn
	 * 防衛地点による勝敗条件追加
	 * @param (isPlayer) プレイヤー側の防衛地点の場合: true
	*/
	void CheckWin::setBaseDefense(bool isPlayer)
	{
		if (isPlayer)
		{
			isPlayerBaseDefense_ = true;
		}
		else
		{
			isEnemyBaseDefense_ = true;
		}
	}

	/**
	 * @fn
	 * 残りターンメッセージ
	 * @param (turnNum) 経過ターン
	*/
	void CheckWin::showRemainingTurnMessage(shared_ptr<Message> message, int turnNum)
	{
		if (limitTurn_ == 0 || winner_ != Winner::UNDECIDED)
		{
			return; // ターンの勝利条件がない場合はメッセージを表示しない、勝敗確定後は出さない
		}

		string text = "残り" + to_string(limitTurn_ - turnNum + 1) + "ターンで";
		
		if (isPlayerWinOverLimit_)
		{
			text = text + "勝利";
		}
		else
		{
			text = text + "敗北";
		}

		message->show(text, false, 3000);
	}

	/**
	 * @fn
	 * 勝敗判定
	 * @param (turnNum) 経過ターン
	*/
	void CheckWin::checkWin(int turnNum)
	{
		if (limitTurn_ == 0)
		{
			return;
		}

		// ターンオーバー
		if (turnNum > limitTurn_)
		{
			if (isPlayerWinOverLimit_)
			{
				winner_ = Winner::PLAYER;
			}
			else
			{
				winner_ = Winner::ENEMY;
			}
		}
	}

	/**
	 * @fn
	 * 勝利条件テキスト取得
	 * @param (text) テキスト取得用
	 * @param (lineCount) 行数取得用
	*/
	void CheckWin::getWinConditionsText(string* text, int* lineCount) const
	{
		(*text) = "・敵ユニットの全滅";
		(*lineCount) = 1;

		if (isEnemyBaseDefense())
		{
			(*text) += "\n・敵砦の制圧";
			++(*lineCount);
		}

		if (isPlayerWinOverLimit() && getLimitTurn() > 0)
		{
			(*text) += "\n・" + to_string(getLimitTurn()) + "ターンの経過";
			++(*lineCount);
		}
		++(*lineCount);
	}

	/**
	 * @fn
	 * 敗北条件テキスト取得
	 * @param (text) テキスト取得用
	 * @param (lineCount) 行数取得用
	*/
	void CheckWin::getLoseConditionsText(string* text, int* lineCount) const
	{
		// 敗北条件内容
		(*text) = "・自軍ユニットの全滅";
		(*lineCount) = 1;

		if (isPlayerBaseDefense())
		{
			(*text) += "\n・自軍砦を敵が制圧";
			++(*lineCount);
		}

		if (!isPlayerWinOverLimit() && getLimitTurn() > 0)
		{
			(*text) += "\n・" + to_string(getLimitTurn()) + "ターンの経過";
			++(*lineCount);
		}
	}

	/**
	 * @fn
	 * 勝敗判定
	 * @param (map) Mapのスマートポインタ
	*/
	void CheckWin::checkWin(shared_ptr<Map> map)
	{
		winner_ = checkWipeOut(map); // ユニット全滅判定

		if (winner_ != Winner::UNDECIDED)
		{
			return;
		}

		if (isPlayerBaseDefense_) // プレイヤー側防衛拠点判定
		{
			pair<int, int> fortPos = map->getPlayerFortMass();
			shared_ptr<Unit> unit = map->getUnit(fortPos.first, fortPos.second);

			if (unit && unit->isEnemy())
			{
				winner_ = Winner::ENEMY;
				return;
			}
		}

		if (isEnemyBaseDefense_) // 敵側防衛拠点判定
		{
			pair<int, int> fortPos = map->getEnemyFortMass();
			shared_ptr<Unit> unit = map->getUnit(fortPos.first, fortPos.second);

			if (unit && !unit->isEnemy())
			{
				winner_ = Winner::PLAYER;
			}
		}
	}

	/**
	 * @fn
	 * ユニット全滅判定
	 * @return 勝敗判定結果
	*/
	int CheckWin::checkWipeOut(shared_ptr<Map> map)
	{
		bool existEnemyUnit = false;
		bool existPlayerUnit = false;

		// どちらかのユニットが全ていなくなっているかチェック
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			if (itr->second->isEnemy())
			{
				existEnemyUnit = true;
			}
			else
			{
				existPlayerUnit = true;
			}

			if (existEnemyUnit && existPlayerUnit)
			{
				return Winner::UNDECIDED;
			}
		}

		if (!existPlayerUnit)
		{
			return Winner::ENEMY;
		}
		else if (!existEnemyUnit)
		{
			return Winner::PLAYER;
		}

		return Winner::UNDECIDED;
	}
}