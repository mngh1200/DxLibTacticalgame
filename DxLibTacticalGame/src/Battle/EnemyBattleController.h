#pragma once
#include <climits>
#include "AI/EnemyAI.h"
#include "BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Utility/Timer.h"

using namespace std;
using namespace Entity;

/**
 * @file EnemyBattleController.h
 * @brief �o�g�����̓G������Ǘ�����N���X
 */

namespace Battle
{
	class EnemyBattleController
	{
	public:
		EnemyBattleController() : timerEvent_(TimerEvent::TIMER_EVENT_NONE), timer_{} {};
		~EnemyBattleController() {};

		void init(shared_ptr<Map> map, int aiKind);

		bool update(BattleManager* bm);

		static float timerRate; //! �^�C�}�[���Ԃ̒����̔{���i���x�����p�j
	private:
		// AI�̎��
		enum AiKind
		{
			NORMAL,				//! �W��
			SELF_DEFENCE_ONLY,	//! �U�����Ă��Ȃ�
			AI_KIND_LEN
		};

		// �^�C�}�[�C�x���g
		enum class TimerEvent
		{
			TIMER_EVENT_NONE,
			ATACK
		};

		void setTimerEvent(int ms, TimerEvent kind);

		//! �}�b�v�|�C���^
		shared_ptr<Map> map_;

		//! ���씻�fAI
		unique_ptr<EnemyAI> ai_;

		//! �^�C�}�[
		Utility::Timer timer_;

		//! �^�C�}�[�I�����ɂǂ̃C�x���g�����{���邩
		TimerEvent timerEvent_;
	};
}