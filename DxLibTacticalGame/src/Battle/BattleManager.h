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
 * @brief �e�L�X�g�t���{�^���v�f
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

		// ��ԑJ�ڏ�
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

		int getPhase() const { return phase_; }; // �󋵂�Ԃ�
		bool isAnimation() const { return phase_ == Phase::FIGHT || phase_ == Phase::MOVE; }; // �A�j���[�V�������ł��邩��Ԃ�
		bool isAtackAble(shared_ptr<Unit> targetUnit) const;
		bool isSelectedUnitActive() const;
		bool isSelectedUnit(shared_ptr<Unit> unit) const;

		//! �o�g��UI
		BattleUI battleUI;

	private:
		//! �I�𒆂̃��j�b�g
		shared_ptr<Entity::Unit> selectedUnit_;

		//! �}�b�v
		shared_ptr<Entity::Map> map_;

		//! �l�퓬�Ǘ��p
		Fight fight_;

		//! �t�F�[�Y
		int phase_;

		//! �G�^�[���ł��邩����
		bool isPlayerTurn_;

		//! ����
		CheckWin checkWin_;

		// ���҂̒l
		enum Winner
		{
			UNDECIDED,
			PLAYER,
			ENEMY
		};
	};


}