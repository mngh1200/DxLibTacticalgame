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
 * @brief �o�g��UI�Ǘ��p�N���X
 */

namespace Battle
{
	class BattleUI
	{
	public:
		BattleUI() : mode_(Mode::NORMAL) {};
		~BattleUI() {};
		
		// �I�u�W�F�N�gID
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
		//! �\�����[�h�̎��
		enum class Mode
		{
			SET_UNITS,
			NORMAL,
			FIGHT_PREDICT,
			ENEMY_TURN
		};

		//! �\�����[�h
		Mode mode_;

		//! �n�`���ʕ\����
		shared_ptr<TerrainEffectDisplay> terrainEffectDisplay_;

		//! �X�e�[�^�X�\����
		shared_ptr<UnitStatusDisplay> unitStatusDisplay_;

		//! �퓬�\����
		shared_ptr<FightPredictDisplay> fightPredictDisplay_;

		//! �^�[���I���{�^��
		shared_ptr<BuiConfirmButton> turnEndButton_;

		//! �G�^�[�����̉E���̃I�u�W�F�N�g
		shared_ptr<EnemyTurnCont> enemyTurnCont_;

		//! ���j�b�g�I��
		shared_ptr<SelectUnitArea> selectUnitArea_;

		//! ���j�b�g�z�u�m��{�^��
		shared_ptr<BuiConfirmButton> confirmUnitSetButton_;
	};


}