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

		constexpr int PADDING_LEFT = 100; //! �����]��
		constexpr int PADDING_TOP = 100; //! �㑤�]��
		constexpr int BUTTON_TOP = 250; //! �{�^��Y���W
		constexpr int BUTTON_MARGIN_Y = 20; //! �{�^���̏c�]��
		constexpr int BUTTON_WIDTH = 400; //! �{�^���̕�
		constexpr int BUTTON_HEIGHT_S = 85; //! �{�^�������iS�T�C�Y�j
		constexpr int BUTTON_HEIGHT_L = 110; //! �{�^���̍����iL�T�C�Y�j


		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("Spirit Wars", PADDING_LEFT, PADDING_TOP, FontType::BLACK_S48, ColorType::TITLE_TEXT));

		// ���j���[�{�^��
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, make_shared<Entity::MenuScreenButton>("�L�����y�[��", PADDING_LEFT, BUTTON_TOP, BUTTON_WIDTH, BUTTON_HEIGHT_L));
		objectsControl.addObject(Layer::UI, UIid::NETWORK_BUTTON, make_shared<Entity::MenuScreenButton>("�ʐM�ΐ�", PADDING_LEFT, BUTTON_TOP + (BUTTON_HEIGHT_L + BUTTON_MARGIN_Y) * 1, BUTTON_WIDTH, BUTTON_HEIGHT_L));
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, make_shared<Entity::MenuScreenButton>("�I��", PADDING_LEFT, BUTTON_TOP + (BUTTON_HEIGHT_L + BUTTON_MARGIN_Y) * 2, BUTTON_WIDTH, BUTTON_HEIGHT_S));


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
					nextScreen_ = new SelectScreen();
					createOverlay(false); // �i�A�j���[�V������Ɂj�Z���N�g��ʂɉ�ʑJ��
				}
				else if (objId == UIid::NETWORK_BUTTON) // �L�����y�[���{�^��
				{
					nextScreen_ = new NetworkScreen();
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

		if (isCloseOverlayEnded() && nextScreen_ != nullptr)
		{
			// �Z���N�g��ʂɉ�ʑJ��
			FrameWork::Game::getInstance().setScreen(nextScreen_);
		}
		
	}
}