#pragma once
#include <climits>
#include "AI/EnemyAI.h"
#include "BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

using namespace std;
using namespace Entity;

/**
 * @file EnemyBattleController.h
 * @brief �o�g�����̓G������Ǘ�����N���X
 */

namespace Battle
{
	class EnemyBattleController
	{
	public:
		EnemyBattleController() {};
		~EnemyBattleController() {};

		void init(shared_ptr<Map> map);

		bool update(BattleManager* bm);
	private:
		//! �}�b�v�|�C���^
		shared_ptr<Map> map_;

		//! ���씻�fAI
		unique_ptr<EnemyAI> ai_;
	};
}