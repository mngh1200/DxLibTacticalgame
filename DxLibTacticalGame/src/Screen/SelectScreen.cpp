#include "MenuScreen.h"
#include "SelectScreen.h"

namespace Screen
{
	/**
	 * @fn
	 * ��������
	*/
	void SelectScreen::init()
	{


		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::SaveManager& saveManager = Utility::SaveManager::getInstance();

		objectsControl.setLayer(Layer::LEN);

		shared_ptr<Entity::Back> back = make_shared<Entity::Back>();
		back->init(Entity::Back::ScreenKind::SELECT);
		objectsControl.addObject(Layer::BACK, 0, back);
		
		// ����̃e�L�X�g
		objectsControl.addFigure(Layer::UI, make_shared<Entity::Text>("�R�[�X�Z���N�g", COURSE_MARGIN_X, PADDING_TOP, (int)FontType::NORMAL_S24, (int)ColorType::MAIN_COLOR));

		// �X�e�[�W�^�C�g��
		stageTitle_ = make_shared<Entity::Text>("", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, COURSE_TOP, FontType::NORMAL_S32, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, stageTitle_);

		// �������� + �s�k����
		winLabel_ = make_shared<Entity::Text>("��������", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, STAGE_INFO_Y, FontType::NORMAL_S24, ColorType::PLAYER_COLOR);
		objectsControl.addFigure(Layer::UI, winLabel_);

		winValue_ = make_shared<Entity::Text>("", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, STAGE_INFO_Y + LINE_HEIGHT, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, winValue_);

		loseLabel_ = make_shared<Entity::Text>("�s�k����", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, STAGE_INFO_Y, FontType::NORMAL_S24, ColorType::ENEMY_COLOR);
		objectsControl.addFigure(Layer::UI, loseLabel_);

		loseValue_ = make_shared<Entity::Text>("", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, STAGE_INFO_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, loseValue_);

		// �X�e�[�W�q���g
		hintLabel_ = make_shared<Entity::Text>("�q���g", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, STAGE_INFO_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_DARK);
		objectsControl.addFigure(Layer::UI, hintLabel_);

		stageHint_ = make_shared<Entity::Text>("", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, STAGE_INFO_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, stageHint_);

		// �R�[�X�{�^��

		for (int i = 0; i < MAX_STAGE; i++)
		{
			// X���W
			int x = (i % COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_X) + COURSE_MARGIN_X;
			int y = (i / COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_Y) + COURSE_TOP;
			int status = saveManager.getRank(i);

			if (status == StageRank::NEW) // �V�R�[�X
			{
				selectedCourseId_ = newCourseId_ = i;
				saveManager.updateRank(i, StageRank::NONE);
				saveManager.save();
			}
			else if (status == StageRank::NONE && selectedCourseId_ == -1) // ���N���A�R�[�X
			{
				selectedCourseId_ = i;
			}
			else if (status == StageRank::LOCK) // ����J�R�[�X
			{
				continue;
			}

			objectsControl.addObject(Layer::COURSE_BUTTON, i, make_shared<Entity::CourseButton>(x, y, status, i == newCourseId_));
		}

		if (selectedCourseId_ == -1)
		{
			selectedCourseId_ = MAX_STAGE - 1;
		}
		
		// �X�^�[�g�{�^��
		shared_ptr<Entity::TextButton> startBtn = make_shared<Entity::TextButton>(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR);
		startBtn->setShape(LEFT_AREA_WIDTH + START_MARGIN, START_Y, WIN_W - LEFT_AREA_WIDTH - START_MARGIN * 2, START_HEIGHT);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startBtn->setText("�X�^�[�g", FontType::BLACK_S48);
		objectsControl.addObject(Layer::UI, UIid::START_BTN, startBtn);

		// �߂�{�^��
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::MAIN_COLOR, ColorType::SUB_COLOR);
		backBtn->setShape(WIN_W - BACK_SIZE, 0, BACK_SIZE, BACK_SIZE);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("�~", FontType::NORMAL_S32);
		objectsControl.addObject(Layer::UI, UIid::BACK_BTN, backBtn);

