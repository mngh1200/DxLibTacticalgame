#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

using namespace std;

/**
 * @file BattleScreen.h
 * @brief �o�g�����
 */

namespace Screen
{
	class BattleScreen : public ScreenBase
	{
	public:
		BattleScreen() {};
		~BattleScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		bool deselectUnit();

		//! �I�𒆂̃��j�b�g
		weak_ptr<Entity::Unit> selectedUnit_;

		enum Layer
		{
			MASK,
			UI,
			PLAYER_UNIT,
			ENEMY_UNIT,
			MAP,
			LEN
		};

		enum UIid
		{
			UIID_LEN
		};

		enum Scene
		{
			PLAYER_TURN,
			ENEMY_TURN
		};
	};
}