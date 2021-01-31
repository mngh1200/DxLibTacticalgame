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

		void init(shared_ptr<Map> map);
		bool createOrders(shared_ptr<Map> map);
		Order getNextOrder();

		map<pair<int, int>, int> massBaseScoreMap; //! �e�}�X�̊�{�X�R�A

	protected:
		constexpr static int SCORE_BY_FORT = MAP_MASS_W + MAP_MASS_H - 2; // �Ԃ���g�y�����{�X�R�A�̉��Z

		void setBaseScore(shared_ptr<Map> map);
		void setBaseScoreByFort(shared_ptr<Map> map, int x, int y, int move, std::map<pair<int, int>, int>& tmpBaseScoreMap);
		shared_ptr<Unit> getNextUnit(shared_ptr<Map> map);
		
		deque<Order> orders_; //! ����菇

	private:
		int getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit);
	};
}