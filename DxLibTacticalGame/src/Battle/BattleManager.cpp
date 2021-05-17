#include "BattleManager.h"
#include "Screen/BattleScreen.h"

namespace Battle {
	/**
	 * @fn
	 * 共通の初期処理
	 * @param (map) マップのポインタ
	 * @param (stageId) ステージID
	 * @param (isSetUnit) ユニット配置シーンの有無取得用
	 */
	void BattleManager::initCommon(shared_ptr<Entity::Map> map)
	{
		this->map = map;
		battleUI.init();
		fight_.init(this->map);

		// メッセージ欄をオブジェクトとして追加
		message = make_shared<Message>();
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addObject(Screen::BattleScreen::Layer::TOP_UI, Screen::BattleScreen::TopUiId::MESSAGE, message);
	}

	/**
	 * @fn
	 * 標準時の初期処理
	 * @param (map) マップのポインタ
	 * @param (stageId) ステージID
	 * @param (isSetUnit) ユニット配置シーンの有無取得用
	 */
	void BattleManager::init(shared_ptr<Entity::Map> map, int stageId, int* setUnitNum, int* aiKind)
	{
		initCommon(map);

		// ステージデータ読み込み
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		vector<int> extraRules;
		Utility::ResourceManager::loadStageData("stage", stageId, &title, &hint, &checkWinData, &extraRules, &mapData, &units);

		this->map->loadStageData(mapData);
		this->map->loadUnits(units);
		checkWin_.loadData(checkWinData);

		// チュートリアル
		tutorial.init(stageId, message);

		// ユニット配置可能数の確認
		*setUnitNum = 0;
		if (extraRules.size() > 0)
		{
			int countMax = extraRules[0];

			if (countMax > 0)
			{
				battleUI.startSelectUnitMode(countMax);
				tutorial.onEvent(TutorialManager::TutorialId::FREE_SET_SELECT, this);
				*setUnitNum = countMax;
			}
		}

		// AIの種類取得
		*aiKind = 0;
		if (extraRules.size() > 1)
		{
			*aiKind = extraRules[1];
		}

		// チュートリアル１～３は敵１対のHPを削っておく
		if (stageId < 3)
		{
			for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
			{
				shared_ptr<Unit> unit = itr->second;
				if (unit->isEnemy())
				{
					unit->forceDecreaseHp(15);
					break;
				}
			}
		}
	}

