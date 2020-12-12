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

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>(Entity::Back::Screen::SELECT));

		int viewId = UIid::UIID_LEN;
		
		// ����̃e�L�X�g
		objectsControl.addFigure(Layer::UI, ++viewId, make_shared<Entity::Text>("�R�[�X�Z���N�g", COURSE_MARGIN_X, PADDING_TOP, ::FontType::NORMAL_S24, ::ColorType::MAIN_COLOR));

		// �R�[�X�^�C�g�� �e�X�g����
		objectsControl.addFigure(Layer::UI, UIid::COURSE_NAME, make_shared<Entity::Text>("�`���[�g���A��1", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, COURSE_TOP, FontType::NORMAL_S32, ColorType::SUB_COLOR));

		// �R�[�X�{�^��
		for (int i = 0; i < 13; i++) // �e�X�g����
		{
			// X���W
			int x = (i % COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_X) + COURSE_MARGIN_X;
			int y = (i / COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_Y) + COURSE_TOP;
			int status = Entity::CourseButton::Status::B;

			// �e�X�g����
			if (i == 2)
			{
				status = Entity::CourseButton::Status::S;
			}
			else if (i == 3)
			{
				status = Entity::CourseButton::Status::A;
			}
			else if (i == 5)
			{
				status = Entity::CourseButton::Status::C;
			}
			else if (i == 12)
			{
				status = Entity::CourseButton::Status::NO_CLEAR;
			}

			objectsControl.addObject(Layer::COURSE_BUTTON, i, make_shared<Entity::CourseButton>(x, y, status));
		}
		
		// �X�^�[�g�{�^��
		shared_ptr<Entity::TextButton> startBtn = make_shared<Entity::TextButton>(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR);
		startBtn->setShape(LEFT_AREA_WIDTH + START_MARGIN, START_Y, WIN_W - LEFT_AREA_WIDTH - START_MARGIN * 2, START_HEIGHT);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startBtn->setText("�X�^�[�g", FontType::BLACK_S48);
		objectsControl.addObject(Layer::UI, UIid::START_BTN, startBtn);

		// �߂�{�^��
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(NULL, ColorType::SUB_COLOR);
		backBtn->setShape(WIN_W - BACK_SIZE, 0, BACK_SIZE, BACK_SIZE);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("�~", FontType::NORMAL_S32);
		objectsControl.addObject(Layer::UI, UIid::BACK_BTN, backBtn);


		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
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
			}
			else if (hitObjSp->getLayerId() == Layer::UI && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// UI�{�^���N���b�N
				if (hitObjSp->getObjectId() == UIid::START_BTN)
				{
					// �X�^�[�g�{�^��
				}
				else if (hitObjSp->getObjectId() == UIid::BACK_BTN)
				{
					// �߂�{�^���@���C�����j���[�ɖ߂�
					openScreen_ = Screen::MAIN_MENU;
					createOverlay(false);
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
		isOpenOverlayEnded(); // �I�[�o�[���C�iopen�j�I������p

		if (isCloseOverlayEnded()) // �I�[�o�[���C�iclose�j�I������p
		{
			if (openScreen_ == Screen::MAIN_MENU)
			{
				// ���C�����j���[�ɑJ��
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
		}
	}

}