#include "PlayerBattleController.h"

namespace Battle {
	
	/**
	 * @fn
	 * 初期処理
	 * @param (map) マップのポインタ
	 */
	void PlayerBattleController::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * イベント処理
	 */
	void PlayerBattleController::updateByEvents(BattleManager* bm, shared_ptr<Object> hitObj, int x, int y, int button, int* eventType)
	{
		if (*eventType == MOUSE_INPUT_LOG_CLICK && !bm->isAnimation()) // クリック かつ　アニメーション中ではない
		{
			if (hitObj->getType() == Object::Type::UNIT)
			{
				onClickUnit(bm, hitObj);
			}
			else if (hitObj->getType() == Object::Type::MAP)
			{
				onClickMass(bm, x, y);
			}
		}
		else if (*eventType == MOUSE_INPUT_LOG_UP && button == MOUSE_INPUT_RIGHT && !bm->isAnimation()) // マウス右クリック（厳密にはボタンアップ）
		{
			if (hitObj->getType() == Object::Type::MAP) // マスが対象
			{
				if (checkMoveConfirm(bm, x, y))
				{
					*eventType = MOUSE_INPUT_LOG_USED;
				}
			}
			else if (hitObj->getType() == Object::Type::UNIT) // ユニットが対象
			{
				if (onRightClickUnit(bm, hitObj))
				{
					*eventType = MOUSE_INPUT_LOG_USED;
				}
			}
		}

		if (hitObj->getType() == Object::Type::UNIT || hitObj->getType() == Object::Type::MAP)
		{
			// ホバー時処理
			onHoverObject(bm, x, y);
		}
		else
		{
			// 戦闘予測解除
			bm->resetFightPredict();
		}

		bm->battleUI.updateByEvents(hitObj, x, y, button, *eventType);

	}

	/**
	 * @fn
	 * マップ要素ホバー時の処理
	 */
	void PlayerBattleController::onHoverObject(BattleManager* bm, int x, int y)
	{
		// 1マップ上の要素をホバー
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		shared_ptr<Mass> mass = map_->getMass(massX, massY);
		shared_ptr<Unit> unit = map_->getUnit(massX, massY);

		// 前回と同じユニットをホバーしている場合は処理を実施しない
		if (mass == hoverMass_ && unit == hoverUnit_)
		{
			return;
		}

		if (mass)
		{
			if (bm->isAtackAble(unit)) // 攻撃可能ユニットを指定している時
			{
				// 戦闘予測
				bm->setFightPredict(unit);
				bm->tutorial.onEvent(TutorialManager::TutorialId::ATACK, bm);
			}
			else
			{
				// 戦闘予測解除
				bm->resetFightPredict();

				// マスデータ表示
				bm->battleUI.setTargetMass(mass);
			}
		}
	}

	/**
	 * @fn
	 * ユニット クリック時処理
	 * @param (bm) バトル管理クラス
	 * @param (hitObj) 対象要素
	 */
	void PlayerBattleController::onClickUnit(BattleManager* bm, shared_ptr<Object> hitObj)
	{
		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(hitObj);
		bool isOwn = !unit->isEnemy(); // 味方ユニットであるか
		

		if (bm->isAtackAble(unit)) // 攻撃対象のユニットクリック
		{
			bm->atackAction(); // 攻撃アクション

		}
		else if (bm->getPhase() == BattleManager::Phase::SELECT_ACTION) // 行動選択
		{
			if (bm->isSelectedUnit(unit)) // 選択中のユニットクリック
			{
				bm->waitAction(); // 待機アクション
				bm->tutorial.onEvent(TutorialManager::TutorialId::TURN_END, bm);
				Utility::ResourceManager::playSound(SoundKind::WAIT);
			}
			else
			{
				bm->endSelectActionPhase(); // 行動選択終了
				bm->selectUnit(unit); // 他のユニット選択
			}
		}
		else
		{
			if (bm->isSelectedUnit(unit))
			{
				if (isOwn)
				{
					// クリックしたユニットが 選択中のユニットだった場合、行動選択フェイズに移行
					bm->startSelectActionPhase();
				}
			}
			else
			{
				bm->selectUnit(unit); // ユニット選択
				bm->tutorial.onEvent(TutorialManager::TutorialId::MOVE, bm);
				Utility::ResourceManager::playSound(SoundKind::SELECT_UNIT);
			}
		}
	}

	/**
	 * @fn
	 * ユニット 右クリック時処理
	 * @param (bm) バトル管理クラス
	 * @param (hitObj) 対象要素
	 * @return 右クリックによる処理が実行される場合はtrueを返す
	 */
	bool PlayerBattleController::onRightClickUnit(BattleManager* bm, shared_ptr<Object> hitObj)
	{
		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(hitObj);


		if (bm->isAtackAble(unit)) // 攻撃対象のユニットクリック
		{
			bm->atackAction(); // 攻撃アクション
			return true;

		}
		else if (bm->getPhase() == BattleManager::Phase::SELECT_ACTION) // 行動選択
		{
			if (bm->isSelectedUnit(unit)) // 選択中のユニットクリック
			{
				bm->waitAction(); // 待機アクション
				bm->tutorial.onEvent(TutorialManager::TutorialId::TURN_END, bm);
				Utility::ResourceManager::playSound(SoundKind::WAIT);
				return true;
			}
		}

		return false;
	}

	/**
	 * @fn
	 * マス クリック時処理
	 * @param (bm) バトル管理クラス
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void PlayerBattleController::onClickMass(BattleManager* bm, int x, int y)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		if (bm->isSelectedUnitActive())
		{

			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (bm->getPhase() == BattleManager::Phase::SELECT_ACTION ) // 行動選択
			{
				bm->moveCancel(); // 移動キャンセル
			}
			else if (targetMass->isMovable())
			{
				bm->moveAction(massX, massY); //移動
			}
			else
			{
				bm->deselectUnit(); // 選択解除
			}
		}
		else
		{
			bm->deselectUnit(); // 選択解除
		}
	}

	/**
	 * @fn
	 * マス 右クリック時処理
	 * @param (bm) バトル管理クラス
	 * @param (x) x座標
	 * @param (y) y座標
	 * @return 移動、移動確定を実行した場合 trueを返す
	 */
	bool PlayerBattleController::checkMoveConfirm(BattleManager* bm, int x, int y)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		if (bm->isSelectedUnitActive())
		{
			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);

			if (bm->getPhase() != BattleManager::Phase::SELECT_ACTION && targetMass->isMovable())
			{
				bm->moveAction(massX, massY, true); //移動 & 移動確定
				Utility::ResourceManager::playSound(SoundKind::WAIT);
				return true;
			}
		}
		return false;
	}
}