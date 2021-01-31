#include "EnemyBattleController.h"

namespace Battle {
	float EnemyBattleController::timerRate = 1.0f;

	/**
	 * @fn
	 * 初期処理
	 * @param (map) マップのポインタ
	 */
	void EnemyBattleController::init(shared_ptr<Map> map)
	{
		map_ = map;
		ai_ = make_unique<EnemyAI>();
		ai_->init(map);
	}

	/**
	 * @fn
	 * 更新処理
	 * @param (bm) バトル管理クラスのポインタ
	 * @return 敵ターン終了時にtrueを返す
	 */
	bool EnemyBattleController::update(BattleManager* bm)
	{
		// テスト処理
		/*
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		for (auto itr = ai_->massBaseScoreMap.begin(); itr != ai_->massBaseScoreMap.end(); ++itr)
		{
			int x = itr->first.first;
			int y = itr->first.second;
			int score = itr->second;

			DxLib::DrawStringFToHandle(x * CHIP_SIZE, y * CHIP_SIZE, to_string(score).c_str(), 0xff0000, rm.getHdlFont(FontType::NORMAL_S32));
		}
		DxLib::ScreenFlip();
		*/

		int phase = bm->getPhase();

		// アニメーション中
		if (phase == BattleManager::Phase::FIGHT || phase == BattleManager::Phase::MOVE)
		{
			return false;
		}

		// タイマーイベントチェック
		if (timerEvent_ != TimerEvent::NONE)
		{
			if (timer_.checkAndCountTimer())
			{
				if (timerEvent_ == TimerEvent::ATACK) // 攻撃
				{
					bm->atackAction();
				}
				timerEvent_ = TimerEvent::NONE;
			}
			return false; // タイマーイベント中は操作命令の進行はしない
		}


		// 操作命令チェック
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
			setTimerEvent(1500, TimerEvent::ATACK); // 戦闘予測出してから一定時間後に攻撃実行
			
		}
		else if (order.kind == Order::Kind::WAIT) // 待機
		{
			bm->waitAction();
		}

		return false;
	}

	/**
	 * @fn
	 * タイマーイベント追加
	 * @param (ms) セット時間(ms)
	 * @param (kind) イベントの種類
	 */
	void EnemyBattleController::setTimerEvent(int ms, TimerEvent kind)
	{
		timer_.setTimer((int)(ms * timerRate));
		timerEvent_ = kind;
	}
}