#include "BattleManager.h"
#include "Screen/BattleScreen.h"

namespace Battle {
	/**
	 * @fn
	 * 初期処理
	 * @param (map) マップのポインタ
	 */
	void BattleManager::init(shared_ptr<Entity::Map> map)
	{
		map_ = map;
		battleUI.init();
		fight_.init(map_);

		// ステージデータ読み込み
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData("stage", 1, &title, &hint, &mapData, &checkWinData, &units);

		map->loadStageData(mapData);
		map->loadUnits(units);
		checkWin_.loadData(checkWinData);

		// メッセージ欄をオブジェクトとして追加
		message = make_shared<Message>();
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addObject(Screen::BattleScreen::Layer::TOP_UI, Screen::BattleScreen::TopUiId::MESSAGE, message);
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
			checkWin_.checkWin(map_);
		}

	}

	/**
	 * @fn
	 * ゲーム終了判定
	 * @return 終了時:true
	*/
	int BattleManager::checkEnd()
	{
		if (checkWin_.getWinner() != Winner::UNDECIDED)
		{
			return checkWin_.getWinner();
		}
		return Winner::UNDECIDED;
	}

	/**
	 * @fn
	 * ターンスタート時処理
	 * @param (isPlayer) プレイヤーターンを開始する場合 true
	*/
	void BattleManager::onStartTurn(bool isPlayer)
	{
		// 行動未終了ユニットがいた場合、待機アクションさせる
		if (phase_ == Phase::SELECT_ACTION)
		{
			waitAction();
		}

		isPlayerTurn_ = isPlayer;

		for (auto itr = map_->unitsBegin(); itr != map_->unitsEnd(); ++itr)
		{
			itr->second->turnEnd(itr->second->isEnemy() == isPlayer);
		}

		// バトルUI処理
		battleUI.onStartTurn(isPlayer);

		phase_ = Phase::NORMAL;

		// ターン経過
		++turnNumEach_;

		// 勝敗判定
		checkWin_.checkWin(getNowTurn());

		// 残りターンメッセージ
		if (isPlayerTurn_)
		{
			checkWin_.showRemainingTurnMessage(message, getNowTurn());
		}
		
	}

	/**
	 * @fn
	 * 行動選択フェイズ開始
	*/
	void BattleManager::startSelectActionPhase()
	{
		if (isSelectedUnitActive())
		{
			map_->clearMassState();
			phase_ = Phase::SELECT_ACTION; // 行動選択 
			map_->displayAtackAbleRange(selectedUnit_, selectedUnit_->getMassX(), selectedUnit_->getMassY());
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
		checkWin_.checkWin(map_);
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
				battleUI.setTargetUnit(selectedUnit_);

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
				battleUI.resetTargetUnit();
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
		if (isSelectedUnitActive())
		{
			map_->confirmMove(selectedUnit_);
			fight_.start();
			map_->clearMassState();
			deselectUnit();
			phase_ = Phase::FIGHT;
		}
	}

	/**
	 * @fn
	 * 待機アクション
	*/
	void BattleManager::waitAction()
	{
		if (isSelectedUnitActive())
		{
			map_->confirmMove(selectedUnit_);
			selectedUnit_->endAction();
			endSelectActionPhase();
		}
	}

	/**
	 * @fn
	 * ユニット移動アクション
	 * @param (massX) マス座標X
	 * @param (massY) マス座標Y
	*/
	void BattleManager::moveAction(int massX, int massY)
	{
		selectedUnit_->move(massX, massY); // 移動
		phase_ = Phase::MOVE;
	}

	/**
	 * @fn
	 * 移動キャンセル
	*/
	void BattleManager::moveCancel()
	{
		selectedUnit_->back(); // 移動キャンセル
		endSelectActionPhase(); // 行動選択終了
	}


	/**
	 * @fn
	 * 戦闘予測を生成
	 * @param (targetUnit) 攻撃対象マス
	*/
	void BattleManager::setFightPredict(shared_ptr<Unit> targetUnit)
	{
		if (isSelectedUnitActive())
		{
			if (fight_.setPrepare(selectedUnit_, targetUnit))
			{
				battleUI.setFightPredict(&fight_);

				if (!isPlayerTurn_)
				{
					// 敵軍の操作の場合、攻撃可能エリアの表示を攻撃対象のみにする
					map_->clearMassState();
					shared_ptr<Mass> mass = map_->getMass(targetUnit->getMassX(), targetUnit->getMassY());
					if (mass)
					{
						mass->state = Mass::ATK_ABLE;
					}
				}
			}
		}
	}

	/**
	 * @fn
	 * 戦闘予測をクリア
	*/
	void BattleManager::resetFightPredict()
	{
		if (phase_ != Phase::FIGHT)
		{
			fight_.reset();
			battleUI.resetFightPredict();
		}
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
			return isSelectedUnitActive() && targetUnit->isEnemy() == isPlayerTurn_ && mass->state == Mass::ATK_ABLE;
		}
		return false;
	}

	/**
	 * @fn
	 * 選択中のユニットが行動可能であるか
	*/
	bool BattleManager::isSelectedUnitActive() const
	{
		return selectedUnit_ && !selectedUnit_->isEnemy() == isPlayerTurn_ && !selectedUnit_->isActed();
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