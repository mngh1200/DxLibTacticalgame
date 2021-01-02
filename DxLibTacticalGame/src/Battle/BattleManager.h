#pragma once
#include <climits>
#include <map>
#include "BUI/BattleUI.h"
#include "Fight.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "BUI/TerrainEffectDisplay.h"

using namespace std;
using namespace Entity;

/**
 * @file TextButton.h
 * @brief �e�L�X�g�t���{�^���v�f
 */

namespace Battle
{
	class BattleManager
	{
	public:
		BattleManager() : phase_(Phase::NORMAL), battleUI_{} {};
		BattleManager(shared_ptr<Entity::Map> map, int uiLayerId);
		~BattleManager() {};

		enum BattleUi
		{
			TERRAIN_EFFECT_DISPLAY,
			UIID_LEN
		};

		void updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType);
		void animationCheck();

	private:
		void onClickUnit(int x, int y);
		void onClickMass(int x, int y);

		void startSelectActionPhase();
		void endSelectActionPhase();

		void selectUnit(shared_ptr<Unit> unit);
		bool deselectUnit();

		void atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit);


		//! �I�𒆂̃��j�b�g
		weak_ptr<Entity::Unit> selectedUnit_;

		//! �}�b�v
		shared_ptr<Entity::Map> map_;

		//! �o�g��UI
		BattleUI battleUI_;

		//! �l�퓬�Ǘ��p
		Fight fight_;

		//! �t�F�[�Y
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