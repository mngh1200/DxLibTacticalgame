#include "BattleUI.h"
#include "Screen/BattleScreen.h"

namespace Battle {

	/**
	 * @fn
	 * 初期処理
	*/
	void BattleUI::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		int uiLayerId = Screen::BattleScreen::Layer::UI;

		// 地形効果表示欄
		terrainEffectDisplay_ = make_shared<TerrainEffectDisplay>();
		objectsControl.addFigure(uiLayerId, BattleUIid::TERRAIN_EFFECT, terrainEffectDisplay_);

		// ステータス表示欄
		unitStatusDisplay_ = make_shared<UnitStatusDisplay>();
		objectsControl.addObject(uiLayerId, BattleUIid::UNIT_STATUS, unitStatusDisplay_);

		// 戦闘予測欄
		fightPredictDisplay_ = make_shared<FightPredictDisplay>();
		objectsControl.addObject(uiLayerId, BattleUIid::FIGHT_PREDICT, fightPredictDisplay_);

		// ターン終了ボタン
		turnEndButton_ = make_shared<BuiConfirmButton>();
		turnEndButton_->setText("ターン終了");
		objectsControl.addObject(uiLayerId, BattleUIid::TURN_END_BUTTON, turnEndButton_);

		// 敵ターン中に右下に設置するオブジェクト
		enemyTurnCont_ = make_shared<EnemyTurnCont>();
		objectsControl.addObject(uiLayerId, BattleUIid::ENEMY_TURN_CONT, enemyTurnCont_);

		// 命中率表記
		hitDisplay_ = make_shared<HitDisplay>();
		objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, hitDisplay_);
	}

	/**
	 * @fn
	 * ユニット選択モードを開始
	 * @param (setUnitNum) ユニット最大配置可能数
	*/
	void BattleUI::startSelectUnitMode(int setUnitNum)
	{
		mode_ = Mode::SET_UNITS;

		turnEndButton_->hide();
		enemyTurnCont_->hide();

		// ユニット選択欄設置
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		int layerId = terrainEffectDisplay_->getLayerId(); // レイヤーID、地形効果表示欄と同じ
		
		selectUnitArea_ = make_shared<SelectUnitArea>();
		selectUnitArea_->setCountMax(setUnitNum);
		objectsControl.addObject(layerId, BattleUIid::SELECT_UNIT_AREA, selectUnitArea_);

		// ユニット配置確定ボタン
		confirmUnitSetButton_ = make_shared<BuiConfirmButton>();
		confirmUnitSetButton_->setText("準備完了");
		confirmUnitSetButton_->setDisabled(true);
		objectsControl.addObject(layerId, BattleUIid::CONFIRM_UNIT_SET, confirmUnitSetButton_);
	}

	/**
	 * @fn
	 * ユニット選択モードを終了
	*/
	void BattleUI::endSelectUnitMode()
	{
		mode_ = Mode::NORMAL;

		// ユニット選択欄と準備完了ボタン削除
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.removeObject(selectUnitArea_->getLayerId(), selectUnitArea_->getObjectId());
		objectsControl.removeObject(confirmUnitSetButton_->getLayerId(), confirmUnitSetButton_->getObjectId());
	}

	/**
	 * @fn
	 * ユニット配置カウント増加
	 * @return 最大可能数による制限でカウント増加できなかった場合はfalseを返す
	 */
	bool BattleUI::addSetUnitCount()
	{
		if (selectUnitArea_->addCount())
		{
			if (selectUnitArea_->isMax()) // 上限まで設置されている場合、準備完了ボタンを有効化
			{
				confirmUnitSetButton_->setDisabled(false);
			}
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * ユニット配置カウント減少
	 */
	void BattleUI::removeSetUnitCount()
	{
		selectUnitArea_->removeCount();
		confirmUnitSetButton_->setDisabled(true);
	}

	/**
	 * @fn
	 * 選択中のユニットIDを返す
	 * @return ユニットID　（取得不可の場合 -1）
	*/
	int BattleUI::getSelectedUnitId() const
	{
		if (selectUnitArea_)
		{
			return selectUnitArea_->getSelectUnitId();
		}
		return -1;
	}

	/**
	 * @fn
	 * ターン開始時処理
	*/
	void BattleUI::onStartTurn(bool isPlayer, bool isNetMatch)
	{
		if (isPlayer)
		{
			turnEndButton_->show();
			enemyTurnCont_->hide();
		}
		else
		{
			turnEndButton_->hide();
			enemyTurnCont_->show();
		}
	}

	/**
	 * @fn
	 * 敵プレイヤーの自由配置待機の開始
	*/
	void BattleUI::startWaitEnemySet()
	{
		enemyTurnCont_->setMode(EnemyTurnCont::Mode::ENEMY_SET_MESSAGE);

		turnEndButton_->hide();
		enemyTurnCont_->show();
	}

	/**
	 * @fn
	 * 敵プレイヤーの自由配置待機の終了
	*/
	void BattleUI::endWaitEnemySet()
	{
		enemyTurnCont_->setMode(EnemyTurnCont::Mode::ENEMY_TURN_MESSAGE);
	}

	/**
	 * @fn
	 * イベントチェック
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	*/
	void BattleUI::updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType)
	{
		unitStatusDisplay_->updateByEvents(x, y);
	}


	/**
	 * @fn
	 * アニメーション処理チェック
	*/
	void BattleUI::animationCheck()
	{

	}

	/**
	 * @fn
	 * ユニット選択時処理
	 * @param (unit) 選択ユニット
	*/
	void BattleUI::setTargetUnit(shared_ptr<Unit> unit)
	{
		if (unit && mode_ == Mode::NORMAL)
		{
			unitStatusDisplay_->setTargetUnit(unit);
		}
	}

	/**
	 * @fn
	 * ユニット選択解除時処理
	*/
	void BattleUI::resetTargetUnit()
	{
		unitStatusDisplay_->clear();
	}

	/**
	 * @fn
	 * マス選択時処理
	 * @param (mass) 選択マス
	*/
	void BattleUI::setTargetMass(shared_ptr<Mass> mass)
	{
		if (mass && mode_ == Mode::NORMAL)
		{
			terrainEffectDisplay_->setTargetMass(mass);
		}
	}

	/**
	 * @fn
	 * マス選択解除時処理
	*/
	void BattleUI::resetTargetMass()
	{
		terrainEffectDisplay_->clear();
	}


	/**
	 * @fn
	 * 戦闘予測表示時
	 * @param (fight) 対象の戦闘情報
	*/
	void BattleUI::setFightPredict(Fight* fight)
	{
		fightPredictDisplay_->emplaceFight(fight);

		// ユニットのHPバーに予測ダメージ量を表示
		fight->showPredictUnitDamage();

		// 命中率表示
		hitDisplay_->show(fight);

		if (mode_ == Mode::NORMAL)
		{
			mode_ = Mode::FIGHT_PREDICT;
		}
		
		resetTargetUnit();
		resetTargetMass();
	}

	/**
	 * @fn
	 * 戦闘予測リセット
	*/
	void BattleUI::resetFightPredict()
	{
		hitDisplay_->hide();
		fightPredictDisplay_->reset();
		mode_ = Mode::NORMAL;
	}

	/**
	 * @fn
	 * 戦闘開始時に呼び出される処理
	*/
	void BattleUI::onStartFight()
	{
		// 命中率非表示
		hitDisplay_->hide();
	}
}