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
	void BattleUI::setFightPredict(const Fight* fight)
	{
		fightPredictDisplay_->emplaceFight(fight);

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
		fightPredictDisplay_->reset();
		mode_ = Mode::NORMAL;
	}
}