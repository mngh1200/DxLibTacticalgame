#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Battle/BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

using namespace std;

/**
 * @file BattleScreen.h
 * @brief バトル画面
 */

namespace Screen
{
	class BattleScreen : public ScreenBase
	{
	public:
		BattleScreen() : btlMng{} {};
		~BattleScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		Battle::BattleManager btlMng;

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