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
		int kind = ActionKind::NO_ACTION; //! ����̎��
		shared_ptr<Unit> unit; //! ����Ώۃ��j�b�g
		int massX = 0; //! �Ώۃ}�XX
		int massY = 0; //! �Ώۃ}�XY
	};

	// �u���i�ǂ̂悤�ȍs����D�悷�邩�j
	struct Oriented
	{
		int stay = 0;			//! ���݂̃}�X���瓮���Ȃ�
		int atack = 10;			//! �U����
		int survive = 5;		//! �����c��
		// int kill = 10;			//! �L��
		int friendship = 10;	//! �����Ƃ̗א�
		int targetPoint = 10;	//! ���� or �h�q
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

		map<pair<int, int>, int> massBaseScoreMap; //! �e�}�X�̊�{�X�R�A

	protected:
		constexpr static int FORT_SCORE = 100; //! �ԃ}�X�̊�{�X�R�A
		constexpr static int FORT_MOVE_SCORE_RATE = 2;	//! �ԃ}�X�̔g�y�X�R�A�̌W��
		constexpr static int SCORE_BY_FORT = (MAP_MASS_W + MAP_MASS_H - 2) * FORT_MOVE_SCORE_RATE; //! �Ԃ���g�y�����{�X�R�A�̉��Z
		constexpr static int SCORE_BY_FRIEND = 2; //! �������j�b�g�Ƃ̗אڃX�R�A�W��

		void setBaseScore(shared_ptr<Map> map);
		void setBaseScoreByFort(shared_ptr<Map> map, int x, int y, int move, std::map<pair<int, int>, int>& tmpBaseScoreMap);
		shared_ptr<Unit> getNextUnit(shared_ptr<Map> map);
		
		deque<Order> orders_; //! ����菇

		Oriented oriented_; //! AI�̎u��

	private:
		int getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit);
	};
}