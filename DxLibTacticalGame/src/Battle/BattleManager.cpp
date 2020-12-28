#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * コンストラクタ
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map, shared_ptr<Entity::SelectActiveMenu> selectActiveMenu) : BattleManager()
	{
		map_ = map;
		selectActiveMenu_ = selectActiveMenu;
	}

	/**
	 * @fn
	 * マスにユニットへの参照をセット
	 * @param (unit) ユニットの参照
	 * @return 追加成功 : true
	 */
	bool BattleManager::setUnit(shared_ptr<Entity::Unit> unit)
	{
		int x = unit->getMassX();
		int y = unit->getMassY();

		if (map_->isRange(x, y))
		{
			auto ret = mapUnits_.emplace(make_pair(x, y), unit); // 新規追加のみ
			return ret.second;
		}
		return false;
	}


	/**
	 * @fn
	 * ユニット クリック時処理
	 */
	void BattleManager::onClickUnit(int x, int y)
	{
		int massX = Map::getMassX(x); // マスのX座標
		int massY = Map::getMassY(y); // マスのY座標
		shared_ptr<Entity::Unit> unit = getUnitWp(massX, massY).lock(); // クリックしたユニット
		bool isEnemy = false;
		
		if (unit)
		{
			isEnemy = unit->isEnemy(); // 敵ユニットであるか
		}
		

		if (phase_ == Phase::SELECT_ACTION) // 行動選択
		{
			if (isEnemy && map_->getMass(massX, massY)->state == Mass::ATK_ABLE)
			{
				// 攻撃
				atackAction(selectedUnit_.lock(), unit);
			}
		}
		else if (!isEnemy) // プレイヤーユニット
		{
			if (unit == selectedUnit_.lock()) // クリックしたユニットが 選択中のユニットだった場合
			{
				startSelectActionPhase(); // 行動選択フェイズ
			}
			else if (deselectUnit()) // 選択解除
			{
				// ユニット選択
				if (unit && unit->select(true))
				{
					selectedUnit_ = unit;
					displayMovableRange();
				}
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
		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{
			int massX = Map::getMassX(x);
			int massY = Map::getMassY(y);
			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (targetMass->isMovable())
			{
				selectedUnitSp->move(massX, massY); // 移動
				phase_ = Phase::MOVE;
			}
			else
			{
				deselectUnit(); // 選択解除
			}
		}
	}

	/**
	 * @fn
	 * 行動選択
	*/
	void BattleManager::onClickActionMenu(int kind)
	{
		if (kind == -1)
		{
			return;
		}

		shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
		if (selectedUnit)
		{
			if (kind == SelectActiveMenu::ButtonKind::WAIT)
			{
				// 待機
				confirmMove(selectedUnit);
			}
			else if (kind == SelectActiveMenu::ButtonKind::CANCEL)
			{
				// キャンセル
				selectedUnit->back();

			}
		}
		endSelectActionPhase(); // 行動選択終了
	}

	/**
	 * @fn
	 * アニメーション処理チェック
	*/
	void BattleManager::animationCheck()
	{
		if (phase_ == Phase::MOVE)
		{
			shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
			if (selectedUnit && !selectedUnit->isAnimation()) // 移動終了
			{
				startSelectActionPhase(); // 行動選択フェイズ
			}
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
		displayAtackRange();

		shared_ptr<Entity::Unit> selectedUnit = selectedUnit_.lock();
		if (selectedUnit)
		{
			selectActiveMenu_->start(selectedUnit->getX(), selectedUnit->getY());
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
		selectActiveMenu_->end();
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
	 * 移動可能範囲表示
	*/
	void BattleManager::displayMovableRange()
	{
		shared_ptr<Unit> unit = selectedUnit_.lock();
		if (unit)
		{
			int move = unit->getMove();
			int x = unit->getMassX();
			int y = unit->getMassY();
			searchMovableMass(x, y, move);
		}
	}

	/**
	 * @fn
	 * 移動可能範囲の探索
	*/
	void BattleManager::searchMovableMass(int x, int y, int move, bool isInit)
	{
		shared_ptr<Mass> nowMass = map_->getMass(x, y);

		// マップ外
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		shared_ptr<Unit> massUnit = getUnitWp(x, y).lock();
		bool isPlayerUnitOnMass = false; // プレイヤーユニットがマス上に存在するか
		
		// 敵ユニットがいる場合
		if (massUnit)
		{
			if (massUnit->isEnemy())
			{
				return;
			}
			isPlayerUnitOnMass = true && !isInit; // 自身の場合は無視
		}

		// movコスト消費(初回はコスト消費しない)
		if (!isInit)
		{
			move = move - nowMass->getCost();
		}

		if (move > nowMass->passingMov)
		{
			if (!isPlayerUnitOnMass)
			{
				nowMass->state = Mass::State::MOVABLE;
			}

			// マス通過時のmovコストを保持
			nowMass->passingMov = move;

			searchMovableMass(x - 1, y, move, false);
			searchMovableMass(x + 1, y, move, false);
			searchMovableMass(x, y - 1, move, false);
			searchMovableMass(x, y + 1, move, false);
		}
	}

	/**
	 * @fn
	 * ユニットのマス移動（移動確定時）
	*/
	void BattleManager::confirmMove(shared_ptr<Unit> unit)
	{
		int baseX = unit->getBaseX();
		int baseY = unit->getBaseY();
		int massX = unit->getMassX();
		int massY = unit->getMassY();
		
		if (baseX != massX || baseY != massY) // 移動しているときのみ
		{
			unit->setPos(massX, massY);
			mapUnits_.emplace(make_pair(massX, massY), unit);
			mapUnits_.erase(make_pair(baseX, baseY));
		}
	}


	/**
	 * @fn
	 * 攻撃範囲表示
	*/
	void BattleManager::displayAtackRange()
	{
		shared_ptr<Unit> unit = selectedUnit_.lock();
		if (unit)
		{
			int move = unit->getMove();
			int x = unit->getMassX();
			int y = unit->getMassY();
			int range = unit->getRange();

			for (int i = 1; i <= range; i++)
			{
				setAtackMass(x - i, y);
				setAtackMass(x + i, y);
				setAtackMass(x, y - i);
				setAtackMass(x, y + i);
			}
		}
	}

	/**
	 * @fn
	 * 攻撃可能範囲の探索
	*/
	void BattleManager::setAtackMass(int x, int y)
	{
		shared_ptr<Mass> nowMass = map_->getMass(x, y);

		// マップ外
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}
		nowMass->state = Mass::ATK_ABLE;
	}


	/**
	 * @fn
	 * 攻撃
	*/
	void BattleManager::atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
	{
		confirmMove(atkUnit);

		if (atkUnit && defUnit)
		{
			int damage = atkUnit->getAtk();
			defUnit->damage(damage);
		}

		endSelectActionPhase(); // テスト

	}

	/**
	 * @fn
	 * マス座標からユニット取得
	*/
	weak_ptr<Unit> BattleManager::getUnitWp(int massX, int massY)
	{
		pair<int,int> pos = make_pair(massX, massY);
		if (mapUnits_.count(pos) > 0)
		{
			return mapUnits_.at(pos);
		}

		return weak_ptr<Unit>();
	}

}