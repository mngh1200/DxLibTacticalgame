#include "MenuScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void MenuScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("GAME TITLE", 100, 100, FontType::TITLE, ColorType::TITLE_TEXT));
		/*
		Entity::TextButton templateTextButton = Entity::TextButton()
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, make_shared<Entity::TextButton>("�L�����y�[��", 100, 250, 400, 100));
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, make_shared<Entity::MenuScreenButton>("�I��", 100, 375, 400, 70));
		*/

		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void MenuScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();


		if (hitObjSp)
		{
			// �{�^���̃N���b�N�C�x���g
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (objId == UIid::CAMPAIN_BUTTON) // �L�����y�[���{�^��
				{
					createOverlay(false); // �i�A�j���[�V������Ɂj�Z���N�g��ʂɉ�ʑJ��
				}
				else if (objId == UIid::QUIT_BUTTON) // �I���{�^��
				{
					FrameWork::Game::getInstance().finish();
				}
			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void MenuScreen::updateByAnimation()
	{
		isOpenOverlayEnded();

		if (isCloseOverlayEnded())
		{
			// �Z���N�g��ʂɉ�ʑJ��
			FrameWork::Game::getInstance().setScreen(new SelectScreen());
		}
		
	}
}