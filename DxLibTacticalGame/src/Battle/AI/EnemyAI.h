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
 * @brief �G�̑��씻�fAI�N���X
 */

namespace Battle
{
	// ������e��ێ�
	struct Order
	{
		// ����̎��
		enum Kind
		{
			NONE,
			MOVE,
			WAIT,
			ATACK
		};

		int kind = Kind::NONE; //! ����̎��
		shared_ptr<Unit> unit; //! ����Ώۃ��j�b�g
		int massX = 0; //! �Ώۃ}�XX
		int massY = 0; //! �Ώۃ}�XY
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
		
		deque<Order> orders_; //! ����菇

	private:
		int getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit);
	};
}