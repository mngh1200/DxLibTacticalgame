#include "BattleUI.h"

namespace Battle {

	/**
	 * @fn
	 * 初期処理
	*/
	void BattleUI::init(int uiLayerId)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

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