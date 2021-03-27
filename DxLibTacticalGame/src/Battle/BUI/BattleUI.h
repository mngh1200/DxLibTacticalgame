#pragma once
#include <climits>
#include <map>
#include "Entity/Unit/Unit.h"
#include "Entity/Battle/Mass.h"
#include "UnitStatusDisplay.h"
#include "TerrainEffectDisplay.h"
#include "FightPredictDisplay.h"
#include "EnemyTurnCont.h"
#include "SelectUnitArea.h"
#include "Entity/UI/Button/BuiConfirmButton.h"

using namespace std;
using namespace Entity;

/**
 * @file BattleUI.h
 * @brief バトルUI管理用クラス
 */

namespace Battle
{
	class BattleUI
	{
	public:
		BattleUI() : mode_(Mode::NORMAL) {};
		~BattleUI() {};
		
		// オブジェクトID
		enum BattleUIid
		{
			TERRAIN_EFFECT,
			UNIT_STATUS,
			FIGHT_PREDICT,
			TURN_END_BUTTON,
			ENEMY_SPEED_CONTROLLER,
			ENEMY_TURN_CONT,
			SELECT_UNIT_AREA,
			CONFIRM_UNIT_SET,
			UIID_LEN
		};

		void init();

		void startSelectUnitMode(int setUnitNum);
		void endSelectUnitMode();
		bool addSetUnitCount();
		void removeSetUnitCount();
		int getSelectedUnitId() const;

		void onStartTurn(bool isPlayer, bool isNetMatch = false);

		void startWaitEnemySet();
		void endWaitEnemySet();

		void setTargetUnit(shared_ptr<Unit> unit);
		void resetTargetUnit();

		void setTargetMass(shared_ptr<Mass> mass);
		void resetTargetMass();

		void setFightPredict(const Fight* fight);
		void resetFightPredict();

		void updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType);

		void animationCheck();

	private:
		//! 表示モードの種類
		enum class Mode
		{
			SET_UNITS,
			NORMAL,
			FIGHT_PREDICT,
			ENEMY_TURN
		};

		//! 表示モード
		Mode mode_;

		//! 地形効果表示欄
		shared_ptr<TerrainEffectDisplay> terrainEffectDisplay_;

		//! ステータス表示欄
		shared_ptr<UnitStatusDisplay> unitStatusDisplay_;

		//! 戦闘予測欄
		shared_ptr<FightPredictDisplay> fightPredictDisplay_;

		//! ターン終了ボタン
		shared_ptr<BuiConfirmButton> turnEndButton_;

		//! 敵ターン中の右下のオブジェクト
		shared_ptr<EnemyTurnCont> enemyTurnCont_;

		//! ユニット選択欄
		shared_ptr<SelectUnitArea> selectUnitArea_;

		//! ユニット配置確定ボタン
		shared_ptr<BuiConfirmButton> confirmUnitSetButton_;
	};


}