	/**
	 * @fn
	 * 通信対戦時の初期処理
	 * @param (map) マップのポインタ
	 * @param (stageId) ステージID
	 * @param (isSetUnit) ユニット配置シーンの有無取得用
	 */
	void BattleManager::init(shared_ptr<Entity::Map> map, int stageId, int setUnitNum, bool isServer, shared_ptr<Network::SendManager> sender)
	{
		initCommon(map);

		// ステージファイル名
		string fileName = "match-s";

		if (!isServer)
		{
			fileName = "match-c";
		}

		// ステージデータ読み込み
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		vector<int> extraRules;
		Utility::ResourceManager::loadStageData(fileName, stageId, &title, &hint, &checkWinData, &extraRules, &mapData, &units);

		this->map->loadStageData(mapData);
		this->map->loadUnits(units);
		checkWin_.loadData(checkWinData);

		battleUI.startSelectUnitMode(setUnitNum);

		// 送信管理クラス
		sender_ = sender;
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
				tutorial.onEvent(TutorialManager::TutorialId::MOVE_CONFIRM, this);

				if (isMoveImmdiateConfirm_) // 移動即時確定
				{
					waitAction();
				}
			}
		}
		else if (phase_ == Phase::FIGHT && fight_.checkUpdate()) // 攻撃
		{
			// 攻撃終了

			if (sender_ && isPlayerTurn_) // データ送信
			{
				FightData actData = fight_.getFightData(true);
				FightData psvData = fight_.getFightData(false);

				ContLog contLog = ContLog{ psvData.unit->getMassX(), psvData.unit->getMassY(), actData.unit->getObjectId(), ActionKind::ATACK_ACT, actData.hitState, psvData.hitState };
				sender_->sendPlayerContLog(contLog);
			}

			phase_ = Phase::NORMAL;
			tutorial.onFight(&fight_, TutorialManager::FightPhase::END, this);
			checkWin_.checkWin(map);
			fight_.reset();
		}

	}

	/**
	 * @fn
	 * ゲーム終了判定
	 * @return 終了時:true
	*/
	int BattleManager::checkEnd()
	{
		if (checkWin_.getWinner() != CheckWin::Winner::UNDECIDED)
		{
			return checkWin_.getWinner();
		}
		return CheckWin::Winner::UNDECIDED;
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
		else
		{
			deselectUnit();
		}

		isPlayerTurn_ = isPlayer;

		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
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

		if (isPlayerTurn_)
		{
			// 残りターンメッセージ
			checkWin_.showRemainingTurnMessage(message, getNowTurn());

			// チュートリアル
			tutorial.onPlayerTurnStart(this);

			// 敵の攻撃範囲判定情報更新
			map->updateAttackedArea(true);
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
			map->clearMassState();
			phase_ = Phase::SELECT_ACTION; // 行動選択 
			if (isPlayerTurn_)
			{
				map->displayAtackAbleRange(selectedUnit_, selectedUnit_->getMassX(), selectedUnit_->getMassY());
			}
		}
	}

	/**
	 * @fn
	 * 行動選択終了
	*/
	void BattleManager::endSelectActionPhase()
	{
		map->clearMassState();
		phase_ = Phase::NORMAL;
		deselectUnit();
		checkWin_.checkWin(map);
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

				if (isPlayerTurn_ && !unit->isActed())
				{
					map->displayMovableRange(unit);
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
				map->clearMassState();
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
	 * @param (actHitState) 攻撃仕掛けた側の命中状況（確定している場合だけ引数を追加）
	 * @param (psvHitState) 攻撃された側の命中状況（確定している場合だけ引数を追加）
	*/
	void BattleManager::atackAction(int actHitState, int psvHitState)
	{
		if (isSelectedUnitActive())
		{
			battleUI.onStartFight();
			map->confirmMove(selectedUnit_, sender_);
			fight_.setHitState(actHitState, psvHitState);
			fight_.start();
			map->clearMassState();
			deselectUnit();
			phase_ = Phase::FIGHT;
			tutorial.onFight(&fight_, TutorialManager::FightPhase::START, this);

			if (isPlayerTurn_)
			{
				map->updateAttackedArea(true);
			}
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
			map->confirmMove(selectedUnit_, sender_);

			if (sender_ && isPlayerTurn_) // データ送信
			{
				// 待機
				ContLog contLog = ContLog{ 0, 0, selectedUnit_->getObjectId(), ActionKind::WAIT_ACT };
				sender_->sendPlayerContLog(contLog);
			}

			selectedUnit_->endAction();
			endSelectActionPhase();

			if (isPlayerTurn_)
			{
				map->updateAttackedArea(true);
			}
		}
	}

	/**
	 * @fn
	 * ユニット移動アクション
	 * @param (massX) マス座標X
	 * @param (massY) マス座標Y
	 * @param (isMoveImmdiateConfirm) trueの場合、移動即時確定
	*/
	void BattleManager::moveAction(int massX, int massY, bool isMoveImmdiateConfirm)
	{
		selectedUnit_->move(massX, massY); // 移動
		phase_ = Phase::MOVE;
		isMoveImmdiateConfirm_ = isMoveImmdiateConfirm;
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
		if (!isSelectedUnitActive())
		{
			return;
		}

		if (fight_.setPrepare(selectedUnit_, targetUnit))
		{
			battleUI.setFightPredict(&fight_);

			if (!isPlayerTurn_)
			{
				// 敵軍の操作の場合、攻撃可能エリアの表示を攻撃対象のみにする
				map->clearMassState();
				shared_ptr<Mass> mass = map->getMass(targetUnit->getMassX(), targetUnit->getMassY());
				if (mass)
				{
					mass->state = Mass::ATK_ABLE;
				}
			}
			else
			{
				tutorial.onFight(&fight_, TutorialManager::FightPhase::PREDICT, this);
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
			shared_ptr<Mass> mass = map->getMass(targetUnit->getMassX(), targetUnit->getMassY());
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