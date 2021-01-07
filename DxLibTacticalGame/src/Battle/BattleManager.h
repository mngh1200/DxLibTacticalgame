#pragma once
#include <climits>
#include <map>
#include "BUI/BattleUI.h"
#include "Fight.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "BUI/BattleUI.h"
#include "Entity/UI/Menu/ContextMenu.h"

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
		BattleManager() : phase_(Phase::NORMAL), battleUI_{} {};
		BattleManager(shared_ptr<Entity::Map> map);
		~BattleManager() {};

		enum BattleUi
		{
			TERRAIN_EFFECT_DISPLAY,
			UIID_LEN
		};

		void updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType);
		void animationCheck();

	private:
		void onClickUnit(shared_ptr<Object> hitObj);
		void onClickMass(int x, int y);

		void startSelectActionPhase();
		void endSelectActionPhase();

		void selectUnit(shared_ptr<Unit> unit);
		bool deselectUnit();

		void atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit);


		//! 選択中のユニット
		shared_ptr<Entity::Unit> selectedUnit_;

		//! マップ
		shared_ptr<Entity::Map> map_;

		//! システムメニュー
		shared_ptr<Entity::ContextMenu> systemMenu_;

		//! バトルUI
		BattleUI battleUI_;

		//! 個人戦闘管理用
		Fight fight_;
		

		//! フェーズ
		int phase_;

		enum Phase
		{
			NORMAL,
			MOVE,
			SELECT_ACTION,
			FIGHT
		};
	};


}