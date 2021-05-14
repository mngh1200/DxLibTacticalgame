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

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		constexpr int BUTTON_TOP = 320; //! �{�^��Y���W
		constexpr int BUTTON_MARGIN_Y = 25; //! �{�^���̏c�]��


		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());

		// �^�C�g��
		
		// objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("Spirit Wars", PADDING_LEFT, PADDING_TOP, (int)FontType::BLACK_S48, (int)ColorType::TITLE_TEXT));

		shared_ptr<ImageView> titleImage = make_shared<ImageView>();
		titleImage->setImageId(rm.getImage(ImageType::IMAGE, ImageId::TITLE_IMAGE_ID));
		titleImage->setPos(TITLE_X, TITLE_Y);
		objectsControl.addFigure(Layer::UI, UIid::TITLE, titleImage);


		// ���j���[�{�^��
		shared_ptr<MenuScreenButton> campaignBtn = make_shared<MenuScreenButton>();
		campaignBtn->setY(BUTTON_TOP);
		campaignBtn->setText("�L�����y�[��");
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, campaignBtn);

		shared_ptr<MenuScreenButton> netMatchBtn = make_shared<MenuScreenButton>();
		netMatchBtn->setY(BUTTON_TOP + (MenuScreenButton::H + BUTTON_MARGIN_Y) * 1);
		netMatchBtn->setText("���[�J���ʐM�ΐ�");
		objectsControl.addObject(Layer::UI, UIid::NETWORK_BUTTON, netMatchBtn);

		shared_ptr<MenuScreenButton> quitBtn = make_shared<MenuScreenButton>();
		quitBtn->setY(BUTTON_TOP + (MenuScreenButton::H + BUTTON_MARGIN_Y) * 2);
		quitBtn->setText("�I��");
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, quitBtn);

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

		// �C�x���g�ΏۃI�u�W�F�N�g���Ȃ��ꍇ�A�I��
		if (!hitObjSp)
		{
			return;
		}

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