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
#include "Entity/UI/ContextMenu.h"
#include "Entity/UI/Dialog.h"
#include "Network/ReceiveManager.h"
#include "Network/SendManager.h"

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
			stageId_(0), 
			setUnitNum_(0),
			isNetMatch_(false),
			nextScreen_(),
			isFirst_(true),
			isServer_(true)
		{};
		~BattleScreen() {};

		// レイヤー
		enum Layer
		{
			MASK,
			TOP_UI,
			DIALOG_FRAME,
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

		void prepareNetMatch(int netHandler, bool isServer, int mapId, int unitNum, bool isFirst);

	private:
		void execSystemMenuProcess(int systemMenuKey);
		void updateNetwork();
		void startBattle();
		void turnEnd();
		void confirmSetUnits();
		void showHint();
		void showCheckWinText();
		void showResultScene(bool isPlayerWin);

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
		ScreenBase* nextScreen_;

		//! システムメニュー
		shared_ptr<Entity::ContextMenu> systemMenu_;

		// システムメニューのキー
		enum SystemMenuKey
		{
			TURN_END,			//! ターンエンド
			CHECK_WIN_TEXT,		//! 勝敗条件
			HINT,				//! ヒント表示
			BACK_SELECT_SCREEN,	//! セレクト画面へ戻る
			BACK_MENU_SCREEN,	//! メニュー画面へ戻る
			SURRENDER			//! 降参
		};

		// シーンの種類
		enum Scene
		{
			SET_UNITS,		//! ユニット配置
			WAIT_ENEMY_SET, //! 敵側のユニット配置待ち
			PLAYER_TURN,	//! プレイヤーターン
			ENEMY_TURN,		//! 敵ターン
			RESULT_ANIME,	//! 勝敗結果前のアニメーション
			RESULT,			//! 勝敗結果
			NETWORK_CLOSE	//! 相手から通信切断されたとき
		};

		// 以下、通信対戦関連

		//! 通信対戦であるか
		bool isNetMatch_;

		//! サーバー側であるか
		bool isServer_;

		//! 先攻/後攻
		bool isFirst_;

		//! 受信管理
		shared_ptr<Network::SendManager> sender_;

		//! 送信管理
		Network::ReceiveManager receiver_;

		//! ダイアログ
		Dialog dialog_;
	};
}