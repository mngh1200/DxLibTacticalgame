#include "StartScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void StartScreen::init()
	{
		// �e�X�g
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::TEXT, 0, make_shared<Entity::Text>("PRESS START", WIN_W / 2 - 10, 200));
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void StartScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getLayerId() == Layer::BACK && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			// �I�[�o�[���C�����i��ʑJ�ڗ\��j
			createOverlay(false);
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void StartScreen::updateByAnimation()
	{
		if (isCloseOverlayEnded())
		{
			// �I�[�o�[���C���������Ƃɉ�ʑJ��
			FrameWork::Game::getInstance().setScreen(new MenuScreen());
		}
		
	}
}