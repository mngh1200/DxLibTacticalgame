#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

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
		BattleManager() {};
		BattleManager(shared_ptr<Entity::Map> map);
		~BattleManager() {};

		bool setUnit(shared_ptr<Entity::Unit> unit);
		void onClickPlayerUnit(int x, int y);
		void onClickMass(int x, int y);

	private:
		weak_ptr<Unit> getUnitWp(int massX, int massY);
		bool deselectUnit();

		//! �}�X��XY���W����ɂ������j�b�g�̃}�b�v
		map<pair<int, int>, weak_ptr<Entity::Unit>> mapUnits_;

		//! �I�𒆂̃��j�b�g
		weak_ptr<Entity::Unit> selectedUnit_;

		shared_ptr<Entity::Map> map_;
	};


}