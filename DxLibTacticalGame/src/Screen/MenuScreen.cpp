#include "MenuScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void MenuScreen::init()
	{
		// �e�X�g
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addObject(Layer::UI, 0, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15, 100, 30)));
		objectsControl.addObject(Layer::UI, 1, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15 + 50, 100, 30)));
		objectsControl.addFigure(Layer::UI, 2, make_shared<Entity::Text>("���j���[", WIN_W / 2, 200, DxLib::CreateFontToHandle("���C���I", 24, 3, DX_FONTTYPE_ANTIALIASING), Entity::Text::Align::CENTER, DxLib::GetColor(124, 104, 95)));

		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void MenuScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		// �e�X�g
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
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
	void MenuScreen::updateByAnimation()
	{
		isOpenOverlayEnded();
	}
}