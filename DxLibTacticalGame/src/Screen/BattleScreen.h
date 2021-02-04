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
#include "Entity/Battle/ResultScene.h"
#include "Battle/BattleManager.h"
#include "Battle/PlayerBattleController.h"
#include "Battle/EnemyBattleController.h"
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
		BattleScreen() : btlMng_{}, playerBtlCont_{},  openScreen_(-1), stageId_(0) {};
		~BattleScreen() {};

		// レイヤー
		enum Layer
		{
			MASK,
			TOP_UI,
			CONTEXT_MENU,
			EFFECT,
			UI,
			PLAYER_UNIT,
			ENEMY_UNIT,
			MAP,
			BACK,
			LEN
		};

		enum TopUiId
		{
			MESSAGE,
			RESULT_SCENE
		};

		void init() override;
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType) override;
		void updateByAnimation() override;

		void setStage(int id);

	private:
		void turnEnd();

		// バトル管理クラス
		Battle::BattleManager btlMng_;

		// バトル画面上のプレイヤー操作管理クラス
		Battle::PlayerBattleController playerBtlCont_;

		// 敵ユニット操作クラス
		Battle::EnemyBattleController enemyBtlCont_;

		//! 選択ステージのID
		int stageId_;

		//! 表示対象スクリーン
		int openScreen_;

		//! システムメニュー
		shared_ptr<Entity::ContextMenu> systemMenu_;

		// システムメニューのキー
		enum SystemMenuKey
		{
			TURN_END,
			BACK_SELECT_SCREEN,
			BACK_MENU_SCREEN
		};

		// シーンの種類
		enum Scene
		{
			PLAYER_TURN,
			ENEMY_TURN,
			RESULT_ANIME,
			RESULT
		};
	};
}