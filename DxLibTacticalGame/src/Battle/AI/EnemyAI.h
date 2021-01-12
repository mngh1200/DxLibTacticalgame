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
 * @brief GÌì»fAINX
 */

namespace Battle
{
	// ìàeðÛ
	struct Order
	{
		// ìÌíÞ
		enum Kind
		{
			NONE,
			MOVE,
			WAIT,
			ATACK
		};

		int kind = Kind::NONE; //! ìÌíÞ
		shared_ptr<Unit> unit; //! ìÎÛjbg
		int massX = 0; //! ÎÛ}XX
		int massY = 0; //! ÎÛ}XY
	};

	class EnemyAI
	{
	public:
		EnemyAI() {};
		~EnemyAI() {};

		bool createOrders(shared_ptr<Map> map);
		Order getNextOrder();

	protected:
		shared_ptr<Unit> getNextUnit(shared_ptr<Map> map);
		
		deque<Order> orders_; //! ìè

	private:
		int getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit);
	};
}