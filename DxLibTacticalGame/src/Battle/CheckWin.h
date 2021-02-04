#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Message.h"

using namespace std;
using namespace Entity;

/**
 * @file CheckWin.h
 * @brief 勝敗条件チェック用のクラス
 */

namespace Battle
{
	class CheckWin
	{
	public:
		CheckWin() : 
			winner_(Winner::UNDECIDED),
			limitTurn_(0),
			isPlayerWinOverLimit_(true),
			isPlayerBaseDefense_(false),
			isEnemyBaseDefense_(false)
		{};
		~CheckWin() {};

		// 勝者の値
		enum Winner
		{
			UNDECIDED,
			PLAYER,
			ENEMY
		};

		void loadData(vector<int>& data);

		void setLimitTurn(int turnNum, bool isPlayerWinOverLimit);

		void setBaseDefense(bool isPlayer);

		void showRemainingTurnMessage(shared_ptr<Message> message, int turnNum);

		void checkWin(shared_ptr<Map> map);
		void checkWin(int turnNum);

		int getWinner() const { return winner_; }; // 勝者を返す

	private:
		int checkWipeOut(shared_ptr<Map> map);

		//! 勝者
		int winner_;

		//! ターン制限
		int limitTurn_;

		//! ターン制限が切れたタイミングでプレイヤーの勝利にするか
		bool isPlayerWinOverLimit_;

		//! プレイヤー側の防衛拠点があるか
		bool isPlayerBaseDefense_;

		//! 敵側の防衛拠点があるか
		bool isEnemyBaseDefense_;

		enum Rule
		{
			NORMAL,  //! 防衛地点なし
			DEFENSE, //! プレイヤー防衛地点有り
			ATTACK,  //! 敵側の防衛地点あり
			CONFRICT //! 両側の防衛地点あり
		};
	};


}