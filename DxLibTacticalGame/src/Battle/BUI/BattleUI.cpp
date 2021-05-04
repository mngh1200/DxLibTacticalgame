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

		// �^�[���I���{�^��
		turnEndButton_ = make_shared<BuiConfirmButton>();
		turnEndButton_->setText("�^�[���I��");
		objectsControl.addObject(uiLayerId, BattleUIid::TURN_END_BUTTON, turnEndButton_);

		// �G�^�[�����ɉE���ɐݒu����I�u�W�F�N�g
		enemyTurnCont_ = make_shared<EnemyTurnCont>();
		objectsControl.addObject(uiLayerId, BattleUIid::ENEMY_TURN_CONT, enemyTurnCont_);

		// �������\�L
		hitDisplay_ = make_shared<HitDisplay>();
		objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, hitDisplay_);
	}

	/**
	 * @fn
	 * ���j�b�g�I�����[�h���J�n
	 * @param (setUnitNum) ���j�b�g�ő�z�u�\��
	*/
	void BattleUI::startSelectUnitMode(int setUnitNum)
	{
		mode_ = Mode::SET_UNITS;

		turnEndButton_->hide();
		enemyTurnCont_->hide();

		// ���j�b�g�I�𗓐ݒu
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		int layerId = terrainEffectDisplay_->getLayerId(); // ���C���[ID�A�n�`���ʕ\�����Ɠ���
		
		selectUnitArea_ = make_shared<SelectUnitArea>();
		selectUnitArea_->setCountMax(setUnitNum);
		objectsControl.addObject(layerId, BattleUIid::SELECT_UNIT_AREA, selectUnitArea_);

		// ���j�b�g�z�u�m��{�^��
		confirmUnitSetButton_ = make_shared<BuiConfirmButton>();
		confirmUnitSetButton_->setText("��������");
		confirmUnitSetButton_->setDisabled(true);
		objectsControl.addObject(layerId, BattleUIid::CONFIRM_UNIT_SET, confirmUnitSetButton_);
	}

	/**
	 * @fn
	 * ���j�b�g�I�����[�h���I��
	*/
	void BattleUI::endSelectUnitMode()
	{
		mode_ = Mode::NORMAL;

		// ���j�b�g�I�𗓂Ə��������{�^���폜
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.removeObject(selectUnitArea_->getLayerId(), selectUnitArea_->getObjectId());
		objectsControl.removeObject(confirmUnitSetButton_->getLayerId(), confirmUnitSetButton_->getObjectId());
	}

	/**
	 * @fn
	 * ���j�b�g�z�u�J�E���g����
	 * @return �ő�\���ɂ�鐧���ŃJ�E���g�����ł��Ȃ������ꍇ��false��Ԃ�
	 */
	bool BattleUI::addSetUnitCount()
	{
		if (selectUnitArea_->addCount())
		{
			if (selectUnitArea_->isMax()) // ����܂Őݒu����Ă���ꍇ�A���������{�^����L����
			{
				confirmUnitSetButton_->setDisabled(false);
			}
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * ���j�b�g�z�u�J�E���g����
	 */
	void BattleUI::removeSetUnitCount()
	{
		selectUnitArea_->removeCount();
		confirmUnitSetButton_->setDisabled(true);
	}

	/**
	 * @fn
	 * �I�𒆂̃��j�b�gID��Ԃ�
	 * @return ���j�b�gID�@�i�擾�s�̏ꍇ -1�j
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
	 * �^�[���J�n������
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
	 * �G�v���C���[�̎��R�z�u�ҋ@�̊J�n
	*/
	void BattleUI::startWaitEnemySet()
	{
		enemyTurnCont_->setMode(EnemyTurnCont::Mode::ENEMY_SET_MESSAGE);

		turnEndButton_->hide();
		enemyTurnCont_->show();
	}

	/**
	 * @fn
	 * �G�v���C���[�̎��R�z�u�ҋ@�̏I��
	*/
	void BattleUI::endWaitEnemySet()
	{
		enemyTurnCont_->setMode(EnemyTurnCont::Mode::ENEMY_TURN_MESSAGE);
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
		unitStatusDisplay_->updateByEvents(x, y);
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
	void BattleUI::setFightPredict(Fight* fight)
	{
		fightPredictDisplay_->emplaceFight(fight);

		// ���j�b�g��HP�o�[�ɗ\���_���[�W�ʂ�\��
		fight->showPredictUnitDamage();

		// �������\��
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
	 * �퓬�\�����Z�b�g
	*/
	void BattleUI::resetFightPredict()
	{
		hitDisplay_->hide();
		fightPredictDisplay_->reset();
		mode_ = Mode::NORMAL;
	}

	/**
	 * @fn
	 * �퓬�J�n���ɌĂяo����鏈��
	*/
	void BattleUI::onStartFight()
	{
		// ��������\��
		hitDisplay_->hide();
	}
}