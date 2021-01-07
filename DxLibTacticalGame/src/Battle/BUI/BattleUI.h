#pragma once
#include <climits>
#include <map>
#include "Entity/Unit/Unit.h"
#include "Entity/Battle/Mass.h"
#include "UnitStatusDisplay.h"
#include "TerrainEffectDisplay.h"
#include "FightPredictDisplay.h"

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
			UIID_LEN
		};

		void init();

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
	};


}