		if (selectedCourseId_ != -1)
		{
			updateStageInfo(); // �X�e�[�W���X�V
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
	void SelectScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();


		if (hitObjSp)
		{
			Entity::ObjectsControl& objCont = FrameWork::Game::getInstance().objectsControl;

			// �R�[�X�{�^���̃N���b�N�C�x���g
			if (hitObjSp->getLayerId() == Layer::COURSE_BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// �I�𒆂̃{�^���̉���
				weak_ptr<Entity::Object> prevObjWp = objCont.getObjectWp(Layer::COURSE_BUTTON, selectedCourseId_);
				shared_ptr<Entity::Object> prevObjSp = prevObjWp.lock();
				if (prevObjSp)
				{
					shared_ptr<Entity::CourseButton> prevSelected = dynamic_pointer_cast<Entity::CourseButton>(prevObjSp);
					prevSelected->setSelected(false);
				}

				// �V�����I�𒆂̃{�^����L����
				shared_ptr<Entity::CourseButton> courseBtn = dynamic_pointer_cast<Entity::CourseButton>(hitObjSp);
				courseBtn->setSelected(true);
				selectedCourseId_ = hitObjSp->getObjectId();

				updateStageInfo();
			}
			else if (hitObjSp->getLayerId() == Layer::UI && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// UI�{�^���N���b�N
				if (hitObjSp->getObjectId() == UIid::START_BTN)
				{
					// �X�^�[�g�{�^��
					openScreen_ = Screen::BATTLE;
					createOverlay(false);

					// �T�E���h
					Utility::ResourceManager::playSound(SoundKind::CLICK);
				}
				else if (hitObjSp->getObjectId() == UIid::BACK_BTN)
				{
					// �߂�{�^���@���C�����j���[�ɖ߂�
					openScreen_ = Screen::MAIN_MENU;
					createOverlay(false);

					// �T�E���h
					// Utility::ResourceManager::playSound(SoundKind::BACK);
				}

			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void SelectScreen::updateByAnimation()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();

		if (isOpenOverlayEnded()) // �I�[�o�[���C�iopen�j�I������p
		{
			// NEW�R�[�X�\���A�j���[�V�����V�[���Z�b�g
			if (newCourseId_ != -1)
			{
				nowScene_ = Scene::BORN;
				game.setScreenLock(true);
				game.objectsControl.addAnimationObj(Entity::CourseButton::AnimationId::BORN, Layer::COURSE_BUTTON, newCourseId_);
			}
			else if (selectedCourseId_ != -1)
			{
				nowScene_ = Scene::SELECT;

				// ���N���A�R�[�X��I��
				shared_ptr<Entity::CourseButton> courseBtn = dynamic_pointer_cast<Entity::CourseButton>(game.objectsControl.getObjectWp(Layer::COURSE_BUTTON, selectedCourseId_).lock());
				courseBtn->setSelected(true);
			}
		}
		else if (isCloseOverlayEnded()) // �I�[�o�[���C�iclose�j�I������p
		{
			if (openScreen_ == Screen::MAIN_MENU)
			{
				// ���C�����j���[�ɑJ��
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
			else if (openScreen_ == Screen::BATTLE)
			{
				// �o�g����ʂɑJ��
				BattleScreen* battleScreen = new BattleScreen();
				battleScreen->setStage(selectedCourseId_);
				FrameWork::Game::getInstance().setScreen(battleScreen);
			}
		}
		else if (nowScene_ == Scene::BORN)
		{
			shared_ptr<Entity::Object> obj = game.objectsControl.getObjectWp(Layer::COURSE_BUTTON, newCourseId_).lock();
			if (!obj || obj->isAnimation() == false) // �I��
			{
				nowScene_ = Scene::SELECT;
				game.setScreenLock(false);
			}
		}
	}

	/**
	 * @fn
	 * �I���X�e�[�W���X�V
	*/
	void SelectScreen::updateStageInfo()
	{
		// �X�e�[�W�f�[�^�ǂݍ���
		string title;
		string hint;
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData(stageKind_, selectedCourseId_, &title, &hint, &checkWinData);

		int lineCount = 0;

		// �^�C�g��
		stageTitle_->setText(title.c_str());

		// ���s�������ǂݍ���
		Battle::CheckWin checkWin;
		checkWin.loadData(checkWinData);

		++lineCount; // �����������x����

		// �����������e

		string winValue;
		int winValueLineCount;

		checkWin.getWinConditionsText(&winValue, &winValueLineCount);

		winValue_->setText(winValue.c_str());
		lineCount += winValueLineCount;

		// �s�k�������x���iy���W�̂ݕύX�j
		loseLabel_->setY(STAGE_INFO_Y + LINE_HEIGHT * lineCount);
		++lineCount;
		
		// �s�k�������e
		string loseValue;
		int loseValueLineCount;

		checkWin.getLoseConditionsText(&loseValue, &loseValueLineCount);

		loseValue_->setText(loseValue.c_str());
		loseValue_->setY(STAGE_INFO_Y + LINE_HEIGHT * lineCount);
		lineCount += loseValueLineCount;

		// �q���g
		hintLabel_->setY(STAGE_INFO_Y + HINT_MARGIN_TOP + LINE_HEIGHT * lineCount);
		++lineCount;

		stageHint_->setText(hint.c_str());
		stageHint_->setY(STAGE_INFO_Y + HINT_MARGIN_TOP + LINE_HEIGHT * lineCount);
		



	}

}