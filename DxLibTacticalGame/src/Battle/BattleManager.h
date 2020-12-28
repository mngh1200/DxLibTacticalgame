#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Menu/SelectActiveMenu.h"

using namespace std;
using namespace Entity;

/**
 * @file TextButton.h
 * @brief テキスト付きボタン要素
 */

namespace Battle
{
	class BattleManager
	{
	public:
		BattleManager() : phase_(Phase::NORMAL) {};
		BattleManager(shared_ptr<Entity::Map> map, shared_ptr<Entity::SelectActiveMenu> selectActiveMenu);
		~BattleManager() {};

		bool setUnit(shared_ptr<Entity::Unit> unit);
		void onClickUnit(int x, int y);
		void onClickMass(int x, int y);
		void onClickActionMenu(int kind);
		void animationCheck();

	private:
		weak_ptr<Unit> getUnitWp(int massX, int massY);
		void startSelectActionPhase();
		void endSelectActionPhase();
		bool deselectUnit();
		void displayMovableRange();
		void searchMovableMass(int x, int y, int move, bool isInit = true);
		void confirmMove(shared_ptr<Unit> unit);
		void displayAtackRange();
		void setAtackMass(int x, int y);
		void atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit);

		

		//! マスのXY座標を基準にしたユニットのマップ
		map<pair<int, int>, weak_ptr<Entity::Unit>> mapUnits_;

		//! 選択中のユニット
		weak_ptr<Entity::Unit> selectedUnit_;

		//! マップ
		shared_ptr<Entity::Map> map_;

		//! 行動選択メニュー
		shared_ptr<Entity::SelectActiveMenu> selectActiveMenu_;

		//! フェーズ
		int phase_;

		enum Phase
		{
			NORMAL,
			MOVE,
			SELECT_ACTION
		};
	};


}