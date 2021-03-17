#pragma once
#include <climits>
#include <memory>
#include <string>
#include "ScreenBase.h"
#include "Screen/SelectScreen.h"
#include "Screen/MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Utility/SaveManager.h"
#include "Entity/UI/Back.h"
#include "Entity/Battle/ResultScene.h"
#include "Battle/BattleManager.h"
#include "Battle/PlayerBattleController.h"
#include "Battle/EnemyBattleController.h"
#include "Battle/SetUnits.h"
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
		BattleScreen() : 
			btlMng_{},
			playerBtlCont_{},
			openScreen_(-1),
			stageId_(0), 
			setUnitNum_(0),
			netHandler_(-1),
			isServer_(false)
		{};
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

		// 上位UIレイヤーのオブジェクトID
		enum TopUiId
		{
			MESSAGE,
			RESULT_SCENE
		};

		void init() override;
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType) override;
		void updateByAnimation() override;

		void setStage(int id);

		void prepareNetMatch(int netHandler, bool isServer, int mapId, int unitNum);

	private:
		void startBattle();
		void turnEnd();
		void confirmSetUnits();
		void showHint();
		void showCheckWinText();

		// バトル管理クラス
		Battle::BattleManager btlMng_;

		// バトル画面上のプレイヤー操作管理クラス
		Battle::PlayerBattleController playerBtlCont_;

		// 敵ユニット操作クラス
		Battle::EnemyBattleController enemyBtlCont_;

		// 配置可能ユニット数
		int setUnitNum_;

		//! 選択ステージのID
		int stageId_;

		//! 表示対象スクリーン
		int openScreen_;

		//! ネットハンドル（通信対戦出ないときは -1）
		int netHandler_;

		//! サーバー側か
		bool isServer_;

		//! システムメニュー
		shared_ptr<Entity::ContextMenu> systemMenu_;

		// システムメニューのキー
		enum SystemMenuKey
		{
			TURN_END,			//! ターンエンド
			CHECK_WIN_TEXT,		//! 勝敗条件
			HINT,				//! ヒント表示
			BACK_SELECT_SCREEN,	//! セレクト画面へ戻る
			BACK_MENU_SCREEN	//! メニュー画面へ戻る
		};

		// シーンの種類
		enum Scene
		{
			SET_UNITS,		//! ユニット配置
			WAIT_ENEMY_SET, //! 敵側のユニット配置待ち
			PLAYER_TURN,	//! プレイヤーターン
			ENEMY_TURN,		//! 敵ターン
			RESULT_ANIME,	//! 勝敗結果前のアニメーション
			RESULT			//! 勝敗結果
		};
	};
}