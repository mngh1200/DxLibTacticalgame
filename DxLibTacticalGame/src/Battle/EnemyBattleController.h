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
 * @brief バトル中の敵操作を管理するクラス
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
		//! マップポインタ
		shared_ptr<Map> map_;

		//! 操作判断AI
		unique_ptr<EnemyAI> ai_;
	};
}