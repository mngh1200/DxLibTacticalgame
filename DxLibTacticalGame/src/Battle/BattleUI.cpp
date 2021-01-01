#include "BattleUI.h"

namespace Battle {

	/**
	 * @fn
	 * ��������
	*/
	void BattleUI::init(int uiLayerId)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �n�`���ʕ\����
		terrainEffectDisplay_ = make_shared<TerrainEffectDisplay>();
		objectsControl.addFigure(uiLayerId, BattleUIid::TERRAIN_EFFECT, terrainEffectDisplay_);

		// �X�e�[�^�X�\����
		unitStatusDisplay_ = make_shared<UnitStatusDisplay>();
		objectsControl.addObject(uiLayerId, BattleUIid::UNIT_STATUS, unitStatusDisplay_);
	}

	/**
	 * @fn
	 * �C�x���g�`�F�b�N
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
	*/
	void BattleUI::setTargetUnit(shared_ptr<Unit> unit)
	{
		if (mode_ == Mode::NORMAL)
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
	*/
	void BattleUI::setTargetMass(shared_ptr<Mass> mass)
	{
		if (mode_ == Mode::NORMAL)
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



}