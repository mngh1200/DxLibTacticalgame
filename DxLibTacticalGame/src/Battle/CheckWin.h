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
		CheckWin() : winner_(Winner::UNDECIDED) {};
		~CheckWin() {};

		void checkWin(shared_ptr<Map> map);

		int getWinner() const { return winner_; }; // 勝者を返す

	private:

		//! 勝者
		int winner_;

		// 勝者の値
		enum Winner
		{
			UNDECIDED,
			PLAYER,
			ENEMY
		};
	};


}