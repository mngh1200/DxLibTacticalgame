#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * コンストラクタ
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map, int uiLayerId) : BattleManager()
	{
		map_ = map;

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// 地形効果表示欄
		terrainEffectDisplay_ = make_shared<TerrainEffectDisplay>();
		objectsControl.addFigure(uiLayerId, BattleUi::TERRAIN_EFFECT_DISPLAY, terrainEffectDisplay_);
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

	void BattleManager::updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType)
	{
		if (eventType == MOUSE_INPUT_LOG_CLICK)
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

			terrainEffectDisplay_->setTargetMass(map_->getMass(massX, massY), x);
		}
		else
		{
			terrainEffectDisplay_->clear();
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
				atackAction(selectedUnit_.lock(), unit); // 攻撃
			}
			else
			{
				shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
				if (selectedUnit)
				{
					if (selectedUnit->getMassX() == massX && selectedUnit->getMassY() == massY) // 選択中のユニットクリック
					{
						confirmMove(selectedUnit); // 待機
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
		else if (phase_ == Phase::ATACK) // 攻撃
		{
			shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
			if (selectedUnit && !selectedUnit->isAnimation() && defUnit_ && !defUnit_->isAnimation())
			{
				// 攻撃終了
				phase_ = Phase::NORMAL;
				deselectUnit();
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
				displayMovableRange();
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
		map_->clearMassState();

		if (atkUnit && defUnit)
		{
			int damage = atkUnit->atack(defUnit->getX(), defUnit->getY());
			// TODO 地形効果
			if (defUnit->damage(damage))
			{
				// 死亡時
				mapUnits_.erase(make_pair(defUnit->getMassX(), defUnit->getMassY()));
			}
			defUnit_ = defUnit;
			phase_ = Phase::ATACK;
		}
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