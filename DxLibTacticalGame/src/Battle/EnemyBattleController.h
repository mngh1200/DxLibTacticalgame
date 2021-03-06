#pragma once
#include <climits>
#include "AI/EnemyAI.h"
#include "BattleManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Utility/Timer.h"
#include "Battle/BUI/EnemyTurnCont.h"

using namespace std;
using namespace Entity;

/**
 * @file EnemyBattleController.h
 * @brief バトル中の敵操作を管理するクラス
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
	private:
		// AIの種類
		enum AiKind
		{
			NORMAL,				//! 標準
			SELF_DEFENCE_ONLY,	//! 攻撃してこない
			AGGRESSIVE,			//! 攻撃的
			AI_KIND_LEN
		};

		// タイマーイベント
		enum class TimerEvent
		{
			TIMER_EVENT_NONE,
			ATACK
		};

		void setTimerEvent(int ms, TimerEvent kind);

		//! マップポインタ
		shared_ptr<Map> map_;

		//! 操作判断AI
		unique_ptr<EnemyAI> ai_;

		//! タイマー
		Utility::Timer timer_;

		//! タイマー終了時にどのイベントを実施するか
		TimerEvent timerEvent_;
	};
}