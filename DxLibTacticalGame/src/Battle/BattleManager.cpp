#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * コンストラクタ
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map, int uiLayerId) : BattleManager()
	{
		map_ = map;
		battleUI_.init(uiLayerId);
		fight_.init(map);
	}

	/**
	 * @fn
	 * イベント処理
	 */
	void BattleManager::updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType)
	{
		if (eventType == MOUSE_INPUT_LOG_CLICK) // クリック
		{
			if (hitObj->getType() == Object::Type::UNIT)
			{
				onClickUnit(x, y);
			}
			else if (hitObj->getType() == Object::Type::MAP)
			{
				onClickMass(x, y);
			}
		}

		if (hitObj->getType() == Object::Type::UNIT || hitObj->getType() == Object::Type::MAP)
		{
			// 1マップ上の要素をホバー
			int massX = Map::getMassX(x);
			int massY = Map::getMassY(y);
			shared_ptr<Mass> mass = map_->getMass(massX, massY);
			shared_ptr<Unit> unit = map_->getUnit(massX, massY);

			if (mass)
			{
				if (phase_ == Phase::SELECT_ACTION && unit && unit->isEnemy() && mass->state == Mass::ATK_ABLE)
				{
					// 戦闘予測表示
					if (fight_.setPrepare(selectedUnit_.lock(), unit))
					{
						battleUI_.setFightPredict(&fight_);
					}
				}
				else
				{
					if (phase_ != Phase::FIGHT)
					{
						fight_.reset();
						battleUI_.resetFightPredict();
					}

					battleUI_.setTargetMass(mass);
				}
			}
		}
		else
		{
			if (phase_ != Phase::FIGHT)
			{
				fight_.reset();
				battleUI_.resetFightPredict();
			}
		}

	}


	/**
	 * @fn
	 * ユニット クリック時処理
	 */
	void BattleManager::onClickUnit(int x, int y)
	{
		int massX = Map::getMassX(x); // マスのX座標
		int massY = Map::getMassY(y); // マスのY座標
		shared_ptr<Entity::Unit> unit = map_->getUnit(massX, massY); // クリックしたユニット
		bool isEnemy = false;
		
		if (unit)
		{
			isEnemy = unit->isEnemy(); // 敵ユニットであるか
		}
		

		if (phase_ == Phase::SELECT_ACTION) // 行動選択
		{

			if (unit && isEnemy && map_->getMass(massX, massY)->state == Mass::ATK_ABLE)
			{
				atackAction(selectedUnit_.lock(), unit); // 攻撃
			}
			else
			{
				shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
				if (selectedUnit)
				{
					if (selectedUnit->getMassX() == massX && selectedUnit->getMassY() == massY) // 選択中のユニットクリック
					{
						map_->confirmMove(selectedUnit); // 待機
					}
				}
				endSelectActionPhase(); // 行動選択終了
			}
			

			if (!isEnemy && selectedUnit_.expired())
			{
				selectUnit(unit); // 他のユニット選択
			}
		}
		else if (!isEnemy) // プレイヤーユニット
		{
			if (unit == selectedUnit_.lock()) // クリックしたユニットが 選択中のユニットだった場合
			{
				startSelectActionPhase(); // 行動選択フェイズ
			}
			else
			{
				selectUnit(unit); // ユニット選択
			}
		}
		else // 敵ユニット
		{

		}

	}

	/**
	 * @fn
	 * マス クリック時処理
	 */
	void BattleManager::onClickMass(int x, int y)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{

			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (phase_ == Phase::SELECT_ACTION) // 行動選択
			{
				selectedUnitSp->back(); // 移動キャンセル
				endSelectActionPhase(); // 行動選択終了
			}
			else if (targetMass->isMovable())
			{
				selectedUnitSp->move(massX, massY); // 移動
				phase_ = Phase::MOVE;
				return;
			}
			else
			{
				deselectUnit(); // 選択解除
			}
		}
	}

	/**
	 * @fn
	 * アニメーション処理チェック
	*/
	void BattleManager::animationCheck()
	{
		if (phase_ == Phase::MOVE) // 移動
		{
			shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
			if (selectedUnit && !selectedUnit->isAnimation()) // 移動終了
			{
				startSelectActionPhase(); // 行動選択フェイズ
			}
		}
		else if (phase_ == Phase::FIGHT && fight_.checkUpdate()) // 攻撃
		{
			// 攻撃終了
			phase_ = Phase::NORMAL;
		}

	}

	/**
	 * @fn
	 * 行動選択フェイズ開始
	*/
	void BattleManager::startSelectActionPhase()
	{
		map_->clearMassState();
		phase_ = Phase::SELECT_ACTION; // 行動選択 
		map_->displayAtackRange(selectedUnit_.lock());
	}

	/**
	 * @fn
	 * 行動選択終了
	*/
	void BattleManager::endSelectActionPhase()
	{
		map_->clearMassState();
		phase_ = Phase::NORMAL;
		deselectUnit();
	}

	/**
	 * @fn
	 * ユニット選択
	*/
	void BattleManager::selectUnit(shared_ptr<Unit> unit)
	{
		if (deselectUnit()) // 選択解除
		{
			// ユニット選択
			if (unit && unit->select(true))
			{
				selectedUnit_ = unit;
				battleUI_.setTargetUnit(selectedUnit_.lock());
				map_->displayMovableRange(unit);
			}
		}
	}

	/**
	 * @fn
	 * ユニットの選択状態を解除
	 * @return 解除できた場合と選択済みのユニットがない場合はtrueを返す
	*/
	bool BattleManager::deselectUnit()
	{
		if (selectedUnit_.expired())
		{
			return true;
		}
		shared_ptr<Entity::Unit> prevSelectedUnit = selectedUnit_.lock();
		if (prevSelectedUnit)
		{
			if (prevSelectedUnit->select(false))
			{
				selectedUnit_.reset();
				battleUI_.resetTargetUnit();
				map_->clearMassState();
				return true;
			}
			return false;
		}

		// 選択済みのユニットがない場合もtureを返す
		return true;
	}


	/**
	 * @fn
	 * 攻撃
	*/
	void BattleManager::atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
	{
		map_->confirmMove(atkUnit);
		map_->clearMassState();
		deselectUnit();

		if (atkUnit && defUnit)
		{
			fight_.start();
			phase_ = Phase::FIGHT;
		}
	}

}