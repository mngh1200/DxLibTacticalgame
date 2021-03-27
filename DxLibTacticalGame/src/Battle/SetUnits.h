#pragma once
#include <climits>
#include "Battle/BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Network/SendManager.h"
#include "Network/ReceiveManager.h"

using namespace std;
using namespace Entity;

/**
 * @file SetUnits.h
 * @brief ユニット配置処理用の関数を管理
 */

namespace SetUnits
{
	void onClick(int x, int y, shared_ptr<Map> map, Battle::BattleUI* battleUI, Battle::TutorialManager* tutorial, Battle::BattleManager* bm);

	void sendSetUnitsData(shared_ptr<Network::SendManager> sender, shared_ptr<Map> map);

	void receiveSetUnitsData(Network::ReceiveManager* receiver, shared_ptr<Map> map);
}