#include "BattleManager.h"
#include "Screen/BattleScreen.h"

namespace Battle {
	/**
	 * @fn
	 * コンストラクタ
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map) : BattleManager()
	{
		map_ = map;
		battleUI_.init();
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
				onClickUnit(hitObj);
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
				if (isAtackAble(unit))
				{
					// 戦闘予測表示
					if (fight_.setPrepare(selectedUnit_, unit))
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
	void BattleManager::onClickUnit(shared_ptr<Object> hitObj)
	{
		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(hitObj);
		bool isOwn = !unit->isEnemy(); // 味方ユニットであるか
		

		if (isAtackAble(unit)) // 攻撃対象のユニットクリック
		{
			atackAction(); // 攻撃アクション

		}
		else if (phase_ == Phase::SELECT_ACTION) // 行動選択
		{
			if (unit == selectedUnit_) // 選択中のユニットクリック
			{
				waitAction(); // 待機アクション
			}
			else
			{
				endSelectActionPhase(); // 行動選択終了
				selectUnit(unit); // 他のユニット選択
			}
		}
		else
		{
			if (unit == selectedUnit_)
			{
				if (isOwn)
				{
					// クリックしたユニットが 選択中のユニットだった場合、行動選択フェイズに移行
					startSelectActionPhase();
				}
			}
			else
			{
				selectUnit(unit); // ユニット選択
			}
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

		if (selectedUnit_)
		{

			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (phase_ == Phase::SELECT_ACTION) // 行動選択
			{
				selectedUnit_->back(); // 移動キャンセル
				endSelectActionPhase(); // 行動選択終了
			}
			else if (targetMass->isMovable())
			{
				selectedUnit_->move(massX, massY); // 移動
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
			if (selectedUnit_ && !selectedUnit_->isAnimation()) // 移動終了
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
	 * ターンスタート時処理
	*/
	void BattleManager::onStartTurn(bool isPlayer)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();

		for (auto itr = map_->unitsBegin(); itr != map_->unitsEnd(); ++itr)
		{
			itr->second->turnEnd();
		}
	}

	/**
	 * @fn
	 * 行動選択フェイズ開始
	*/
	void BattleManager::startSelectActionPhase()
	{
		if (!selectedUnit_->isEnemy() && !selectedUnit_->isActed())
		{
			map_->clearMassState();
			phase_ = Phase::SELECT_ACTION; // 行動選択 
			map_->displayAtackRange(selectedUnit_);
		}
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
				battleUI_.setTargetUnit(selectedUnit_);

				if (!unit->isActed())
				{
					map_->displayMovableRange(unit);
				}
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
		if (!selectedUnit_)
		{
			return true;
		}

		if (selectedUnit_)
		{
			if (selectedUnit_->select(false))
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
	void BattleManager::atackAction()
	{
		map_->confirmMove(selectedUnit_);
		fight_.start();
		endSelectActionPhase();
		phase_ = Phase::FIGHT;
	}

	/**
	 * @fn
	 * 待機アクション
	*/
	void BattleManager::waitAction()
	{
		map_->confirmMove(selectedUnit_);
		selectedUnit_->endAction();
		endSelectActionPhase();
	}

	/**
	 * @fn
	 * 対象ユニットに攻撃可能化
	 * @param (targetUnit) 攻撃対象ユニット
	*/
	bool BattleManager::isAtackAble(shared_ptr<Unit> targetUnit) const
	{
		if (targetUnit)
		{
			shared_ptr<Mass> mass = map_->getMass(targetUnit->getMassX(), targetUnit->getMassY());
			return isSelectedUnitActive() && targetUnit->isEnemy() && mass->state == Mass::ATK_ABLE;
		}
		return false;
	}

	/**
	 * @fn
	 * 選択中のユニットが行動可能であるか
	*/
	bool BattleManager::isSelectedUnitActive() const
	{
		return selectedUnit_ && !selectedUnit_->isEnemy() && !selectedUnit_->isActed();
	}

	/**
	 * @fn
	 * 選択中のユニットであるか判定
	 * @param (unit) 判定対象ユニット
	*/
	bool BattleManager::isSelectedUnit(shared_ptr<Unit> unit) const
	{
		return selectedUnit_ == unit;
	}
}