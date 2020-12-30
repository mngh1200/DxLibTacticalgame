#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Entity/View/Battle/TerrainEffectDisplay.h"

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
		BattleManager(shared_ptr<Entity::Map> map, int uiLayerId);
		~BattleManager() {};

		enum BattleUi
		{
			TERRAIN_EFFECT_DISPLAY,
			UIID_LEN
		};

		bool setUnit(shared_ptr<Entity::Unit> unit);

		void updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType);

		

		void animationCheck();

	private:
		void onClickUnit(int x, int y);
		void onClickMass(int x, int y);

		weak_ptr<Unit> getUnitWp(int massX, int massY);
		void startSelectActionPhase();
		void endSelectActionPhase();
		void selectUnit(shared_ptr<Unit> unit);
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

		//! 防御側のユニット
		shared_ptr<Entity::Unit> defUnit_;

		//! マップ
		shared_ptr<Entity::Map> map_;

		//! 地形効果表示欄
		shared_ptr<Entity::TerrainEffectDisplay> terrainEffectDisplay_;

		//! フェーズ
		int phase_;

		enum Phase
		{
			NORMAL,
			MOVE,
			SELECT_ACTION,
			ATACK
		};
	};


}