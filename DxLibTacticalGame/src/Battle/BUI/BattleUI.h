#pragma once
#include <climits>
#include <map>
#include "Entity/Unit/Unit.h"
#include "Entity/Battle/Mass.h"
#include "UnitStatusDisplay.h"
#include "TerrainEffectDisplay.h"

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
			BATTLE_PREDICT_PLAYER,
			BATTLE_PREDICT_ENEMY,
			UIID_LEN
		};

		void init(int uiLayerId);

		void setTargetUnit(shared_ptr<Unit> unit);
		void resetTargetUnit();

		void setTargetMass(shared_ptr<Mass> mass);
		void resetTargetMass();

		void updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType);

		void animationCheck();

	private:
		//! �\�����[�h�̎��
		enum class Mode
		{
			NORMAL,
			BATTLE_PREDICT,
			ENEMY_TURN
		};

		//! �\�����[�h
		Mode mode_;

		//! �n�`���ʕ\����
		shared_ptr<TerrainEffectDisplay> terrainEffectDisplay_;

		//! �X�e�[�^�X�\����
		shared_ptr<UnitStatusDisplay> unitStatusDisplay_;
	};


}