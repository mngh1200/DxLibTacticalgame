#pragma once
#include <climits>
#include <map>
#include "BUI/BattleUI.h"
#include "Fight.h"
#include "CheckWin.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "BUI/BattleUI.h"

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
			isPlayerTurn_(true), phase_(Phase::NORMAL), 
			checkWin_{}, battleUI{} {};
		~BattleManager() {};

		// 状態遷移状況
		enum Phase
		{
			NORMAL,
			MOVE,
			SELECT_ACTION,
			FIGHT
		};

		void init(shared_ptr<Map> map);

		void animationCheck();
		bool checkEnd();

		void onStartTurn(bool isPlayer);

		void startSelectActionPhase();
		void endSelectActionPhase();

		void selectUnit(shared_ptr<Unit> unit);
		bool deselectUnit();

		void atackAction();
		void waitAction();
		void moveAction(int massX, int massY);
		void moveCancel();

		void setFightPredict(shared_ptr<Unit> targetUnit);
		void resetFightPredict();

		int getPhase() const { return phase_; }; // 状況を返す
		bool isAnimation() const { return phase_ == Phase::FIGHT || phase_ == Phase::MOVE; }; // アニメーション中であるかを返す
		bool isAtackAble(shared_ptr<Unit> targetUnit) const;
		bool isSelectedUnitActive() const;
		bool isSelectedUnit(shared_ptr<Unit> unit) const;

		//! バトルUI
		BattleUI battleUI;

	private:
		//! 選択中のユニット
		shared_ptr<Entity::Unit> selectedUnit_;

		//! マップ
		shared_ptr<Entity::Map> map_;

		//! 個人戦闘管理用
		Fight fight_;

		//! フェーズ
		int phase_;

		//! 敵ターンであるか判定
		bool isPlayerTurn_;

		//! 勝者
		CheckWin checkWin_;

		// 勝者の値
		enum Winner
		{
			UNDECIDED,
			PLAYER,
			ENEMY
		};
	};


}