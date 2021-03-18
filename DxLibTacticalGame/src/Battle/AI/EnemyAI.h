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
		int kind = ActionKind::NO_ACTION; //! 操作の種類
		shared_ptr<Unit> unit; //! 操作対象ユニット
		int massX = 0; //! 対象マスX
		int massY = 0; //! 対象マスY
	};

	// 志向（どのような行動を優先するか）
	struct Oriented
	{
		int stay = 0;			//! 現在のマスから動かない
		int atack = 10;			//! 攻撃性
		int survive = 5;		//! 生き残る
		// int kill = 10;			//! キル
		int friendship = 10;	//! 味方との隣接
		int targetPoint = 10;	//! 制圧 or 防衛
	};

	class EnemyAI
	{
	public:
		EnemyAI() {};
		~EnemyAI() {};

		void init(shared_ptr<Map> map);
		void setOriented(Oriented oriented);

		bool createOrders(shared_ptr<Map> map);
		Order getNextOrder();

		map<pair<int, int>, int> massBaseScoreMap; //! 各マスの基本スコア

	protected:
		constexpr static int FORT_SCORE = 100; //! 砦マスの基本スコア
		constexpr static int FORT_MOVE_SCORE_RATE = 2;	//! 砦マスの波及スコアの係数
		constexpr static int SCORE_BY_FORT = (MAP_MASS_W + MAP_MASS_H - 2) * FORT_MOVE_SCORE_RATE; //! 砦から波及する基本スコアの加算
		constexpr static int SCORE_BY_FRIEND = 2; //! 味方ユニットとの隣接スコア係数

		void setBaseScore(shared_ptr<Map> map);
		void setBaseScoreByFort(shared_ptr<Map> map, int x, int y, int move, std::map<pair<int, int>, int>& tmpBaseScoreMap);
		shared_ptr<Unit> getNextUnit(shared_ptr<Map> map);
		
		deque<Order> orders_; //! 操作手順

		Oriented oriented_; //! AIの志向

	private:
		int getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit);
	};
}