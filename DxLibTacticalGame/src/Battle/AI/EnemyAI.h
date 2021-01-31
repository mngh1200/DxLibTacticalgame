#pragma once
#include <climits>
#include <deque>
#include "Battle/Fight.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

using namespace std;
using namespace Entity;

/**
 * @file EnemyAI.h
 * @brief 敵の操作判断AIクラス
 */

namespace Battle
{
	// 操作内容を保持
	struct Order
	{
		// 操作の種類
		enum Kind
		{
			NONE,
			MOVE,
			WAIT,
			ATACK
		};

		int kind = Kind::NONE; //! 操作の種類
		shared_ptr<Unit> unit; //! 操作対象ユニット
		int massX = 0; //! 対象マスX
		int massY = 0; //! 対象マスY
	};

	class EnemyAI
	{
	public:
		EnemyAI() {};
		~EnemyAI() {};

		void init(shared_ptr<Map> map);
		bool createOrders(shared_ptr<Map> map);
		Order getNextOrder();

		map<pair<int, int>, int> massBaseScoreMap; //! 各マスの基本スコア

	protected:
		constexpr static int SCORE_BY_FORT = MAP_MASS_W + MAP_MASS_H - 2; // 砦から波及する基本スコアの加算

		void setBaseScore(shared_ptr<Map> map);
		void setBaseScoreByFort(shared_ptr<Map> map, int x, int y, int move, std::map<pair<int, int>, int>& tmpBaseScoreMap);
		shared_ptr<Unit> getNextUnit(shared_ptr<Map> map);
		
		deque<Order> orders_; //! 操作手順

	private:
		int getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit);
	};
}