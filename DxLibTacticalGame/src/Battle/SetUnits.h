#pragma once
#include <climits>
#include "Battle/BattleManager.h"
#include "Entity/Battle/Map.h"

using namespace std;
using namespace Entity;

/**
 * @file SetUnits.h
 * @brief ユニット配置処理用の関数を管理
 */

namespace SetUnits
{
	void onClick(int x, int y, shared_ptr<Map> map, Battle::BattleUI* battleUI);
}