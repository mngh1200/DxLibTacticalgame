#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

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

		void setLimitTurn(int turnNum, bool isPlayerWinOverLimit);

		void setBaseDefense(bool isPlayer);

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
	};


}