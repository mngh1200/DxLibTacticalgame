#include "EnemyBattleController.h"

namespace Battle {
	
	/**
	 * @fn
	 * 初期処理
	 * @param (map) マップのポインタ
	 */
	void EnemyBattleController::init(shared_ptr<Map> map)
	{
		map_ = map;
		ai_ = make_unique<EnemyAI>();
	}

	/**
	 * @fn
	 * 更新処理
	 * @param (bm) バトル管理クラスのポインタ
	 * @return 敵ターン終了時にtrueを返す
	 */
	bool EnemyBattleController::update(BattleManager* bm)
	{
		int phase = bm->getPhase();

		// アニメーション中
		if (phase == BattleManager::Phase::FIGHT || phase == BattleManager::Phase::MOVE)
		{
			return false;
		}

		Order order = ai_->getNextOrder(); // 次の操作命令を取得

		if (order.kind == Order::Kind::NONE) // 命令なしの場合
		{
			if (ai_->createOrders(map_)) // 新しい操作命令生成
			{
				order = ai_->getNextOrder();
			}
			else
			{
				return true; // 操作命令がなくなったため終了
			}
		}

		if (order.kind == Order::Kind::MOVE) // 移動
		{
			bm->selectUnit(order.unit);
			bm->moveAction(order.massX, order.massY);
		}
		else if (order.kind == Order::Kind::ATACK) // 攻撃
		{
			bm->setFightPredict(map_->getUnit(order.massX, order.massY));
			bm->atackAction();
		}
		else if (order.kind == Order::Kind::WAIT) // 待機
		{
			bm->waitAction();
		}

		return false;
	}
}