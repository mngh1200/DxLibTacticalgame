#include "BattleScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void BattleScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;
		objectsControl.setLayer(Layer::LEN);

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �w�i
		shared_ptr<Entity::Back> back = make_shared<Entity::Back>(Entity::Back::Screen::BATTLE);
		objectsControl.addObject(Layer::BACK, 0, back);

		// �V�X�e�����j��-
		systemMenu_ = make_shared<Entity::ContextMenu>();
		objectsControl.addObject(Layer::CONTEXT_MENU, 0, systemMenu_);

		systemMenu_->addMenuButton(SystemMenuKey::TURN_END, "�^�[���I��");
		systemMenu_->addMenuButton(SystemMenuKey::CHECK_WIN_TEXT, "���s����");
		systemMenu_->addMenuButton(SystemMenuKey::HINT, "�q���g");
		systemMenu_->addMenuButton(SystemMenuKey::BACK_SELECT_SCREEN, "�Z���N�g��ʂɖ߂�");
		systemMenu_->addMenuButton(SystemMenuKey::BACK_MENU_SCREEN, "���j���[��ʂɖ߂�");

		// �}�b�v�i�}�X�j
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);
		
		// �o�g���Ǘ��p�N���X�̏�������
		if (isNetMatch_) // �ʐM�ΐ�
		{
			btlMng_.init(map, stageId_, setUnitNum_, isServer_);
			playerBtlCont_.init(map);
			enemyBtlCont_.init(map, 0);
		}
		else // �W��
		{
			int aiKind;
			btlMng_.init(map, stageId_, &setUnitNum_, &aiKind);
			playerBtlCont_.init(map);
			enemyBtlCont_.init(map, aiKind);
		}

		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	 * @param (hitObjWp) �ڐG�I�u�W�F�N�g�̎�Q��
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	*/
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{

			if (nowScene_ == Scene::PLAYER_TURN) // �v���C���[�^�[��
			{
				playerBtlCont_.updateByEvents(&btlMng_, hitObjSp, x, y, button, &eventType);

				// �V�X�e�����j���[�֘A�C�x���g
				int systemMenuKey = systemMenu_->checkRunButton(x, y, eventType);

				if (systemMenuKey == SystemMenuKey::TURN_END)
				{
					// �^�[���G���h����
					turnEnd();
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::HINT)
				{
					showHint();
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::CHECK_WIN_TEXT)
				{
					showCheckWinText();
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::BACK_SELECT_SCREEN || systemMenuKey == SystemMenuKey::BACK_MENU_SCREEN)
				{
					// �����ʂɖ߂�
					openScreen_ = systemMenuKey;
					createOverlay(false);
				}
				else if (eventType == MOUSE_INPUT_LOG_UP || (eventType == MOUSE_INPUT_LOG_CLICK && hitObjSp != systemMenu_))
				{
					systemMenu_->hide();

					if (button == MOUSE_INPUT_RIGHT) // �E�}�E�X�_�E��
					{
						systemMenu_->show(x, y);
					}
				}
				
				
				if (eventType == MOUSE_INPUT_LOG_CLICK)
				{
					// �^�[���I���{�^��
					if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::TURN_END_BUTTON)
					{
						turnEnd();
					}
				}
			}
			else if (nowScene_ == Scene::SET_UNITS && eventType == MOUSE_INPUT_LOG_CLICK) // ���j�b�g�z�u�V�[��
			{
				// ���������{�^��
				if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::CONFIRM_UNIT_SET)
				{
					confirmSetUnits();
				}
				else if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::SELECT_UNIT_AREA)
				{
					// ���j�b�g�I���G���A
					btlMng_.tutorial.onEvent(Battle::TutorialManager::TutorialId::FREE_SET_SET, &btlMng_);
				}
				else
				{
					// ���j�b�g�z�u�C�x���g
					SetUnits::onClick(x, y, btlMng_.map, &btlMng_.battleUI, &btlMng_.tutorial, &btlMng_);
				}
			}
			else if (nowScene_ == Scene::RESULT && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// ���s��ʎ��́A�N���b�N���邱�ƂŃZ���N�g��ʂɑJ��
				openScreen_ = SystemMenuKey::BACK_SELECT_SCREEN;
				createOverlay(false);
			}
			else if (nowScene_ == Scene::NETWORK_CLOSE) // ����̃l�b�g���[�N�ؒf��
			{
				if (eventType == MOUSE_INPUT_LOG_CLICK && dialog_.isEqualsOkBtn(hitObjSp)) // �_�C�A���O��OK�{�^���N���b�N��
				{
					openScreen_ = SystemMenuKey::BACK_MENU_SCREEN;
					createOverlay(false);
				}
			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void BattleScreen::updateByAnimation()
	{
		if (isNetMatch_) // �ʐM�ΐ�֘A����
		{
			updateNetwork();
		}

		btlMng_.animationCheck(); // �o�g���Ǘ��n�̏���

		if (nowScene_ == Scene::ENEMY_TURN) // �G�^�[��
		{
			if (enemyBtlCont_.update(&btlMng_))
			{
				turnEnd();
			}
		}

		// ���s����
		int winner = btlMng_.checkEnd();
		if (winner !=  Battle::CheckWin::Winner::UNDECIDED &&
			(nowScene_ == Scene::ENEMY_TURN || nowScene_ == Scene::PLAYER_TURN))
		{
			nowScene_ = Scene::RESULT_ANIME;

			bool isPlayerWin = winner == Battle::CheckWin::Winner::PLAYER;

			ResultScene::makeResultScene(isPlayerWin);
			
			if (isPlayerWin) // �V�R�[�X���֔���
			{
				Utility::SaveManager& saveManager = Utility::SaveManager::getInstance();

				saveManager.updateRank(stageId_, StageRank::CLEAR);

				int newStageId = stageId_ + 1;
				if (saveManager.getRank(newStageId) == StageRank::LOCK)
				{
					saveManager.updateRank(newStageId, StageRank::NEW);
				}

				saveManager.save();
			}

		} 
		else if (nowScene_ == Scene::RESULT_ANIME) // ���ʉ�ʂ̃A�j���[�V�����I������
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			shared_ptr<Entity::Object> retScene = objectsControl.getObjectWp(Layer::TOP_UI, TopUiId::RESULT_SCENE).lock();

			if (retScene && !retScene->isAnimation())
			{
				nowScene_ = Scene::RESULT;
			}
		}
		else if (isOpenOverlayEnded()) // �I�[�o�[���C�J��
		{
			if (setUnitNum_ > 0) // ���j�b�g�z�u
			{
				nowScene_ = Scene::SET_UNITS;
			}
			else
			{
				startBattle();
			}
		}	
		else if (isCloseOverlayEnded()) // �I�[�o���C����
		{
			if (openScreen_ == SystemMenuKey::BACK_MENU_SCREEN)
			{
				// ���j���[��ʂɖ߂�
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
			else if (openScreen_ == SystemMenuKey::BACK_SELECT_SCREEN)
			{
				// �Z���N�g��ʂɖ߂�
				FrameWork::Game::getInstance().setScreen(new SelectScreen());
			}
		}
	}

	/**
	 * @fn
	 * �I���X�e�[�W���Z�b�g
	*/
	void BattleScreen::setStage(int id)
	{
		stageId_ = id;
	}

	/**
	 * @fn
	 * �ʐM�ΐ�ł����Ԃ�����
	 * @param (isServer) �T�[�o�[���ł��邩
	 * @param (mapId) �}�b�vID
	 * @param (unitNum) ���j�b�g��
	*/
	void BattleScreen::prepareNetMatch(int netHandle, bool isServer, int mapId, int unitNum)
	{
		isNetMatch_ = true;
		stageId_ = mapId;
		setUnitNum_ = unitNum;
		isServer_ = isServer;

		sender_.setNetHandle(netHandle);
		receiver_.setNetHandle(netHandle);
	}

	/**
	 * @fn
	 * �ʐM�ΐ�֘A�̍X�V����
	*/
	void BattleScreen::updateNetwork()
	{
		if (nowScene_ == Scene::NETWORK_CLOSE)
		{
			return;
		}

		// �ؒf���ꂽ�ꍇ
		if (DxLib::GetLostNetWork() == receiver_.getNetHandle())
		{
			DxLib::CloseNetWork(receiver_.getNetHandle());
			nowScene_ = Scene::NETWORK_CLOSE;

			// �_�C�A���O�\��
			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			dialog_.show("���肪�ʐM��ؒf���܂���\n���C�����j���[�ɖ߂�܂�", Layer::DIALOG_FRAME, Layer::TOP_UI);
		}

		receiver_.receive(); // �f�[�^��M

		if (nowScene_ == Scene::WAIT_ENEMY_SET) // �G�v���C���[�̃��j�b�g�z�u�҂�
		{
			if (receiver_.getNextSignal() == SignalKind::SET_END) // �G�v���C���[�z�u����
			{
				SetUnits::receiveSetUnitsData(&receiver_, btlMng_.map); // �G���j�b�g�̔z�u
				btlMng_.battleUI.endWaitEnemySet();
				startBattle();
			}
		}
	}

	/**
	 * @fn
	 * �o�g���J�n
	*/
	void BattleScreen::startBattle()
	{
		nowScene_ = Scene::PLAYER_TURN;
		btlMng_.onStartTurn(true);
	}

	/**
	 * @fn
	 * �^�[���I�����i�ʃ^�[���ɐؑ֎��j
	*/
	void BattleScreen::turnEnd()
	{
		Utility::ResourceManager::playSound(SoundKind::CLICK);

		if (nowScene_ == Scene::PLAYER_TURN) // �v���C���[�^�[���I��
		{
			nowScene_ = Scene::ENEMY_TURN;
			btlMng_.onStartTurn(false);
		}
		else if (nowScene_ == Scene::ENEMY_TURN) // �G�^�[���I��
		{
			nowScene_ = Scene::PLAYER_TURN;
			btlMng_.onStartTurn(true);
		}
	}

	/**
	 * @fn
	 * ���j�b�g�ݒu�̏��������{�^���������̏���
	*/
	void BattleScreen::confirmSetUnits()
	{
		btlMng_.battleUI.endSelectUnitMode();
		btlMng_.map->clearMassUnitSet();

		if (isNetMatch_)
		{
			nowScene_ = Scene::WAIT_ENEMY_SET;
			btlMng_.battleUI.startWaitEnemySet();
			SetUnits::sendSetUnitsData(&sender_, btlMng_.map); // �z�u��񑗐M
		}
		else
		{
			startBattle();
		}
	}

	/**
	 * @fn
	 * �q���g�\��
	*/
	void BattleScreen::showHint()
	{
		string title;
		string hint;
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData("stage", stageId_, &title, &hint, &checkWinData);

		btlMng_.message->show(hint, false);
	}

	/**
	 * @fn
	 * ���s�����\��
	*/
	void BattleScreen::showCheckWinText()
	{
		string text = "��������\n";

		string value;
		int lineCount;

		// �����������e
		btlMng_.getCheckWin().getWinConditionsText(&value, &lineCount);
		text += value;

		// �s�k�������x��
		text += "\n\n�s�k����\n";

		// �s�k�������e
		btlMng_.getCheckWin().getLoseConditionsText(&value, &lineCount);
		text += value;

		btlMng_.message->show(text, false);
	}
}