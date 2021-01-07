#include "BattleUI.h"
#include "Screen/BattleScreen.h"

namespace Battle {

	/**
	 * @fn
	 * ��������
	*/
	void BattleUI::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		int uiLayerId = Screen::BattleScreen::Layer::UI;

		// �n�`���ʕ\����
		terrainEffectDisplay_ = make_shared<TerrainEffectDisplay>();
		objectsControl.addFigure(uiLayerId, BattleUIid::TERRAIN_EFFECT, terrainEffectDisplay_);

		// �X�e�[�^�X�\����
		unitStatusDisplay_ = make_shared<UnitStatusDisplay>();
		objectsControl.addObject(uiLayerId, BattleUIid::UNIT_STATUS, unitStatusDisplay_);

		// �퓬�\����
		fightPredictDisplay_ = make_shared<FightPredictDisplay>();
		objectsControl.addObject(uiLayerId, BattleUIid::FIGHT_PREDICT, fightPredictDisplay_);
	}

	/**
	 * @fn
	 * �C�x���g�`�F�b�N
	 * @param (hitObjWp) �ڐG�I�u�W�F�N�g�̎�Q��
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	*/
	void BattleUI::updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType)
	{

	}


	/**
	 * @fn
	 * �A�j���[�V���������`�F�b�N
	*/
	void BattleUI::animationCheck()
	{


	}

	/**
	 * @fn
	 * ���j�b�g�I��������
	 * @param (unit) �I�����j�b�g
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
	 * ���j�b�g�I������������
	*/
	void BattleUI::resetTargetUnit()
	{
		unitStatusDisplay_->clear();
	}

	/**
	 * @fn
	 * �}�X�I��������
	 * @param (mass) �I���}�X
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
	 * �}�X�I������������
	*/
	void BattleUI::resetTargetMass()
	{
		terrainEffectDisplay_->clear();
	}


	/**
	 * @fn
	 * �퓬�\���\����
	 * @param (fight) �Ώۂ̐퓬���
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
	 * �퓬�\�����Z�b�g
	*/
	void BattleUI::resetFightPredict()
	{
		fightPredictDisplay_->reset();
		mode_ = Mode::NORMAL;
	}
}