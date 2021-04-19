#include "MenuScreen.h"
#include "SelectScreen.h"

namespace Screen
{
	const char* SelectScreen::STAGE_KIND = "stage";

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

		// �w�i
		shared_ptr<Back> back = make_shared<Entity::Back>();
		back->init(Back::BackKind::DARK_IMAGE);
		objectsControl.addObject(Layer::BACK, 0, back);
		
		// �^�C�g��
		objectsControl.addFigure(Layer::UI, 
			make_shared<Entity::Text>("�X�e�[�W�I��", PADDING, PADDING, (int)FontType::NORMAL_S32, (int)ColorType::SUB_COLOR));

		// �^�C�g���̉���
		shared_ptr<Panel> underLine = make_shared<Panel>();
		underLine->setShape(PADDING, CONTENT_TOP - UNDER_LINE_MARGIN, 500, UNDER_BAR_HEIGHT);
		underLine->setColor(ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, underLine);


		/* �X�e�[�W�I�� �������� */


		// �X�e�[�W�I�𕔕��̃p�l��
		/*
		shared_ptr<Panel> stageListPanel = make_shared<Panel>();
		stageListPanel->setShape(PADDING, CONTENT_TOP, STAGE_LIST_W, STAGE_LIST_H);
		stageListPanel->setColor(ColorType::MAIN_COLOR);
		objectsControl.addFigure(Layer::PANEL, stageListPanel);
		*/

		// �X�e�[�W�{�^��

		for (int i = 0; i < MAX_STAGE; i++)
		{
			int status = saveManager.getRank(i);
			int buttonState = CourseButton::State::NORMAL;

			if (status == StageRank::NEW) // �V�R�[�X
			{
				buttonState = CourseButton::State::NEW;
				selectedCourseId_ = newCourseId_ = i;
				saveManager.updateRank(i, StageRank::NONE); // ��x�Z���N�g��ʂ��J������͖��N���A�X�e�[�W��
				saveManager.save();
			}
			else if (status == StageRank::CLEAR)
			{
				buttonState = CourseButton::State::CLEAR;
			}
			else if (status == StageRank::NONE && selectedCourseId_ == -1) // ���N���A�R�[�X
			{
				selectedCourseId_ = i;
			}
			else if (status == StageRank::LOCK) // ����J�R�[�X
			{
				continue;
			}

			// �X�e�[�W�^�C�g���擾
			string title;
			Utility::ResourceManager::loadStageTitle(STAGE_KIND, i, &title);

			// Y���W
			int y = CONTENT_TOP/* + STAGE_LIST_PADDING*/ + i * (STAGE_BUTTON_H + STAGE_BUTTON_MARGIN);

			objectsControl.addObject(Layer::COURSE_BUTTON, i, 
				make_shared<Entity::CourseButton>(PADDING/* + STAGE_LIST_PADDING*/, y,
					STAGE_LIST_W/* - STAGE_LIST_PADDING * 2*/, STAGE_BUTTON_H, title.c_str(), buttonState));
		}

		if (selectedCourseId_ == -1)
		{
			selectedCourseId_ = MAX_STAGE - 1;
		}
	

		/* �X�e�[�W�I�� �����܂� */



		/* �X�e�[�W��� �������� */


		// �X�e�[�W��񗓃p�l��
		shared_ptr<Panel> stageInfoPanel = make_shared<Panel>();
		stageInfoPanel->setShape(STAGE_INFO_X, CONTENT_TOP, STAGE_INFO_W, STAGE_INFO_H);
		stageInfoPanel->setColor(ColorType::MAIN_COLOR);
		objectsControl.addFigure(Layer::PANEL, stageInfoPanel);

		// �X�e�[�W��񗓃^�C�g��
		shared_ptr<Text> stageInfoTitle = make_shared<Text>("�X�e�[�W���", STAGE_INFO_X, CONTENT_TOP, FontType::NORMAL_S20, ColorType::MAIN_COLOR);
		stageInfoTitle->setBackgroundColor(ColorType::SUB_COLOR);
		stageInfoTitle->setPadding(STAGE_INFO_HEAD_V_PADDING, STAGE_INFO_PADDING);
		stageInfoTitle->setW(STAGE_INFO_W);
		objectsControl.addFigure(Layer::UI, stageInfoTitle);

		// �X�e�[�W�^�C�g��
		stageTitle_ = make_shared<Entity::Text>("", INFO_TEXT_X, INFO_TEXT_Y,
			FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, stageTitle_);

