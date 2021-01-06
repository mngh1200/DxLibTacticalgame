#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "Screen/SelectScreen.h"
#include "Screen/MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/UI/Back.h"
#include "Battle/BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Menu/ContextMenu.h"

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
		BattleScreen() : btlMng{}, openScreen_(-1) {};
		~BattleScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		// バトル管理クラス
		Battle::BattleManager btlMng;

		//! 表示対象スクリーン
		int openScreen_;

		shared_ptr<Entity::ContextMenu> systemMenu_;

		// レイヤー
		enum Layer
		{
			MASK,
			CONTEXT_MENU,
			UI,
			PLAYER_UNIT,
			ENEMY_UNIT,
			MAP,
			BACK,
			LEN
		};

		// シーン
		enum Scene
		{
			PLAYER_TURN,
			ENEMY_TURN
		};

		// システムメニューのキー
		enum SystemMenuKey
		{
			TURN_END,
			BACK_SELECT_SCREEN,
			BACK_MENU_SCREEN
		};
	};
}