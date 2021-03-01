#pragma once
#include <climits>
#include <map>
#include "BUI/BattleUI.h"
#include "Fight.h"
#include "CheckWin.h"
#include "Battle/TutorialManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "BUI/BattleUI.h"
#include "Entity/UI/Message.h"

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
		BattleManager() :
			isPlayerTurn_(true),
			phase_(Phase::NORMAL),
			turnNumEach_(0),
			checkWin_{},
			battleUI{},
			tutorial{}
		{};
		~BattleManager() {};

		// 状態遷移状況
		enum Phase
		{
			NORMAL,
			MOVE,
			SELECT_ACTION,
			FIGHT
		};

		void init(shared_ptr<Entity::Map> map, int stageId, bool* isSetUnit, int* aiKind);

		void animationCheck();
		int checkEnd();

		void onStartTurn(bool isPlayer);

		void startSelectActionPhase();
		void endSelectActionPhase();

		void selectUnit(shared_ptr<Unit> unit);
		bool deselectUnit();

		void atackAction();
		void waitAction();
		void moveAction(int massX, int massY, bool isMoveImmdiateConfirm = false);
		void moveCancel();

		void setFightPredict(shared_ptr<Unit> targetUnit);
		void resetFightPredict();

		bool isPlayerTurn() const { return isPlayerTurn_; } // プレイヤーであるかを返す
		int getPhase() const { return phase_; }; // 状況を返す
		bool isAnimation() const { return phase_ == Phase::FIGHT || phase_ == Phase::MOVE; }; // アニメーション中であるかを返す
		bool isAtackAble(shared_ptr<Unit> targetUnit) const;
		bool isSelectedUnitActive() const;
		bool isSelectedUnit(shared_ptr<Unit> unit) const;
		int getNowTurn() const { return turnNumEach_ / 2 + 1; }; // 現在ターンを返す
		const CheckWin& getCheckWin() const { return checkWin_; }; // 勝敗判定用クラスを返す
		const shared_ptr<Unit> getSelectedUnit() const { return selectedUnit_; } // 選択中のユニットを返す

		//! バトルUI
		BattleUI battleUI;

		//! チュートリアル管理クラス
		TutorialManager tutorial;

		//! メッセージ
		shared_ptr<Entity::Message> message;

		//! マップ
		shared_ptr<Entity::Map> map;

	private:
		//! 経過ターン数(プレイヤーターン、敵ターンでそれぞれ＋１にする、表示ターンの二倍で表示)
		int turnNumEach_;

		//! 選択中のユニット
		shared_ptr<Entity::Unit> selectedUnit_;

		//! 個人戦闘管理用
		Fight fight_;

		//! フェーズ
		int phase_;

		//! 敵ターンであるか判定
		bool isPlayerTurn_;

		//! 勝者
		CheckWin checkWin_;

		//! 移動を即確定させるか
		bool isMoveImmdiateConfirm_;
	};


}