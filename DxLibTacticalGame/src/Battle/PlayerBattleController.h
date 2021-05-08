#pragma once
#include <climits>
#include "BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

using namespace std;
using namespace Entity;

/**
 * @file PlayerBattleController.h
 * @brief バトル中のプレイヤー操作を管理するクラス
 */

namespace Battle
{
	class PlayerBattleController
	{
	public:
		PlayerBattleController() {};
		~PlayerBattleController() {};

		void init(shared_ptr<Map> map);

		void updateByEvents(BattleManager* bm, shared_ptr<Object> hitObj, int x, int y, int button, int* eventType);

	private:
		void onClickUnit(BattleManager* bm, shared_ptr<Object> hitObj);
		bool onRightClickUnit(BattleManager* bm, shared_ptr<Object> hitObj);
		void onClickMass(BattleManager* bm, int x, int y);
		bool checkMoveConfirm(BattleManager* bm, int x, int y);
		void onHoverObject(BattleManager* bm, int x, int y);

		shared_ptr<Map> map_;

		shared_ptr<Unit> hoverUnit_;
		shared_ptr<Mass> hoverMass_;
	};
}