		// �X�e�[�W�^�C�g���̉���
		shared_ptr<Panel> titleUnderLine = make_shared<Panel>();
		titleUnderLine->setShape(INFO_TEXT_X, INFO_TEXT_Y + LINE_HEIGHT, STAGE_INFO_W - STAGE_INFO_PADDING * 2, UNDER_BAR_HEIGHT);
		titleUnderLine->setColor(ColorType::SUB_COLOR_LITE);
		objectsControl.addFigure(Layer::UI, titleUnderLine);


		// �������� + �s�k���� (�ꕔ��Y���W�͌ォ��w��)

		winLabel_ = make_shared<Entity::Text>("��������:", INFO_TEXT_X, INFO_TEXT_Y + LINE_HEIGHT + SECTION_MARGIN, FontType::NORMAL_S20, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(Layer::UI, winLabel_);

		winValue_ = make_shared<Entity::Text>("", INFO_TEXT_X, INFO_TEXT_Y + 2 * LINE_HEIGHT + SECTION_MARGIN, FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, winValue_);

		loseLabel_ = make_shared<Entity::Text>("�s�k����:", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(Layer::UI, loseLabel_);

		loseValue_ = make_shared<Entity::Text>("", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, loseValue_);

		// �A���_�[�o�[
		sectionUnderBar_ = make_shared<Entity::Panel>();
		sectionUnderBar_->setShape(INFO_TEXT_X, 0, STAGE_INFO_W - STAGE_INFO_PADDING * 2, UNDER_BAR_HEIGHT);
		sectionUnderBar_->setColor(ColorType::SUB_COLOR_LITE);
		objectsControl.addFigure(Layer::UI, sectionUnderBar_);


		// �X�e�[�W�q���g
		hintLabel_ = make_shared<Entity::Text>("�q���g:", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(Layer::UI, hintLabel_);

		stageHint_ = make_shared<Entity::Text>("", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, stageHint_);


		/* �X�e�[�W��� �����܂� */


		
		// �X�^�[�g�{�^��
		shared_ptr<Entity::TextButton> startBtn = make_shared<Entity::TextButton>(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR);
		startBtn->setShape(INFO_TEXT_X, BTN_Y, BTN_W, BTN_H);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startBtn->setText("����", FontType::BLACK_S32);
		objectsControl.addObject(Layer::UI, UIid::START_BTN, startBtn);

		// �߂�{�^��
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::SUB_COLOR_LITE2, ColorType::SUB_COLOR);
		backBtn->setShape(INFO_TEXT_X + BTN_W + BTN_MARGIN, BTN_Y, BTN_W, BTN_H);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::SUB_COLOR_LITE, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("�߂�", FontType::BLACK_S32);
		objectsControl.addObject(Layer::UI, UIid::BACK_BTN, backBtn);
		
		// �X�e�[�W���X�V
		updateStageInfo();

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

		// �C�x���g�ΏۃI�u�W�F�N�g���Ȃ��ꍇ�A�I��
		if (!hitObjSp)
		{
			return;
		}


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
			courseBtn->setSelected(true, true);
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

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void SelectScreen::updateByAnimation()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();

		if (isOpenOverlayEnded()) // �I�[�o�[���C�iopen�j�I������p
		{
			if (selectedCourseId_ != -1)
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
		Utility::ResourceManager::loadStageData(STAGE_KIND, selectedCourseId_, &title, &hint, &checkWinData);

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

		// ��s
		++lineCount;

		// �s�k�������x���iy���W�̂ݕύX�j
		loseLabel_->setY(INFO_TEXT_Y + LINE_HEIGHT * lineCount + SECTION_MARGIN);
		++lineCount;
		
		// �s�k�������e
		string loseValue;
		int loseValueLineCount;

		checkWin.getLoseConditionsText(&loseValue, &loseValueLineCount);

		loseValue_->setText(loseValue.c_str());
		loseValue_->setY(INFO_TEXT_Y + LINE_HEIGHT * lineCount + SECTION_MARGIN);
		lineCount += loseValueLineCount;

		// ������
		sectionUnderBar_->setY(INFO_TEXT_Y + LINE_HEIGHT * lineCount + SECTION_MARGIN);

		// �q���g
		hintLabel_->setY(INFO_TEXT_Y + SECTION_MARGIN * 2 + LINE_HEIGHT * lineCount);
		++lineCount;

		stageHint_->setText(hint.c_str());
		stageHint_->setY(INFO_TEXT_Y + SECTION_MARGIN * 2 + LINE_HEIGHT * lineCount);
	}
}