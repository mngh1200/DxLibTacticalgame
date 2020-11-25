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

		objectsControl.setLayer(InitLayer::LEN);
		objectsControl.addObject(InitLayer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addObject(InitLayer::BUTTON, 0, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15, 100, 30)));
		objectsControl.addObject(InitLayer::BUTTON, 1, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15 + 50, 100, 30)));
		objectsControl.addFigure(InitLayer::BUTTON, 2, make_shared<Entity::Text>("�^�C�g��", WIN_W / 2 - 10, 200));
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void StartScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		// �e�X�g
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getLayerId() == InitLayer::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			int objId = hitObjSp->getObjectId();
			if (objId == 0)
			{
				FrameWork::Game::getInstance().finish();
			}
			else if (objId == 1)
			{
				hitObjSp->destroy();
			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void StartScreen::updateByAnimation()
	{
	}
}